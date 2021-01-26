
#include"pd_headers.h"

using namespace std;
using namespace pond;

class __EvaPatternUnifyPairsTable:public __EvaTable< map<u_int,int> >{
public:
  __EvaPatternUnifyPairsTable(){
    tableName = "PatternUnifyPairs";
  };
  void  FreePairs(Index id){
    objs[id.row][id.col].clear();
    ///waitAndLock(); //should check 
    // std::lock_guard<std::mutex> lock(mt);
    freeObjs.push_back(id);
    //unLock();
  }

private:
  void Free();
};

static __EvaPatternUnifyPairsTable UnifyPairsPool;

Pattern::Pattern(){
};

Pattern::~Pattern(){
};

//the pairs is is   SymbolId to Variable ID, so a map can accerlerate the pair process
// Variable coded from 1, zero are left unused
void localUnifyLeft(Object&list,map<u_int,int>&pairs){
  if ( list.NullQ() ) return;
  if ( list.AtomQ() ) return;
  if ( list.ListQ( SYMID_OF_Pattern ) && list.Size() >= 2 ){
    if ( not list[1].SymbolQ() or list[1].ids() == 0 ){
      return;
    }
    // pattern previously occured will auto treated for map will not accept dumplicate value of one key
    auto res = pairs.insert( pair<u_int,int>( list[1].ids(), pairs.size()+1 ) );
    list[1].SetSymbol( SYMID_OF___Variable ); list[1].set_re( (*res.first).second );
    return;
  }
  for ( auto iter = list.ElementsBegin(); iter!= list.ElementsEnd(); iter++){
    localUnifyLeft(*iter,pairs);
  }
  return;
}

void localUnifyRight(Object&list,map<u_int,int>&pairs){
  if ( list.NullQ() ) return;
  if ( list.AtomQ() ){
    if ( list.SymbolQ() && list.ids() != 0 and not list.SymbolQ( SYMID_OF___Variable ) ){
      auto iter = pairs.find(list.ids());
      if (iter != pairs.end() ){
        list.SetSymbol( SYMID_OF___Variable ); list.set_re( (*iter).second );
      }
    }
    return;
  }
  for ( auto iter = list.ElementsBegin(); iter != list.ElementsEnd(); iter++){
    localUnifyRight(*iter,pairs);
  }
  return;
}

// unify a rule is kind of thing that transform expr[x_,y_] -> expr2[x,y] to expr[$_1_,$_2_] -> expr2[$_1,$_2]
int Pattern::UnifyRule(Object&left,Object&right){
  Index idx = UnifyPairsPool.New();
  map<u_int,int> &pairs = UnifyPairsPool.Get(idx);
  localUnifyLeft(left, pairs);
  localUnifyRight(right, pairs);
  int res = pairs.size();
  UnifyPairsPool.FreePairs(idx);
  // left.set_code( res );
  return res;
}

int Pattern::UnifyRuleLeft(Object&left){
  Index idx = UnifyPairsPool.New();
  map<u_int,int> &pairs = UnifyPairsPool.Get(idx);
  localUnifyLeft(left, pairs);
  int res = pairs.size();
  UnifyPairsPool.FreePairs(idx);
  // left.set_code( res );
  return res;
}

int Pattern::UnifyRule( Object & rule ){
  if ( rule.ListQ(SYMID_OF_HoldPattern) && (rule).Size() == 1){
    return UnifyRule(rule[1]);
  }
  if ( not RuleQ(rule) ){
    zhErroring("标准化规则","尝试标准化的表达式不是规则")||
      Erroring("UnifyRule","Try to unify a non-rule-form Expression.");
    return 8;
  }
  return UnifyRule( rule[1],rule[2] );
}

// attributes OneIdentity is very important when matching

