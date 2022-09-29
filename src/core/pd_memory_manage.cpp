#include<pd_headers.h>

using namespace std;
using namespace pond;

EvaMemoryPool::EvaMemoryPool(){
#define _DS2(sym,pL,pR) if ( SYMID_OF_##sym != Symbols.GetOrNew(#sym,pL,pR) ){ \
    _Erroring("MemoryPool",#sym " has differnet symbol id when defined in macro"); \
  }
  //////////////////////////////////////

  _DS2(Null,                   0 ,0 );
  _DS2(True,                   0,0);       
  _DS2(False,                  0,0);       
  _DS2(Slot,                   35,34);                   
  // _DS2(Compound,               0,0);               
  _DS2(ArgsFollow,             37,0);              
  _DS2(PartArgsFollow,         37,0);          
  _DS2(ParenthesisFollow,      37,0);       
  /////////////////// 30 is the smallest level
  _DS2(Prefix,                 48,44);     
  _DS2(PrefixIn,               48,44);     
  ///////////////////        
  _DS2(Black,                  45,46);     
  _DS2(BlackSequence,          45,46);     
  _DS2(BlackNullSequence,      45,46);     
  _DS2(Pattern,                47,48);     
  ///////////////////
  ///////////////////
  _DS2(Get,                    50,51);     
  ///////////////////
  _DS2(Increment,              55,0);      
  _DS2(PreIncrement,           0,55);      
  _DS2(Decrement,              55,0);      
  _DS2(PreDecrement,           0,55);      
  ///////////////////
  _DS2(Factorial,              60,0);      
  _DS2(Power,                  70,69);     
  ///////////////////
  _DS2(Reciprocal,             0,80);      
  ///////////////////////////////////////////////////////
  _DS2(Times,                  100,100);   
  _DS2(Divide,                 100,99);    
  _DS2(Mod,                    100,99);    
  _DS2(Dot,                    100,99);    
  /////////////////////////////////////////////////////// range 110-180 are arithematic range
  _DS2(Plus,                   110,110);  
  _DS2(Subtract,               110,109); 
  /////////////////// differ from c++
  _DS2(Greater,                130,129);   
  _DS2(Less,                   130,129);   
  _DS2(GreaterEqual,           130,129);   
  _DS2(LessEqual,              130,129);   
  _DS2(Equal,                  130,129);   
  _DS2(UnEqual,                130,130);   
  ///////////////////
  _DS2(Not,                    0,168);     
  _DS2(And,                    170,169);   
  _DS2(Or,                     175,174);   
  ///////////////////////////////////////////////////////
  _DS2(Alternatives,           181,181);   
  ///////////////////
  ///////////////////
  _DS2(PureFunction,           190,0);//   
  _DS2(Rule,                   200,199);   
  _DS2(RuleDelayed,            200,199);   
  ////////////////////////////
  _DS2(ReplaceAll,             205,204);   
  _DS2(ReplaceAllRepeated,     205,204);   
  ///////////////////
  _DS2(Postfix,                222,48);    
  ///////////////////
  _DS2(Set,                    48,210);    
  _DS2(SetDelayed,             48,210);    
  _DS2(AddTo,                  48,210);    
  _DS2(TimesBy,                48,210);    
  _DS2(SubtractFrom,           48,210);    
  _DS2(DivideBy,               48,210);    
  _DS2(PowerWith,              48,210);    
  _DS2(ModBy,                  48,210);    
  ///////////////////
  _DS2(Put,                    220,219);   
  ///////////////////
  _DS2(CompoundExpression,     230,230);   
  ///////////////////
  _DS2(Seperator,              240,240);   
  /////////////No symbols but used frequen
  _DS2(Span,                   176,176);   
  _DS2(Help,                   50,0);      
  _DS2(Conjunct,               44,42);     
  _DS2(Apply,                  48,206);    
  _DS2(Map,                    48,206);    
  _DS2(__Variable,             0,0);       
  _DS2(SerialCode,             0,0);       
  _DS2(FunctionVariable,       0,0);       
  _DS2(KeyValuePair,           200,199);   
  ////////////////////////////////////////////
#define _DS4(sym,pL,pR,oT,oN) if ( SYMID_OF_##sym != Symbols.GetOrNew(#sym,pL,pR,oT,oN) ){ _Erroring("MemoryPool",#sym " has differnet symbol id then defined in macro"); }
  _DS4(if,                     0,220,   1,2); 
  _DS4(else,                   218,219, 3,1);
  _DS4(elif,                   218,219, 3,2);
  _DS4(while,                  0,220,   1,2);
  _DS4(for,                    0,220,   1,2);
  _DS4(do,                     0,220,   1,2);
  _DS4(switch,                 0,220,   1,2);
  _DS4(case,                   0,220,   1,1);
  _DS4(import,                 0,220,   1,1);
  _DS4(from,                   220,219, 3,1);
  _DS4(and,                    170,169, 3,1);
  _DS4(or,                     175,174, 3,1);
  _DS4(not,                    0,168,   1,1);
  _DS4(in,                     120,115, 3,1);
  _DS4(var,                    0,220,   1,1);
  _DS4(global,                 0,220,   1,1);
  _DS4(continue,               0,0,     1,0);
  _DS4(break,                  0,0,     1,0);
  _DS4(return,                 0,220,   1,1);
  _DS4(function,               0,220,   1,2);
  _DS4(class,                  0,220,   1,2);
  _DS4(new,                    0,160,   1,1);
  _DS4(del,                    0,160,   1,1);
  _DS4(extends,                220,219, 3,1);
#define _DS(sym) if ( SYMID_OF_##sym != Symbols.GetOrNew(#sym) ){ _Erroring("MemoryPool",#sym " has differnet symbol id then defined in macro"); }
  _DS(Part                     );
  _DS(Parenthesis              );
  _DS(Complex                  );
  _DS($START$                  );
  _DS($END$                    );
  _DS(HoldPattern              );
  _DS(Hold                     );
  _DS(HoldOnce                 );
  _DS(Evaluate                 );
  _DS(Exit                     );
  _DS(List                     );   
  _DS(Dict                     );   
  _DS2(Sequence,               215,215);
  _DS(Undefined                );
  _DS(Unknown                  );
  _DS(default                  ); // default just a symbol             
  _DS(ExpressionList           );
  _DS2(Unpack,                 0,55 );
  _DS2(UnpackDict,             0,55 );
  _DS4(Query,                  47,195,  0,0);
  _DS(LineBreak                );       // a special one
  _DS4(ArrowFunction,          48,210,  3,1);
  _DS4(as,                     170,170, 3,1);
  /////////////////////////////////////////////////////////


  // smaller than compond expression and set and 220 but larger than 
  // var a = 3, b = 4;    > set(210)   < sfunction(220)   < compond(230)
  // _DS2( SequenceComma,  215,215); 
#undef _DS4
#undef _DS2
#undef _DS

  // is_finalizing = false;

  srand( (int)time(0) );
  //dout<<"sym id of rule " << SYMID_OF_Rule << endl;

  //EvaSettings.calculatePosition = CalculatePositionHost;
  EvaSettings.DataPosition = DataHost;
  EvaSettings.ParallelMode = ParallelModeGpu;
  EvaSettings.threadNumberPerBlock = 256;
  EvaSettings.threadNumberPerKernel = 1;

  EvaSettings.precision = 8;
  EvaSettings.scientificFormat = false;
  EvaSettings.epsilon = 0.000000000000005;

  flag = 0;
}

