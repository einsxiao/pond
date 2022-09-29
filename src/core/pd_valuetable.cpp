#include"pd_headers.h"

using namespace std;
using namespace pond;

void ValueTable::VeryInit(){
  stackTable.SetList();
  pairTable.SetList();
  patternTable.SetList(); 
}

ValueTable::ValueTable(){
  VeryInit();
};

ValueTable::ValueTable(Object&pairT){
  stackTable.SetList();
  pairTable      =    pairT;
  patternTable.SetList();
}

ValueTable::ValueTable(Object&pairT,Object&patternT){
  stackTable.SetList();
  pairTable      =    pairT     ;
  patternTable   =    patternT  ;
}

ValueTable::ValueTable(const ValueTable&other){
  stackTable   =  other.stackTable ;
  pairTable    =  other.pairTable ;
  patternTable =  other.patternTable ;
};

ValueTable::~ValueTable(){
  if ( pairTable.cnt() == 0 ){ // surly will erase
    for ( int i=1; i<=pairTable.Size(); i++ ){
      if ( pairTable.ids() > 0 ){
        EvaKernel->ClearEvaRecord( pairTable.ids() );
      }
    }
  }
};

ValueTable&ValueTable::operator=(const ValueTable&other){
  stackTable.CopyObject(   other.stackTable );
  pairTable.CopyObject(    other.pairTable );
  patternTable.CopyObject( other.patternTable );
  return *this;
};



static int patternType(const Object & expr){
  if ( expr.ListQ( SYMID_OF_Pattern) && expr.Size() >=2){
    if ( expr(2).ListQ( SYMID_OF_BlackNullSequence ) ) return 5;
    if ( expr(2).ListQ( SYMID_OF_BlackSequence) ) return 4;
    if (expr(2).ListQ( SYMID_OF_Black) ) return 3;
  }
  if ( expr.ListQ( SYMID_OF_BlackNullSequence) ) return 5;
  if ( expr.ListQ( SYMID_OF_BlackSequence) ) return 4;
  if ( expr.ListQ( SYMID_OF_Black) ) return 3;
  return 0;
}

bool isBlackPattern(const Object&pat){
  for ( u_int i = 0 ; i<= pat.Size(); i++ ){
    if ( patternType( pat(i) ) > 0 )
      return true;
  }
  return false;
}

//  both should be NonBlackPattern
//  -1 <
//   0 =
//  +1 >
int compareNonBlackPattern(const Object&pat1,const Object&pat2){
  if ( pat1.AtomQ() or pat2.AtomQ() )
    { _Erroring("ValueTable","Atom Expression when in TopComparePattern."); return 0; }
  // pat1 and pat2 must be List
  // pat2 is none Sequence Pattern(Black is Allowed) which is in the first part of pattern Table
  int l1 = pat1.Size(), l2 = pat2.Size();
  for ( int i = 0; i<= l1; i++ ){
    if ( i > l2 ){
      return 1;
    }
    if ( pat1(i).ListQ() ){
      if ( pat2(i).ListQ() ){
        return_if_not( Object::SimpleCompare( pat1(i)[0], pat2(i)[0] ) ,0 );
      }else{
        return 1;
      }
    }else{
      if ( pat2(i).ListQ() ){
        return -1;
      }else{
        return_if_not( Object::SimpleCompare( pat1(i), pat2(i) ) ,0 );
      }
    }
  }
  if ( l1 > l2 )
    return 1;
  return 0;
}