// the pattern is LEFT part of the unified rule
// Pattern    name_Head?Func/;Func2[name].default
//          or name:
// Pattern[name(could be Null),form(one of black blackSeq. , blackNullSeq.),test_function, test_expr, default]
//              ...                ...                                        ?e                /;e         .e
// pairs 0:nothin  1: VariableT1  2:VariableT2 ...
// Viariable 0: means unamed pattern
// in this function pair_size is the size of valid pairs not the data length of pairs. so acctually pair_size == pairs.size()-1
#define return_false ({pair_size = old_pair_size; return false;})
bool UNIFY_MATCHQ(const Object&expr,const Object &pat, vector<MatchRecord> &pairs,u_int&pair_size, u_int pos1,u_int pos2,bool isShortestMatch){
  int old_pair_size = pair_size;
  //dout<< " try match "<< expr<<" to "<<pat<< " pair_size = "<< pair_size<<" pos1 ="<<pos1<<" pos2="<<pos2<<endl;
  //situation I: whole expression match
  if ( pos1 == 0 and pos2 == 0 ){
    /////situation I.1: expr is HoldOnce or HoldPattern
    if ( expr.ListQ() and expr.Size() == 1 and EvaKernel->AttributeQ(expr(0), AttributeType::OneIdentity ) ){
      //dout<< expr << " is an indentiry expression "<<endl;
      if ( UNIFY_MATCHQ( expr(1),pat,pairs,pair_size,0,0,isShortestMatch) )
        return true;
      // return_false; // should not instantly, for it may match without indentity
    }
    ///// situation I.2: pat is a Pattern
    if ( pat.ListQ() ){
      if ( pat.ListQ( SYMID_OF_Hold ) ||
           pat.ListQ( SYMID_OF_HoldOnce ) ||
           pat.ListQ(SYMID_OF_HoldPattern ) ||
           pat.ListQ(SYMID_OF_Parenthesis ) ){
        if ( pat.Size() != 1 ){
          Erroring("Pattern",pat.ToString() + " is in the wrong form.");
          return_false;
        }
        //dout<<" try pat oneidentiry "<< pat <<endl;
        if ( UNIFY_MATCHQ( expr,pat(1),pairs,pair_size,0,0,isShortestMatch))
          return true;
        return_false; //same reason as one
      }else if ( pat.ListQ( SYMID_OF_Pattern ) ){
        if ( pat.Size() < 2 ){
          Erroring("Pattern",expr.ToString()+" is in the wrong form.");
          return_false;
        }
        //dprintf("pat %s is a Pattern, expr = %s",pat.ToString().c_str(),expr.ToString().c_str() );
        unsigned int varI = pat(1).re();
        if ( varI != 0  ){
          if ( varI > pair_size + 1 ){
            Erroring("Pattern","Unified Pattern is in the wrong form.");
            return_false; 
          }
          if ( varI <= pair_size ){ // VariableT already recorded
            if ( pairs[varI].n == 1 && (Object(pairs[varI].idx) == expr ) )
              return true;
            return_false;
          }
          //Pattern not exist, Record new Single match
          //dprintf("record new var %d",varI);
          pair_size ++; while ( pair_size >= pairs.size() ) pairs.push_back( MatchRecord() );
          pairs[varI].n = 1;
          pairs[varI].p = 0;
          pairs[varI].idx = expr.objid;
        }//varI == 0 of unamed situation has the same condition check as named ones.
        //check requirements
        // Head requirement for Black, BlackSequence, BlackNullSequence will be checked
        if ( pat(2).PairQ(SYMID_OF_Query) ){
          Object&query = pat(2);
          if ( not UNIFY_MATCHQ(expr, query(1), pairs,pair_size, 0, 0 ,isShortestMatch) ) 
            return_false;
          Object nobj = query(2).Copy();
          nobj.Deepen(); 
          nobj.PushBackRef( expr );
          EvaKernel->Evaluate( nobj );
          if ( nobj.Boolean() > 0 ) return true;
          return_false;
        }else{
          if ( not UNIFY_MATCHQ(expr, pat(2), pairs,pair_size, 0, 0 ,isShortestMatch) ) 
            return_false;
        }
        return true;
      //// situation I.3: pat is one of Black, BlackSequence and BlackNullSequence
      }else if ( pat.PairQ(SYMID_OF_Query) ){
        const Object&query = pat;
        if ( not UNIFY_MATCHQ(expr, query(1), pairs, pair_size, 0, 0, isShortestMatch) )
          return_false;
        Object nobj = query(2).Copy();
        nobj.Deepen(); 
        nobj.PushBackRef( expr );
        EvaKernel->Evaluate( nobj );
        if ( nobj.Boolean() > 0 ) return true;
        return_false;
      }else if ( pat.ListQ(SYMID_OF_Black ) ||
                 pat.ListQ(SYMID_OF_BlackSequence ) ||
                 pat.ListQ(SYMID_OF_BlackNullSequence ) ){
        //dprintf(" _ __ ___ match for %s", expr.ToFullFormString().c_str() );
        // sequence will not direct match. Aditional flatten operation needed
        if ( expr.ListQ(SYMID_OF_Sequence) )
          return_false;
        if ( pat.Size()<1 ){ // no head requirement
          return true;
        }else{//judge the head in Black[String] whether equal "abc" or Black[Object ]  {a,b,c}
          //dprintf(" _ __ ___ test for expre head %s", expr.Head() );
          if ( not pat(1).SymbolQ() ) return_false;

          if ( pat(1).SymbolQ( SYMID_OF_List ) ){
            if ( expr.ListQ() ) return true;
          }
          if ( pat(1).SymbolQ( expr.Head() ) ) return true;
          return_false;
        }
        //end of situation I.3
      }else{//pat is a List, rather than Pattern, Black, BlackSequence, BlackNullSequence
        if ( expr.AtomQ() ){//for sure will not match
          return_false;
        }// otherwize test match in situation II
      }
    }else{//pat.AtomQ()
    //// situation I.4: of at least one is atom
      if ( expr.AtomQ() ){
        return Object::SimpleCompare( expr, pat) == 0; 
      }else{
        return_false;
      }
    }
  }//end of situation I

  // situation II
  // both are list. 3 situations
  // 1: pos2 > pat.Size;
  // 2: pos1 > expr.Size expr could be Atom. so Size may not applied.
  // 3: both are not at the end of the list
  // situation II.1: dealing expr or pat comes to the end
  if ( pos2 > pat.Size() ){ 
    if ( pos1 > expr.Size() )
      return true;
    return_false;
  }else if ( pos1 > expr.Size() ){
    // situation II.1: expr is end but pat[pos2] can match null, proceed.
    // Query will not match this case, for it need eval function with something as input
    if ( pat(pos2).ListQ( SYMID_OF_BlackNullSequence ) )//last one can represent null and no name 
      return UNIFY_MATCHQ(expr,pat,pairs,pair_size,pos1,pos2+1,isShortestMatch);
    if ( (pat(pos2).ListQ( SYMID_OF_Pattern) ) and
         (pat(pos2).Size() >=2 ) and
         (pat(pos2)[2].ListQ( SYMID_OF_BlackNullSequence ) ) ){//name BlackNullSequence
      u_int varI = pat(pos2)[1].re();
      if ( varI != 0  ){ // pattern exist
        if ( varI > pair_size + 1 ){
          Erroring("Pattern","Coding of unified pattern is not right.");
          return_false; 
        }
        if ( varI <= pair_size ){ // VariableT already recorded
          if ( pairs[varI].n != 0 ) // should be null match, otherwize return_false
            return_false;
        }else{ // pattern not exist, push a new record represent null match
          pair_size ++;
          while ( pair_size >= pairs.size() )
            pairs.push_back( MatchRecord() );
          pairs[pair_size].n = 0;
        }
      }//unamed pattern, just try next match
      if ( not UNIFY_MATCHQ(expr,pat,pairs,pair_size,pos1,pos2+1,isShortestMatch) ){
        return_false;
      }
      return true;
    }
    return_false;
  }
  // situation 3
  // elements
  // BlackSequence and BlackNullSequence
  bool singleq  = false;
  bool nullq    = false;
  bool queryq   = false;
  //situation II.2: pat[pos2] is pattern and its content is BlackSequence or BlackNullSequence
  if ( pat(pos2).ListQ( SYMID_OF_Pattern )  ){
    if ( pat(pos2).Size() < 2 ){
      Erroring("Pattern","Pattern is in the wrong form.");
      return_false; 
    }
    if ( (singleq=pat(pos2)[2].ListQ(SYMID_OF_Black ) )   or
         (pat(pos2)[2].ListQ( SYMID_OF_BlackSequence ) )  or
         (nullq=pat(pos2)[2].ListQ(SYMID_OF_BlackNullSequence ) ) or
         ( (queryq=pat(pos2)[2].PairQ(SYMID_OF_Query)  ) and 
           (singleq = pat(pos2)[2](1).ListQ(SYMID_OF_Black)) )
    ) {
      //dout<< "1 into pos match all: "<< expr<<"   ?   "<< pat <<endl;
      //dout<< "1 into pos match pos: "<< expr(pos1)<<"   ?   "<< pat(pos2) <<endl;
      //////pattern of form Black, BlackNullSequence or BlackSequence
      u_int varI = pat(pos2)[1].re();
      if ( varI != 0  ){ // pattern is a named pattern try to record or compare to what already recorded
        //dout<<" is a named pattern "<<endl;
        if ( varI > pair_size + 1 ){
          //dprintf("error pat: %s",pat.ToString().c_str() );
          //dprintf("varI = %d, pair_size=%d, real pair size = %d",varI,pair_size,(int)pairs.size() );
          Erroring("Pattern","Coding of unified pattern is not right.");
          return_false;
        }
        if ( varI <= pair_size ){ // VariableT already recorded // pattern Exist
          //dout<<"aready a recored pattern"<<endl;
          if ( expr.Size() - pos1 + 1 < pairs[varI].n ) //rest elements are not enough 
            return_false;
          if ( pairs[varI].n == 1 ){
            if ( Object(pairs[varI].idx) != expr(pos1) )
              return_false;
          }else{//null match for nullsequence will processed by for clause
            for ( u_int i = 0; i< pairs[varI].n; i++){
              vector<Object>&tlist=GlobalPool.Lists.Get( pairs[varI].idx );
              if ( pairs[varI].p + i >= tlist.size() ){
                //dprintf("\nexpr=%s,\npat=%s, \n pair_size=%d pos1=%d, pos2=%d \n n=%d,size=%d varI=%d ", expr.ToString().c_str(), pat.ToString().c_str(), pair_size, pos1, pos2, pairs[varI].n, (int)tlist.size(),(int)varI );
                Erroring("MatchQ","recorded matched list index out of range.");
                return_false;
              }
              if ( tlist[ pairs[varI].p+i ] != expr(pos1+i) )
                return_false;
            }
          }
          if ( not UNIFY_MATCHQ( expr, pat, pairs,pair_size, pos1 + pairs[varI].n, pos2+1,isShortestMatch) )
            return_false;
          return true;
        }else{ // pattern not exist Record new match record // varI == pair_size + 1 
          //dout<<"not a recored pattern"<<endl;
          pair_size ++; while ( pair_size >= pairs.size() ) pairs.push_back( MatchRecord() );
          pairs[varI].n = 0;
          if ( singleq ){//Form Black, only single match
            //dout<<"is single q"<<endl;
            //check if Match at current position
            if ( queryq ){
              Object &query = pat(pos2)(2);
              //dout<<"is a query"<<endl;
              //dout<<" try match "<< expr(pos1)<<"  ???  "<<pat(pos2)<<endl;
              if ( not ({
                    u_int new_pair_size = pair_size -1;
                    UNIFY_MATCHQ( expr(pos1), query(1), pairs,new_pair_size,0,0,isShortestMatch);
                  }))
                return_false;
              Object nobj = query(2).Copy();
              nobj.Deepen(); 
              nobj.PushBackRef( expr(pos1) );
              //dout<<"do query test with "<< nobj <<endl;
              EvaKernel->Evaluate( nobj );
              if ( nobj.Boolean() <= 0 ){
                //dout<<" query test failed "<<endl;
                return_false;
              }
              //dout<<" query test success"<<endl;
            }else{
              if ( not ({u_int new_pair_size = pair_size -1;UNIFY_MATCHQ( expr(pos1), pat(pos2)(2), pairs,new_pair_size,0,0,isShortestMatch);}) )
                return_false;
            }
            pairs[varI].n = 1;
            pairs[varI].p = 0;
            pairs[varI].idx = expr(pos1).objid;
            if ( not UNIFY_MATCHQ( expr, pat, pairs, pair_size, pos1+1, pos2+1,isShortestMatch) )
              return_false;
            return true;
          }else if ( isShortestMatch ){//shortest match
            // zero match of current pat
            if ( nullq and ({ u_int new_pair_size = pair_size -1;UNIFY_MATCHQ( expr, pat, pairs, new_pair_size, pos1,pos2+1 ,isShortestMatch);}) )
              return true;
            // 1, 2 or more
            for ( u_int i = pos1; i<= expr.Size(); i++ ){
              //check if match at expr[pi]
              if ( not ({ u_int new_pair_size = pair_size -1;UNIFY_MATCHQ( expr(i), pat(pos2), pairs,new_pair_size, 0,0, isShortestMatch);}) )
                return_false;
              // record current match in pairs and try following match
              if ( i == pos1 ){//1
                pairs[varI].n = 1;
                pairs[varI].p = 0;
                pairs[varI].idx = expr(pos1).objid;
              }else if ( i > pos1 ){// 2 or more 
                pairs[varI].n = i - pos1 + 1;
                pairs[varI].p = pos1;
                //pairs[varI].idx = expr.objid;
                pairs[varI].idx = expr.idx();
              }
              if ( UNIFY_MATCHQ( expr, pat, pairs,pair_size, i+1,pos2+1,isShortestMatch) )
                return true;
            }
            return_false;
          }else{ // longest match
            u_int pi = 0;
            for ( pi= pos1; pi<=expr.Size(); pi++){//test each if match at every position
              //check if match at expr[pi]
              if ( not ({u_int new_pair_size = pair_size -1; UNIFY_MATCHQ( expr(pi), pat(pos2),pairs,new_pair_size,0,0,isShortestMatch);}) )
                break;
            }
            for ( u_int i = pi-1; i>= pos1; i-- ){//record possible match, and try following match
              if ( i > pos1 ){ // 2 ore more
                pairs[varI].n = i - pos1 +1 ;
                pairs[varI].p = pos1;
                //pairs[varI].idx = expr.objid;
                pairs[varI].idx = expr.idx();
              }else{ // i == pos1  //1
                pairs[varI].n = 1;
                pairs[varI].p = 0;
                pairs[varI].idx = expr(pos1).objid;
              }
              if ( UNIFY_MATCHQ( expr, pat, pairs,pair_size, i+1,pos2+1,isShortestMatch) )
                return true;
            }
            pairs[varI].n = 0;
            if ( nullq and UNIFY_MATCHQ( expr, pat, pairs,pair_size, pos1,pos2+1 ,isShortestMatch) )
              return true;
            return_false;
          }
        }
        //situation II.3: pat[pos2] is pattern and its content is BlackSequence or BlackNullSequence, but without a code
      }else{ // unamed pattern, no need to record new pair, different from  named match
        goto unamed_pattern_match_label;
      }//end of unamed pattern
      //situation II.4: pat[pos2] is BlackSequence or BlackNullSequence, but without a code
    }else{//not one of Black BlackSequence or BlackNullSequence
      //dout<< " try match "<< expr<<" to "<<pat<< " pair_size = "<< pair_size<<" pos1 ="<<pos1<<" pos2="<<pos2<<endl;
      //dout<< expr(pos1) << "  ?  "<< pat(pos2) <<endl;
      Erroring("Pattern","Content of pattern is required to be Black, BlackSequence, BlackNullSequence or Query.");
      return_false;
    }
  }else if ( (singleq = pat(pos2).ListQ( SYMID_OF_Black ) ) or
             (pat(pos2).ListQ( SYMID_OF_BlackSequence ) ) or
             (nullq=pat(pos2).ListQ( SYMID_OF_BlackNullSequence ) ) or
             ( (queryq = pat(pos2).PairQ(SYMID_OF_Query)) and (singleq = pat(pos2)(1).ListQ(SYMID_OF_Black) ) )
  ){
  unamed_pattern_match_label:
    //dout<< "2 into pos match "<<expr(pos1)<<"   ? "<< pat(pos2) <<endl;
    // another kind of unamed pattern
    if ( singleq ){//single match
      //check if Match at current position
      if ( not queryq ){
        if ( not UNIFY_MATCHQ( expr(pos1), pat(pos2),pairs,pair_size,0,0,isShortestMatch) ){
          return_false;
        }
      }else{
        Object&query = pat(pos2);
        if ( not UNIFY_MATCHQ( expr(pos1), query(1) ,pairs, pair_size, 0, 0, isShortestMatch) ){
          return_false;
        }
        Object nobj = query(2).Copy();
        nobj.Deepen(); 
        nobj.PushBackRef( expr(pos1) );
        EvaKernel->Evaluate( nobj );
        if ( nobj.Boolean() <= 0 ) return_false;
      }
      if ( UNIFY_MATCHQ( expr, pat, pairs, pair_size, pos1+1, pos2+1,isShortestMatch) )
        return true;
      return_false;
    }else if ( isShortestMatch ){//shortest match
      if ( nullq and UNIFY_MATCHQ( expr, pat, pairs,pair_size, pos1,pos2+1 ,isShortestMatch) )
        return true;
      for ( u_int i = pos1; i<= expr.Size(); i++ ){
        //check only head condition 
        if ( not UNIFY_MATCHQ( expr(i), pat(pos2), pairs,pair_size, 0, 0, isShortestMatch ) )
          return_false;
        if ( UNIFY_MATCHQ( expr, pat, pairs,pair_size, i+1,pos2+1,isShortestMatch) )
          return true;
      }
      return_false;
    }else{ // longest match
      u_int pi = 0;
      for ( pi= pos1; pi<=expr.Size(); pi++){
        //check head and other requirements
        if ( not UNIFY_MATCHQ( expr(pi), pat(pos2), pairs, pair_size, 0, 0, isShortestMatch) )
          break;
      }
      for ( u_int i = pi-1; i>= pos1; i-- ){
        if ( UNIFY_MATCHQ( expr, pat, pairs, pair_size, i+1,pos2+1,isShortestMatch) )
          return true;
      }
      if ( nullq and UNIFY_MATCHQ( expr, pat, pairs, pair_size, pos1,pos2+1 ,isShortestMatch) )
        return true;
      return_false;
    }
    // end of directly BlackSequence and BlackNullSequnce
  }
  //situation II.6: other situations
  if ( not UNIFY_MATCHQ( expr(pos1), pat(pos2), pairs,pair_size,0,0 ,isShortestMatch) )
    return_false;
  //process to next match position
  if ( UNIFY_MATCHQ( expr,pat, pairs,pair_size, pos1+1, pos2+1,isShortestMatch) )
    return true;
  return_false;
}
#undef return_false