EvaMemoryPool::~EvaMemoryPool(){
  //dout<<endl;
  //dout<<"Finalilze EvaMemoryPool"<<endl;
  flag = 1;
}


__EvaSymbolTable::__EvaSymbolTable(){
  //GetOrNew("Null",0,0);
  ptr = 0;
}

__EvaSymbolTable::~__EvaSymbolTable(){
  //dout<<"   finalize SymbolTable"<<endl;
  // for ( auto iter = records.begin(); iter != records.end(); iter++){
  //   delete (*iter).key;
  // }
  //dout<<"   finalize SymbolTable finished"<<endl;
}

u_int __EvaSymbolTable::GetOrNew(const char*key,u_char priLeft,u_char priRight,u_char operType,u_char operNum){
  //if ( mt.try_lock_for( std::chrono::milliseconds( __EvaMaxWaitTime ) ) ){
  // std::lock_guard<std::mutex> lock(mt);
  int first=0,final = positions.size()-1, result=-999;
  u_int pos= final+1;
  while ( first <= final ){
    pos = first+(final-first)/2;
    result = compare( key, positions[pos].key );
    if ( result == 0 ){
      if ( priLeft != 0 or priRight != 0 ){
        records[ positions[pos].pos ].pL    = priLeft;
        records[ positions[pos].pos ].pR    = priRight;
        records[ positions[pos].pos ].oType = operType;
        records[ positions[pos].pos ].oNum  = operNum;
      }
      //unLock();
      return positions[pos].pos;
    }else if ( result < 0 ){
      final = pos-1;
    }else{// result > 0
      first = pos+1;
    }
  }
  auto iter_pos = positions.begin() + pos;
  if ( pos < positions.size() and result > 0 ){
    iter_pos ++;
    pos ++;
  }
  recordType rec( strdup( key), pos, priLeft, priRight, operType, operNum );
  iter_pos = positions.insert( iter_pos, positionType( rec.key, ptr) );
  records.push_back(rec);
  //update weight
  iter_pos++;
  while ( iter_pos != positions.end() ){
    records[ (*iter_pos).pos ].weight++;
    iter_pos ++;
  }
  // }else{
  //   ThrowError("Pool","SymbolTable","Cannot unlock time_mutex within "+ToString(__EvaMaxWaitTime)+"ms");
  // }
  return (ptr++);
}