//notice that only NonBlackPattern increases, will the v0 be increased
// cause v0 is the start position not the number
bool ValueTable::UpdateOrInsertPattern(const Object&left_in,
                                       const Object&right_in,
                                       const bool onlyUpdate,
                                       const bool tryDel){
  //cout<<"step into insert pattern"<<endl;
  Object newrule; 
  newrule.SetPairCopy(left_in,right_in);
  //cout<<*newrule<<endl;
  Pattern::UnifyRule( (newrule)[1],(newrule)[2] );
  //unify pattern
  // step 1 , find pos in PatternTable
  int first=1,final = patternTable.Size(),result=-1;
  int pos= final+1;
  while ( first <= final ){
    pos = first+(final-first)/2;
    result = compare( left_in.Key(), (patternTable)[pos][0].Key() );
    if ( result == 0 ){
      first = final == pos; break;
    }else if ( result < 0 ){  //  left < pos
      final = pos-1;
    }else{ // result > 0      //  left > pos
      first = pos+1;
    }
  }
  // if no result is find then first > final will occure for sure

  //cout<<"top pos find"<<pos<<","<<result<<endl;
  bool isblack = isBlackPattern(left_in) ;
  if ( result != 0 ){// find the position and insert the record in patternTable
    if ( onlyUpdate or tryDel ){
      return 0;
    }
    if ( result > 0 ) pos++;
    //insert right ahead at pos
    Object rec; rec.SetList( left_in.SymbolId() );  // create MTable
    if ( isblack ){// insert into BTable
      rec.PushBackRef(newrule );
      rec[0].set_re( 1 );//start position of BlackPattern
    }else{ // insert NBTable
      rec[0].set_re( 2 );
      rec.PushBackNull();
      rec[1].PushBackRef(newrule);
    }
    patternTable.InsertRef(pos,rec);
    return 1;
  }//end of insert new in patternTable
  
  //cout<<"top pos find sub table already"<<endl;
  Object&subTable = (patternTable)(pos); // by v0 divided into NBTable and BTable
  if ( not isblack ){
    //cout<<"deal noneblack"<<endl;
    // place find try to find in NBTable
    // v0-1 is the end position of NBTable
    first=1, final = subTable[0].re() -1, result=-1;
    pos= final+1;
    while ( first <= final ){
      pos = first+(final-first)/2;
      result = compareNonBlackPattern( left_in, (subTable)[pos][1][1] );
      if ( result == 0 ){
        first = final == pos; break;
      }else if ( result < 0 ){  //  left < pos
        final = pos-1;
      }else{ // result > 0      //  left > pos
        first = pos+1;
      }
    }
    if ( result != 0 ){//not exist in NBTable insert just ahead of pos
      if ( onlyUpdate or tryDel ){
        return 0;
      }
      if ( result > 0 ) pos++;
      subTable[0].set_re( subTable[0].re() +1 ); // NBTable increases in size or v0 should increase 1
      Object rec; rec.SetList(); 
      rec.PushBackRef(newrule);
      subTable.InsertRef(pos,rec);
      return 1;
    }else{// found in NBTable
      // cause every element of NBTable is a list with same second level head
      // we need to find a proper position to insert the new record
      Object&subsubTable = (subTable)(pos);
      first=1, final = subsubTable.Size(), result=-1;
      int subpos= final+1;
      while ( first <= final ){
        subpos = first+(final-first)/2;
        result = Pattern::PatternCompare( (newrule)[1], (subsubTable)[subpos][1] );
        if ( result == 0 ){
          first = final == subpos; break;
        }else if ( result < 0 ){  //  left < subpos
          final = subpos-1;
        }else{ // result > 0      //  left > subpos
          first = subpos+1;
        }
      }
      if ( result != 0 ){
        if ( onlyUpdate or tryDel ){
          return 0;
        }
        if ( result > 0 ) subpos++;
        // here insert is happened in subsubTable, no change for size of subTable
        //  so v0 should not be changed.
        //  notice insert is to subsubTable not subTable
        subsubTable.InsertRef(subpos,newrule);
        return 1;
      }else{//exist one try update or delete
        if ( tryDel ){
          subsubTable.Delete(subpos);
          if ( subsubTable.Size() == 0 ){
            //if the subsubTable is Empty then delete it
            // this is happened in subTable with subscribe pos
            subTable[0].set_re( subTable[0].re() -1 );
            subTable.Delete(pos);
          }
        }else{//update
          // update is happened in subsubTable
          (subsubTable)[subpos] = (newrule)[2];
        }
        return 1;
      }
    }//end of insert into first part
  }else{// is Black Pattern insert into following
    //cout<<"deal black"<<endl;
    first=subTable[0].re(), final = subTable.Size(), result=-1;
    pos= final+1;
    //cout<<"try find sub pos in black parts"<<endl;
    while ( first <= final ){
      pos = first+(final-first)/2;
      result = Pattern::PatternCompare( (newrule)[1], (subTable)[pos][1] );
      if ( result == 0 ){
        first = final == pos; break;
      }else if ( result < 0 ){  //  left < pos
        final = pos-1;
      }else{ // result > 0      //  left > pos
        first = pos+1;
      }
    }
    //cout<<"pos find"<<endl;
    if ( result != 0 ){//find a position in BTable
      if ( onlyUpdate or tryDel ){
        return 0;
      }
      if ( result > 0 ) pos ++;
      subTable.InsertRef(pos, newrule );
      return 1;
    }else{//exist one
      if ( tryDel ){
        (subTable).Delete(pos);
      }else{ 
        (subTable)[pos][2] = (newrule)[2];
      }
      return 1;
    }
  }//end of Black Pattern insert

}