int Pattern::UnifiedMatchQ(const Object&expr,const Object&pat){
  Index idx = GlobalPool.Matches.New();
  vector<MatchRecord>&pairs = GlobalPool.Matches.Get( idx );
  // pairs.reserve( pat.code()+1 );
  pairs.clear();
  u_int pair_size = 0;
  bool res = UNIFY_MATCHQ(expr,pat,pairs,pair_size,0,0,false);
  GlobalPool.Matches.FreePairs( idx );
  return res;
}

int Pattern::UnifiedMatchQ(const Object&expr,const Object&pat,vector<MatchRecord>&pairs){
  // pairs.reserve( pat.code() +1 );
  u_int pair_size = 0;
  return UNIFY_MATCHQ(expr,pat,pairs,pair_size,0,0,false);
}

static int __patternType(const Object & expr){
  if ( expr.ListQ( SYMID_OF_Pattern ) && expr.Size() >=2){
    if ( expr(2).ListQ(SYMID_OF_BlackNullSequence ) ) return 5;
    if ( expr(2).ListQ(SYMID_OF_BlackSequence ) ) return 4;
    if ( expr(2).ListQ(SYMID_OF_Black ) ) return 3;
  }
  if ( expr.ListQ(SYMID_OF_BlackNullSequence) ) return 5;
  if ( expr.ListQ(SYMID_OF_BlackSequence) ) return 4;
  if ( expr.ListQ(SYMID_OF_Black) ) return 3;
  return 0;
}