#define declare_ref(obj,id) auto &obj= objs[id.row ][id.col ]
//#define declare_ref(obj,id) auto &obj= (objs.at(id.row ))[id.col ]

Index __EvaStringTable::NewString(const char*str){
  Index id;
  id = __EvaTable::New();
  objs[id.row][id.col ] = str;
  return id;
}

Index __EvaStringTable::NewString(Index id){
  Index newid;
  newid = __EvaTable::New();
  objs[newid.row ][newid.col ] = objs[id.row ][id.col ]; 
  return newid;
}

Index __EvaListTable::NewList(Index id){
  Index newid = New();
  declare_ref(newlist,newid);
  declare_ref(list,id);
  newlist.resize( list.size() );
  for ( auto iter = newlist.begin(),riter = list.begin() ; iter != newlist.end(); iter++ ,riter++){
    (*iter).CopyObject(*riter);
  }
  return newid;
}

void __EvaListTable::FreeList(Index id){
  //cout<<"Free Object of List type: "<<id<<endl;
  declare_ref(obj,id);
  // for (auto iter=obj.begin(); iter != obj.end(); iter++){
  //   (*iter).SetVoid();
  // }
  //dout<<"    free list "<<id<<" size = "<<obj.size()<<" before buffer="<<freeObjs.size()<<endl;
  obj.clear();
  if ( obj.capacity() > 128 ){
    obj.shrink_to_fit();
  }

  freeObjs.push_back(id);
  //dout<<"     afterr buffer size="<<freeObjs.size() <<endl;
}

void __EvaListTable::SetList(Index left, Index right){
  declare_ref(lobj,left);
  declare_ref(robj,right);
  if ( lobj.size() > robj.size() ){
    lobj.erase( lobj.begin()+robj.size(),lobj.end() );
  }else{
    lobj.resize( robj.size() );
  }
  for ( auto iter = lobj.begin(),riter = robj.begin(); iter!= lobj.end(); iter++,riter++){
    (*iter).CopyObject( *riter );
  }
  return;
}

void __EvaMatchTable::FreePairs(Index id){
  declare_ref(obj,id);
  obj.clear();
  if ( obj.capacity() > 64 ) obj.resize(16);
  //waitAndLock(); //should check 
  //if ( mt.try_lock_for( std::chrono::milliseconds( __EvaMaxWaitTime ) ) ){
  // std::lock_guard<std::mutex> lock(mt);
  freeObjs.push_back(id);
  // }else{
  //   ThrowError("MatchTable","FreePairs","Cannot unlock time_mutex within "+ToString(__EvaMaxWaitTime)+"ms");
  // }
  //unLock();
}

#undef declare_ref