int ValueTable::GetValueFromPattern(const Object &in,Object&out){
  if ( not in.ListQ() ) {
    _Erroring("ValueTable","Cannot Get value from pattern Table for an Atom Object.");
    return 0;
  }
  //firstly search top head
  // step 1 , find outer most position
  int first=1,final = patternTable.Size(),result=-1;
  int pos= final+1;
  while ( first <= final ){
    pos = first+(final-first)/2;
    result = compare( in.Key(), (patternTable)[pos][0].Key() );
    if ( result == 0 ){
      first = final == pos; break;
    }else if ( result < 0 ){  //  left < pos
      final = pos-1;
    }else{ // result > 0      //  left > pos
      first = pos+1;
    }
  }
  if ( result != 0 )
    return 0;

  //1st find from non Black Patterns
  //find the position
  Object&subTable = (patternTable)(pos);
  first=1, final = subTable[0].re() - 1, result = -1;
  pos= final+1;
  while ( first <= final ){
    pos = first+(final-first)/2;
    result = compareNonBlackPattern( in, (subTable)[pos][1][1] );
    if ( result == 0 ){
      first = final == pos; break;
    }else if ( result < 0 ){  //  left < pos
      final = pos-1;
    }else{ // result > 0      //  left > pos
      first = pos+1;
    }
  }
       
  Index idx =  GlobalPool.Matches.New();
  vector<MatchRecord>&pairs = GlobalPool.Matches.Get(idx);
  if ( result == 0 ){//try to match one by one otherwize goto match BlackPattern one by one
    Object&subsubTable=(subTable)(pos);
    for ( u_int i=1; i<= subsubTable.Size(); i++ ){
      pairs.clear();
      if ( Pattern::UnifiedMatchQ(in, (subsubTable)[i][1], pairs)  ){//match,get value
        if ( &in == &out ){
          Object temp;
          temp.CopyObject( (subsubTable)[i][2] );
          Pattern::UnifiedApplyPatternPairs(temp, pairs);
          out.CopyObject(temp);
          GlobalPool.Matches.FreePairs( idx ); 
          return 1;
        }else{
          out.CopyObject( (subsubTable)[i][2] );
          Pattern::UnifiedApplyPatternPairs(out, pairs);
          GlobalPool.Matches.FreePairs( idx ); 
          return 1;
        }
      }
    }
  }
   
  //2nd match BlackPattern one by one
  for ( u_int i=subTable[0].re(); i<= subTable.Size(); i++ ){
    pairs.clear();
    if ( Pattern::UnifiedMatchQ(in, (subTable)[i][1], pairs)  ){//match,get value
      if ( &in == &out){
        Object temp;
        temp.CopyObject( (subTable)[i][2] );
        Pattern::UnifiedApplyPatternPairs(temp, pairs);
        out.CopyObject( temp );
        GlobalPool.Matches.FreePairs( idx ); 
        return 1;
      }else{
        out.CopyObject( (subTable)[i][2] );
        Pattern::UnifiedApplyPatternPairs(out, pairs);
        GlobalPool.Matches.FreePairs( idx ); 
        return 1;
      }
    }
  }
  return 0;
}