// -5 -1(unknow but less) 0(complete same) 1(unknown expr more) 5
// -1 <
//  0 == 
//  1 >
int Pattern::PatternCompare(const Object &pat1,const Object &pat2){
  //cout<<pat1<<","<<pat2<<endl;
  switch ( pat1.type() ){
  case ObjectType::Number: 
    switch ( pat2.type() ){
    case ObjectType::Number: 
      return compare( pat1.Number(),pat2.Number() );
    default:
      return -1;
    }
  case ObjectType::String:
    switch ( pat2.type()){
    case ObjectType::Number: return 1;
    case ObjectType::String: return compare(pat1.Key(),pat2.Key());
    default: return -1;
    }
  case ObjectType::Symbol:
    switch ( pat2.type() ){
    case ObjectType::Number: case ObjectType::String: return 1;
    case ObjectType::Symbol: return compare( pat1.SymbolWeight(), pat2.SymbolWeight() );
    default: return -1;
    }
  case ObjectType::List:{
    switch ( pat2.type()){
    case ObjectType::Number: case ObjectType::String: return 1;
    case ObjectType::Symbol: return 1;
    case ObjectType::List:{
      // Pattern and kind of thins need to be considered
      int w1,w2;
      w1 = __patternType(pat1); w2 =  __patternType(pat2);
      //cout<<w1<<","<<w2<<endl;
      if ( w1 ){//pat1 is a pattern
        if ( w2 ){//both are pattern
          return_if_not( w1-w2, 0);
        }else{//pat2 is not a pattern
          return 1;
        }
      }else{// pat1 is not a pattern
        if ( w2 ){ // only pat2 is a pattern
          return -1;
        }else{//both are not pattern
          return_if_not( compare( pat1.Head(), pat2.Head() ), 0 );
        }
      }
      auto iter1 = pat1.ElementsBegin();
      auto iter2 = pat2.ElementsBegin();
      // non numbers
      while ( iter1 != pat1.End() && iter2 != pat2.End() ){
        if ( (*iter1).NullQ() ) return 1;
        if ( (*iter2).NullQ() ) return -1;
        return_if_not( PatternCompare(*iter1,*iter2),0 );
        iter1++; iter2++;
      }
      if ( iter1 != pat1.End() ) return 1;
      if ( iter2 != pat2.End() ) return -1;
      return 0;
      //end of default
    }
    default: return -1;
    }
  }
  }//end of switch outermost
  return 0;
}

void Pattern::UnifiedApplyPatternPairs(Object &Argv,vector<MatchRecord>&pairs){
  if ( pairs.size() <= 1 ) return;
  // replace from back to head can avoid create extra space to store pairs when
  //   apply on the original Obj;
  if ( Argv.SymbolQ( SYMID_OF___Variable ) ){
    unsigned int p = Argv.re();
    if ( p > 0 && p < pairs.size() ){
      if ( pairs[p].n == 0 ){
        Argv.SetList( SYMID_OF_Sequence );
        return;
      }else if ( pairs[p].n == 1 ){
        Argv.CopyObject( Object(pairs[p].idx) );
        return;
      }else{
        Argv.SetList( SYMID_OF_Sequence );
        for ( u_int i = 0; i< pairs[p].n; i++){
          vector<Object>&tlist = GlobalPool.Lists.Get( pairs[p].idx );
          Argv.PushBackCopy( tlist[ pairs[p].p+i ] );
        }
        return;
      }
    }else{
      Erroring("Pattern","Pattern unify is not right.");
      return;
      //return;
    }
  }
  if ( not Argv.ListQ() ) return;
  for (Object::iterator iter =Argv.ElementsBegin(); iter != Argv.ElementsEnd(); iter++){
    UnifiedApplyPatternPairs( *iter,pairs);
  }
  return;
}