Object ValueTable::UpdateOrInsertPairValue(const Object&left,
                                           const Object&right,
                                           const bool onlyUpdate,
                                           const bool isRef
){
  for ( auto  i = stackTable.Size(); i>=1; i-- ){
    if ( left == stackTable[i] ){
      if ( isRef ) stackTable[i][2] = right;
      else         stackTable[i][2].CopyObject( right );
      return stackTable[i];
    }
  }
  iterator iter;
  if ( pairTable.DictGetPosition(left,iter) == 0 ){
    if ( isRef ) (*iter)[2] = right;
    else         (*iter)[2].CopyObject( right );
    return (*iter);
  }else{
    if ( onlyUpdate ) return Object();
    Object item ; 
    if ( isRef ) item.SetPairRef( left, right );
    else         item.SetPairCopy( left, right );
    pairTable.InsertRef(iter,item);
    return item;
  }
}


Object ValueTable::RefInsertOrUpdatePairValue(const Object&left, const Object&right){
  return UpdateOrInsertPairValue(left,right,false,true);
}

Object ValueTable::UpdateOrInsert(const Object & left,
                                  const Object & right,
                                  const bool onlyUpdate,
                                  const bool isKnownPattern){
  if ( left.NullQ() or not ( left.SymbolQ() or left.ListQ() ) )
    return NullObject;
  if ( not isKnownPattern and not Pattern::PatternListQ(left) ){
    return UpdateOrInsertPairValue(left,right,onlyUpdate);
  }
  bool res = UpdateOrInsertPattern(left,right,onlyUpdate);
  return Object();
}

Object ValueTable::GetOrNewValuePairRef(const Object &left,
                                        const bool onlyGet,
                                        const bool isCheckNoPattern){
  //return the value pair node in value table
  //dout<<"Get or New ValueRef in valueTable of Object "<<left<<" with key: "<<left.Key()<<endl;
  if ( left.Key() == NULL ) {
    return Object();
  }
  if ( isCheckNoPattern or not Pattern::PatternListQ(left) ){
    //dout<<"value is not a pattern, try find value or create pair..."<<endl;
    //dout<<"Current stackTable is "<<stackTable<<endl;
    for ( auto  i = stackTable.Size(); i>=1; i-- ){
      if ( left == stackTable[i][1] ){
        return stackTable[i];
      }
    }
    //dout<<"Current pairTable is "<<pairTable<<endl;
    iterator iter;
    if ( pairTable.DictGetPosition(left,iter) == 0 ){
      //dout<<"Find position with pair: "<<*iter<<endl;
      return *iter; 
    }else{
      //dout<<"Not find a value pair."<<endl;
      if ( onlyGet ){
        //dout<<"Just return."<<endl;
        return Object();
      }
      //dout<<"try to create a value pair."<<endl;
      Object item;
      //dout<<"item created initialy: "<<item.idx<<endl;
      item.SetPairCopy(left,left);
      // item.set_state( Object_State_Block );
      //dout<<"pair created:"<<item<<endl;
      pairTable.InsertRef(iter,item);
      //dout<<"pairTable after insert: "<<pairTable<<endl;
      return item; //pair<Object *,Object *>( item->ElementPtrAt(1) , item->ElementPtrAt(2) );
    }
  }
  return Object();//pair<Object *,Object *>(NULL,NULL);
}