int Pattern::UnifiedRawReplaceAll(Object &Argv, Object &rule,bool repeated){
  if ( rule.Size() < 2 ) {
    Erroring("UnifiedRawReplaceAll","Rule is not in the required form.");
    return false;
  }
  Index idx = GlobalPool.Matches.New();
  vector<MatchRecord> &pairs = GlobalPool.Matches.Get(idx);
  //vector<MatchRecord> pairs;// matchqq will construct a pair Argv
  // pairs.reserve(rule[1].code() + 1 );
  if ( ({u_int pair_size=0;UNIFY_MATCHQ( Argv, rule[1] ,pairs,pair_size,0,0,false);}) ){ // normal Matched
    Object nObj = rule[2].Copy();
    UnifiedApplyPatternPairs( nObj, pairs );
    GlobalPool.Matches.FreePairs(idx);
    Argv = nObj;
    return 1;
  }
  if ( Argv.AtomQ() ){
    GlobalPool.Matches.FreePairs(idx);
    return false;
  }
  bool replaced = false;
  for ( Object::iterator iter=Argv.ElementsBegin(); iter!= Argv.ElementsEnd(); iter++){
    if ( UnifiedRawReplaceAll(*iter,rule,repeated) ) {
      replaced = true;
      if ( !repeated )
        return true;
    }
  }
  return replaced;
}