// 0 not find, -1 protected and found, 1 unProtected and found
int ValueTable::GetValueRef(const Object &left,
                            Object &right){
  if ( left.NullQ() ) return -1;
  if ( (left.type() == ObjectType::Number) or (left.type() == ObjectType::String) ) return -1; 

  for ( auto  i = stackTable.Size(); i>=1; i-- ){
    if ( left == stackTable[i] ){
      right = stackTable[i][2];
      right.set_state(1);
      return 1;
    }
  }
  iterator iter;
  if ( pairTable.DictGetPosition(left,iter) == 0 ){
    right = (*iter)[2];
    right.set_state(1);
    return 1;
  }
  return 0;
}

int ValueTable::GetValue(const Object &left,
                         Object &right,
                         const bool onlyCheckPatternTable){
  if ( left.NullQ() ) return 0;
  if ( (left.type() == ObjectType::Number) or (left.type() == ObjectType::String) ) return 0; 
  if ( onlyCheckPatternTable ){
    //dprintf("get %s in patternTable without check.",left.ToString().c_str() );
    return GetValueFromPattern(left,right);
  }
  
  for ( auto  i = stackTable.Size(); i>=1; i-- ){
    if ( left == stackTable[i] ){
      right.CopyObject( stackTable[i][2] );
      return 1;
    }
  }
  iterator iter;
  if ( pairTable.DictGetPosition(left,iter) == 0 ){
    right.CopyObject( (*iter)[2] );
    //dprintf("left %s get normal value %s",left.ToString().c_str(), right.ToString().c_str()  );
    return 1; 
  }
  //dprintf("no left %s normal value",left.ToString().c_str()  );
  // only none atom can matched by a pattern
  if ( left.SymbolQ() ){//a single atom can not construct a pattern, cause like Black[]
    return 0;
  }
  //dprintf("try get %s in patternTable final try.",left.ToString().c_str() );
  return GetValueFromPattern(left,right);
}

// 0 ignore
// 1 done
// -1 ignored cause protected
// stackTable will not influenced by this
int ValueTable::Clear(Object &left,Evaluation*Eva){
  iterator iter;
  if (not Pattern::PatternListQ(left) ){
    if ( pairTable.DictGetPosition(left,iter) == 0 ){
      if ( (*iter).ids() > 0 ){// clear EvaRecord in Evaluation EvaRecordTable
        EvaRecord *rec = Eva->evaRecordTable[ (*iter).ids() ];
        if ( rec != NULL ){ 
          // if ( AttributeQ( rec->attributes, AttributeType::Protected ) ) return -1;
          Eva->ClearEvaRecord( (*iter).ids() );
        }
      }
      (*iter).set_ids( 0 );
      (*iter)[2].CopyObject( (*iter)[1] );
      return 1;
    }
    return 0;
  }
  return UpdateOrInsertPattern(left,left,false,true);
}

int ValueTable::ClearAll(Evaluation *Eva){
  while ( pairTable.Size() > 0 ){
    Eva->ClearEvaRecord( pairTable[1].ids() );
    pairTable[1].set_ids( 0 );
    pairTable[1][2].CopyObject( pairTable[1][1] );
  }
  patternTable.DeleteElements();
  return 0;
}

Object ValueTable::StackPushCopy(const Object&left,const Object&right){
  Object pair(ObjectType::List, SYMID_OF_List);
  pair.PushBackCopy(left);
  pair.PushBackCopy(right);
  //dout<<"push pair "<< pair << " to "<<stackTable<<endl;
  stackTable.PushBackRef(pair);
  //dout<<"after push table: "<<stackTable<<endl;
  return pair;
}

void   ValueTable::StackPushRef(const Object&pair){
  stackTable.PushBackRef(pair);
}

int ValueTable::StackPop(){
  stackTable.PopBack();
  return 0;
}