int Pattern::UnifiedReplaceAll(Object &Argv, Object &rule,bool repeated){
  if ( rule.ListQ( SYMID_OF_HoldPattern ) && (rule).Size() == 1){
    return UnifiedReplaceAll(Argv,(rule)[1],repeated);
  }
  if ( !RuleQ(rule) ){
    if ( repeated ) {
      zhErroring("标准化全部替换",rule.ToString()+"不是规则")||
        Erroring("UnifiedReplaceAll",rule.ToString()+" is not a rule.");
      return false;
    }else{
      zhErroring("标准化替换",rule.ToString()+"不是规则")||
        Erroring("UnifiedReplace",rule.ToString()+" is not a rule.");
      return false;
    }
  }
  return UnifiedRawReplaceAll(Argv,rule,repeated);
}


bool Pattern::MatchQ(const Object &Argv,const Object &pattern){
  Object temp_pat;
  temp_pat.CopyObject( pattern );
  UnifyRuleLeft( temp_pat );
  return UnifiedMatchQ(  Argv, temp_pat );
}

int ApplyPatternPairs(Object &Argv,map<Object ,Object >&pairs){
  if ( Argv.AtomQ() ){
    map<Object ,Object >::iterator iter = pairs.find(Argv);
    if ( iter!=pairs.end() )
      Argv=iter->second;
    return 0;
  }
  for (Object::iterator iter =Argv.ElementsBegin(); iter != Argv.ElementsEnd(); iter++){
    ApplyPatternPairs( *iter,pairs);
  }
  return 0;
}

// int Pattern::ReplaceAll(Object &Argv, Object &rule,bool repeated){
//   if ( rule.ListQ(SYMID_OF_HoldPattern) && (rule).Size() == 1){
//     return ReplaceAll(Argv,rule[1],repeated);
//   }
//   if ( !RuleQ(rule) ){
//     if ( repeated ){
//         Erroring("ReplaceAll",rule.ToString()+" is not a rule.");
//         return false;
//     }else {
//       Erroring("Replace",rule.ToString()+" is not a rule.");
//       return false;
//     }
//   }
//   UnifyRule( rule[1],rule[2] );
//   //dprintf("ReplaceAll with unified rule: %s", rule.ToString().c_str() );
//   return UnifiedRawReplaceAll(Argv,rule,repeated);
// }

bool Pattern::RuleQ(const Object &Argv){
  if ( Argv.PairQ( SYMID_OF_Rule ) or
       Argv.PairQ( SYMID_OF_RuleDelayed ) or
       Argv.PairQ( SYMID_OF_KeyValuePair ) )
    return true;
  return false;
}

bool Pattern::PatternListQ(const Object &list){
  switch( list.type() ){
  case ObjectType::Number: case ObjectType::String: case ObjectType::Symbol: return false;
  case ObjectType::List:
    if ( list(0).SymbolQ() ){
      if ( list(0).ids() == SYMID_OF_Pattern ||
           list(0).ids() == SYMID_OF_Black ||
           list(0).ids() == SYMID_OF_BlackSequence ||
           list(0).ids() == SYMID_OF_BlackNullSequence ) 
        return true;
    }
    for (auto iter = list.ElementsBegin();iter!= list.ElementsEnd();iter++){
      if ( PatternListQ(*iter) )
        return true;
    }
    return false;
  }
  return false;
}

