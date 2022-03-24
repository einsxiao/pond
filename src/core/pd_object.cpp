
#include<pd_headers.h>

using namespace pond;
using namespace std;

#define _obj  GlobalPool.Objects.objs[ objid.row ][ objid.col ]
#define _sym  GlobalPool.Symbols.GetKey( ids() )
#define _str  GlobalPool.Strings.Get( idx() )
#define _list GlobalPool.Lists.Get( idx() )

#define __check(t,ret) if ( voidQ()  ) {                                \
    Erroring("Object",(string)"Function '"+__FUNCTION__+"' is only applied to none Null Object."); \
    return ret;                                                         \
  }                                                                     \
  if ( type() != ObjectType::t ) {                                      \
    Erroring("Object",(string)"Function '"+__FUNCTION__+"' is only appled to Object with type "+ \
             Type2String(ObjectType::t)+", while current object is a "+TypeString()+"."); \
    return ret;                                                         \
  }
#define __check_not(t,ret)                                              \
  if ( voidQ() ) {                                                      \
    Erroring("Object",(string)"Function '"+__FUNCTION__+"' is only applied to a none Null Object."); \
    return ret;                                                         \
  }                                                                     \
  if ( type() == ObjectType::t ){                                       \
    Erroring("Object",(string)"Function '"+__FUNCTION__+"' can not be appled to Object with type "+ \
             Type2String(ObjectType::t)+".");                           \
    return ret;                                                         \
  }
#define __check_not_null(ret)                                           \
  if ( voidQ() ) {                                              \
    Erroring("Object",(string)"Function '"+__FUNCTION__+"' is only applied to a none Null Object."); \
    return ret;                                                         \
  }

// #define __check_state_uncomplete(ret)                                   \
//   if ( _list.size() == 0 ) {                                            \
//     Erroring("Object","Uncomplete List.");                              \
//     return ret;                                                         \
// }

#undef __check
#undef __check_not
#undef __check_not_null
#define __check(t,ret) 
#define __check_not(t,ret) 
#define __check_not_null(ret) 
// #undef __check_state_blocked
// #undef __check_state_uncomplete
// #define __check_state_uncomplete(ret)

static int specialCharReplacement(string &str){
  specialCharReplace(str,"\"","$QUOTATION_MARK$");
  specialCharReplace(str,"$QUOTATION_MARK$","\\\"");
  return 0;
}

void Object::Malloc(){
  if ( voidQ() ){
    objid =  GlobalPool.Objects.New();
    _obj.init();
  }
};
void Object::Clear(){
  if ( voidQ() ) return; 
  switch ( type() ){                             
  case ObjectType::Number: break;                                      
  case ObjectType::Symbol: break;                                      
  case ObjectType::String: GlobalPool.Strings.Free( idx() ); break;  
  case ObjectType::List:   GlobalPool.Lists.FreeList( idx() ); break;  
  }
  _obj.clear();
};
void Object::Free(){
  if ( voidQ() ) return; 
  if ( cnt() == 0 ){
    switch ( type() ){                             
    case ObjectType::Number: break;                                      
    case ObjectType::Symbol: break;                                      
    case ObjectType::String: GlobalPool.Strings.Free( idx() ); break;  
    case ObjectType::List:   GlobalPool.Lists.FreeList( idx() ); break;  
    }
    //cout<<"Free Object on globalPool with flag = "<<GlobalPool.flag<<endl;
    GlobalPool.Objects.Free( objid );
  }else{
    cnt_decr();
  }
  objid.zero();
  return;
};


Object &Object::operator=(const Object&obj){
  if ( obj.voidQ() ){
    if ( nonvoidQ() ) Free();
    return *this;
  }
  (const_cast<Object&>(obj)).cnt_incr();
  if ( nonvoidQ() ){
    Index tmpid = obj.objid ;
    Free();
    objid = tmpid;
  }else{
    objid = obj.objid;
  }
  return *this;
};

/*
  this is a core function of the whole system. And it is complex too. Some part need special care.
  1. when Assignment of case List->Symbol or String->Symbol we must zero the value of _idx, otherwize it will become a referenced symbol by its remainning value. However, it is not.

 */

// const unsigned char Object_State_Uncomplete = 14;
Object& Object::CopyObject(const Object&other,bool keepRef){ 
  if ( other.NullQ() ){
    Free();
    return *this;
  }
  if ( voidQ() ){
    Malloc();
    _obj = GlobalPool.Objects.Get(other.objid);
    switch ( other.type() ){
    case ObjectType::Number: 
      return *this;
    case ObjectType::Symbol:
      if ( keepRef ) return *this;
      if ( _obj.ids != SYMID_OF___Variable and
           _obj.ids != SYMID_OF_FunctionVariable and
           _obj.ids != SYMID_OF_SerialCode ) 
        _obj.val._idx.zero(); // ValuedSymbol info will be erased
      return *this;
    case ObjectType::String:
      set_idx( GlobalPool.Strings.New() );
      _str = GlobalPool.Strings.Get( other.idx() );
      return *this;
    case ObjectType::List:
      // (const_cast<Object&>(other)).set_state( Object_State_Uncomplete );
      set_idx( GlobalPool.Lists.New() );
      GlobalPool.Lists.SetList( idx(), other.idx() );
      // (const_cast<Object&>(other)).set_state( 0 );
      return *this;
    }
    return *this;
  }
  switch ( other.type() ){

  case ObjectType::Number: //both simple type, no need to set type or other info
    Clear();
    _obj = GlobalPool.Objects.Get( other.objid );
    return *this;
  case ObjectType::Symbol: //both simple type, no need to set type or other info
    Clear();
    _obj = GlobalPool.Objects.Get( other.objid );
    if ( keepRef ) return *this;
    if ( _obj.ids != SYMID_OF___Variable and
         _obj.ids != SYMID_OF_FunctionVariable and
         _obj.ids != SYMID_OF_SerialCode ) 
      _obj.val._idx.zero();  // ValuedSymbol info will be erased
    return *this;
  case ObjectType::String://other is complex String, so need this to deal mem
    switch ( type() ){
    case ObjectType::Number: case ObjectType::Symbol: // this is simple, only need malloc string 
      _obj = GlobalPool.Objects.Get( other.objid );
      set_idx( GlobalPool.Strings.NewString( other.idx() ) ); 
      return *this;
    case ObjectType::String: //both string, only need to update string value
      _str = GlobalPool.Strings.Get( other.idx() );
      return *this;
    case ObjectType::List:// this is List,need 1. free ori 2. change type 3. update value
      GlobalPool.Lists.FreeList( idx() );
      _obj = GlobalPool.Objects.Get( other.objid );
      set_idx( GlobalPool.Strings.NewString( other.idx() ) );
      return *this;
    }
  case ObjectType::List: // other is complex List,need more complex operation than string
    // (const_cast<Object&>(other)).set_state( Object_State_Uncomplete );
    switch ( type() ){
    case ObjectType::Number: case ObjectType::Symbol: // this is simple, just malloc new
      _obj = GlobalPool.Objects.Get( other.objid );
      set_idx( GlobalPool.Lists.New() );
      GlobalPool.Lists.SetList( idx(), other.idx() );
      // (const_cast<Object&>(other)).set_state( 0 );
      return *this;
    case ObjectType::String: // this is string, free old, malloc new
      GlobalPool.Strings.Free(idx());
      set_idx( GlobalPool.Lists.New() );
      GlobalPool.Lists.SetList( idx(), other.idx() );
      // (const_cast<Object&>(other)).set_state( 0 );
      return *this;
    case ObjectType::List: // both list, update elements is enough
      // set_idx( GlobalPool.Lists.New() ); // keep idx, otherwize will cause memory leak
      GlobalPool.Lists.SetList( idx(), other.idx() );
      // (const_cast<Object&>(other)).set_state( 0 );
      return *this;
    }
  }
  return *this;
}

Object& Object::SetNumber(const double num){
  if ( voidQ() ){
    Malloc();
    set_type( ObjectType::Number );
    set_re( num);
    return *this;
  }
  switch ( type() ){
  case ObjectType::Symbol: set_ids( 0 ); break;
  case ObjectType::Number: set_re( num); return *this;
  case ObjectType::String: GlobalPool.Strings.Free(idx()); break;
  case ObjectType::List: GlobalPool.Lists.FreeList(idx()); break;
  }
  set_type( ObjectType::Number );
  set_re( num);
  return *this;
}

// Object& Object::SetComplex(const complex num){
//   SetList( SYMID_OF_Complex );
//   PushBackNumber( num.re );
//   PushBackNumber( num.im );
//   return *this;
// }

Object& Object::SetNumber(const complex num){
  if ( num.realq() ){
    SetNumber( num.re );
    return *this;
  }
  SetList( SYMID_OF_Complex );
  PushBackNumber( num.re );
  PushBackNumber( num.im );
  return *this;
}

Object& Object::SetString(const char*str){
  if ( voidQ() ){
    Malloc();
    set_type( ObjectType::String );
    set_idx(GlobalPool.Strings.NewString(str) );
    return *this;
  }
  switch ( type() ){
  case ObjectType::Symbol: set_ids( 0); break;
  case ObjectType::Number: break;
  case ObjectType::String: _str = str; return *this;
  case ObjectType::List: GlobalPool.Lists.FreeList(idx()); break;
  }
  set_type( ObjectType::String );
  set_idx(GlobalPool.Strings.NewString(str) );
  return *this;
}

Object& Object::SetString(const string &str){
  return SetString( str.c_str() );
}

Object& Object::SetSymbol(const u_int id){
  if ( id >= GlobalPool.Symbols.TableSize() ) {
    Erroring("Object","Try to SetSymbol with Symbol Id "+pond::ToString(id)+" that does not exist."); 
    return *this; 
  }
  if ( voidQ() ){
    Malloc();
    set_type( ObjectType::Symbol );
    set_ids( id );
    return *this;
  }
  switch ( type() ){
  case ObjectType::Symbol:
    set_ids( id );
    zero_idx();
    return *this;
  case ObjectType::Number:
    break;
  case ObjectType::String:
    GlobalPool.Strings.Free(idx()); break;
  case ObjectType::List:
    GlobalPool.Lists.FreeList(idx()); break;
  }
  set_type( ObjectType::Symbol );
  set_ids( id );
  zero_idx();
  return *this;
}

Object& Object::SetBoolean(const bool val){
  u_int id = val?SYMID_OF_True:SYMID_OF_False;
  if ( voidQ() ){
    Malloc();
    set_type( ObjectType::Symbol );
    set_ids( id );
    return *this;
  }
  switch ( type() ){
  case ObjectType::Symbol:
    set_ids( id );
    zero_idx();
    return *this;
  case ObjectType::Number:
    break;
  case ObjectType::String:
    GlobalPool.Strings.Free(idx()); break;
  case ObjectType::List:
    GlobalPool.Lists.FreeList(idx()); break;
  }
  set_type( ObjectType::Symbol );
  set_ids( id );
  zero_idx();
  return *this;
}

Object& Object::SetSymbol(const char*str){
  u_int id =  GlobalPool.Symbols.GetOrNew( str );
  SetSymbol( id );
  return *this;
}

Object& Object::ReserveSize(const u_int len){
  __check(List,*this);
  _list.reserve(len+16);
  return *this;
}

Object& Object::SetList(const u_int id){// id is the sym id of its symbol head
  if ( voidQ() ){
    Malloc();
    set_type( ObjectType::List );
    set_idx( GlobalPool.Lists.New() );
    Object x;
    x.SetSymbol( id );
    _list.push_back( x );
    return *this;
  }
  switch ( type() ){
  case ObjectType::Symbol: set_ids( 0 ); break;
  case ObjectType::Number: break;
  case ObjectType::String: GlobalPool.Strings.Free(idx()); break;
  case ObjectType::List:{
    vector< Object > &li = GlobalPool.Lists.Get( idx() );
    li.erase( li.begin() +1, li.end() );
    li[0].SetSymbol(id);
    return *this;
  }
  }
  set_type( ObjectType::List );
  set_idx( GlobalPool.Lists.New() );
  Object x;
  x.SetSymbol( id );
  _list.push_back( x );
  return *this;
};

Object& Object::SetList(const char*str){
  u_int id = GlobalPool.Symbols.GetOrNew(str);
  SetList( id );
  return *this;
}

Object& Object::SetPairCopy(const Object&first,const Object&second,u_int headSymId){
  SetList( headSymId );
  PushBackCopy(first);
  PushBackCopy(second);
  return *this;
}

Object& Object::SetPairRef(const Object&first,const Object&second,u_int headSymId){
  SetList( headSymId );
  PushBackRef(first);
  PushBackRef(second);
  return *this;
}

string Object::ToRawString()const{
  string res;
  res= pond::ToString(objid);
  if ( nonvoidQ() ){
    res += "{type="+TypeString()
      +",state="+pond::ToString( state() )
      // +",code="+pond::ToString( code() )
      +",cnt="+pond::ToString( cnt() )
      +",ids="+pond::ToString( ids() )+
      +",v={"+pond::ToString(idx())+","
      +pond::ToString(re())+"}}";
  }
  return res;
}

string Object::TypeString()const{
  switch (type()){
  case ObjectType::Number: return "Number";
  case ObjectType::Symbol: return "Symbol";
  case ObjectType::String: return "String";
  case ObjectType::List:   return "List"; 
  }
  Erroring("Object::TypeString","switch");
  return "";
}
///////////////////////////////////////////////
bool Object::NullQ()const{
  if ( voidQ() )
    return true;
  if ( type() == ObjectType::Symbol and ids() == 0 )
    return true;
  if ( ( type() == ObjectType::String or type() == ObjectType::List ) and idx().zeroQ() )
    return true;
  return false;
}

// bool Object::NullSymbolQ()const{
//   if ( voidQ() )
//     return false;
//   if ( type() == ObjectType::Symbol and ids() == 0 )
//     return true;
//   return false;
// }

// bool Object::VeryNullQ()const{
//   return voidQ() ;
// }

bool Object::AtomQ()const{
  if ( voidQ() ) return true;
  if ( type() == ObjectType::List )
    return false;
  return true;
}

bool Object::TypeQ(ObjectType t)const{
  if ( voidQ() ) return false;
  if ( type() == t ) return true;
  return false;
}

///////////////////////////////////////////////
bool Object::SymbolQ()const{
  if ( voidQ() ) return false;
  if ( type() == ObjectType::Symbol ) return true;
  return false;
}

bool Object::SymbolQ(const char*str)const{
  if ( voidQ() ) return false;
  if ( type() == ObjectType::Symbol ){
    if ( compare( _sym, str) == 0 )
      return true;
  }
  return false;
}

bool Object::SymbolQ(const u_int id)const{
  if ( voidQ() ) return false;
  if ( type() == ObjectType::Symbol ){
    if (  ids() == id )
      return true;
  }
  return false;
}

bool Object::ValuedSymbolQ()const{
  if ( voidQ() ) return false;
  if ( type() == ObjectType::Symbol ){
    if (  ( idx().nonzeroQ() )
          and ids() != SYMID_OF___Variable
          and ids() != SYMID_OF_FunctionVariable
          and ids() != SYMID_OF_SerialCode )
      return true;
  }
  return false;
}

bool Object::NumberQ()const{
  if ( voidQ() ) return false;
  switch ( type() ){
  case ObjectType::Number:
    return true;
  case ObjectType::String: case ObjectType::Symbol:
    return false;
  case ObjectType::List:
    if ( _list[0].SymbolQ( SYMID_OF_Complex ) )
      return true;
    return false;
  }
  return false;
}

bool Object::NumberQ(const double nu)const{
  if ( voidQ() ) return false;
  switch ( type() ){
  case ObjectType::Number:
    return re() == nu;
  case ObjectType::String: case ObjectType::Symbol:
    return false;
  case ObjectType::List:
    return false;
  }
  return false;
}

bool Object::RealQ()const{
  if ( voidQ() ) return false;
  switch ( type() ){
  case ObjectType::Number:
    return true;
  case ObjectType::String: case ObjectType::Symbol: case ObjectType::List:
    return false;
  }
  return false;
}

bool Object::RealQ(const double nu)const{
  if ( voidQ() ) return false;
  switch ( type() ){
  case ObjectType::Number:
    return re() == nu;
  case ObjectType::String: case ObjectType::Symbol: case ObjectType::List:
    return false;
  }
  return false;
}

bool Object::NumberQ(const complex nu)const{
  if ( voidQ() ) return false;
  switch ( type() ){
  case ObjectType::Number:
    return re() == nu.re and nu.im == 0;
  case ObjectType::String: case ObjectType::Symbol:
    return false;
  case ObjectType::List:
    if ( _list[0].SymbolQ( SYMID_OF_Complex ) ){
      return _list[1].NumberQ( nu.re ) and _list[2].NumberQ( nu.im );
    }
    return false;
  }
  return false;
}

bool Object::ComplexQ()const{
  if ( voidQ() ) return false;
  switch ( type() ){
  case ObjectType::Number: case ObjectType::String: case ObjectType::Symbol:
    return false;
  case ObjectType::List:
    if ( _list[0].SymbolQ( SYMID_OF_Complex ) ){
      return true;
    }
    return false;
  }
  return false;
}

bool Object::ComplexQ(const complex nu)const{
  if ( voidQ() ) return false;
  switch ( type() ){
  case ObjectType::Number:
    return re() == nu.re and nu.im == 0;
  case ObjectType::String: case ObjectType::Symbol:
    return false;
  case ObjectType::List:
    if ( _list[0].SymbolQ( SYMID_OF_Complex ) ){
      return _list[1].NumberQ( nu.re ) and _list[2].NumberQ( nu.im );
    }
    return false;
  }
  return false;
}

bool Object::IntegerQ()const{
  if ( voidQ() ) return false;
  switch ( type() ){
  case ObjectType::Number:
    return Math::IntegerQ( re() );
  case ObjectType::String: case ObjectType::Symbol:
    return false;
  case ObjectType::List:
    return false;
  }
  return false;
}

bool Object::StringQ()const{
  if ( voidQ() ) return false;
  if ( type() != ObjectType::String ) return false;
  return true;
}

bool Object::StringQ(const char*key)const{
  if ( voidQ() ) return false;
  if ( type() != ObjectType::String ) return false;
  if ( compare(_str, key) != 0 ) return false;
  return true;
}

bool Object::StringQ(const Index id)const{
  if ( voidQ() ) return false;
  if ( type() != ObjectType::String ) return false;
  if ( compare(_str, GlobalPool.Strings.Get(id) ) != 0 ) return false;
  return true;
}

bool Object::ListQ()const{
  if ( voidQ() ) return false;
  if ( type() != ObjectType::List ) return false;
  return true;
}

bool Object::ListQ(const char* head)const{
  if ( voidQ() ) return false;
  if ( type() != ObjectType::List ) return false;
  if ( not _list[0].SymbolQ( head ) ) return false;
  return true;
}

bool Object::ListQ(u_int headSymId)const{
  if ( voidQ() ) return false;
  if ( type() != ObjectType::List ) return false;
  if ( not _list[0].SymbolQ( headSymId ) ) return false;
  return true;
}

bool Object::ListQ(const Object&obj)const{
  if ( voidQ() ) return false;
  if ( type() != ObjectType::List ) return false;
  if ( not (_list[0] == obj) ) return false;
  return true;
}

bool Object::RuleQ()const{
  if ( voidQ() ) return false;
  if ( type() != ObjectType::List ) return false;
  if ( _list.size() != 3 ) return false;
  if ( _list[0].SymbolQ( SYMID_OF_Rule ) )  return true;
  if ( _list[0].SymbolQ( SYMID_OF_RuleDelayed ) )  return true;
  return false;
}

bool Object::PairQ()const{
  if ( voidQ() ) return false;
  if ( type() != ObjectType::List ) return false;
  if ( _list.size() != 3 ) return false;
  return true;
}

bool Object::PairQ(const u_int ids)const{
  if ( voidQ() ) return false;
  if ( type() != ObjectType::List ) return false;
  if ( _list.size() != 3 ) return false;
  if ( _list[0].SymbolQ( ids ) ) return true;
  return false;
}

bool Object::PairQ(const Object&obj)const{
  if ( voidQ() ) return false;
  if ( type() != ObjectType::List ) return false;
  if ( _list.size() != 3 ) return false;
  if ( _list[0] == obj ) return true;
  return false;
}

bool Object::SimpleListQ()const{
  if ( voidQ() ) return false;
  if ( type() != ObjectType::List ) return false;
  if ( not _list[0].SymbolQ() ) return false;
  return true;
}

bool Object::SimpleListQ(const char* head)const{
  return ListQ(head);
}

bool Object::SimpleListQ(const u_int id)const{
  return ListQ(id);
}

bool Object::HeadQ(const char*head)const{
  return compare( head, Head() ) == 0 ;
}

bool Object::HeadQ(const Object&obj)const{
  __check(List,false);
  return SimpleCompare( obj , _list[0] ) == 0 ;
}

//////////////////////////////////////
const char *Object::Head()const{
  switch ( type() ){
  case ObjectType::Symbol: return "Symbol";
  case ObjectType::Number: return "Number";
  case ObjectType::String: return "String";
  case ObjectType::List:
    //dout<< _ElementAt(0).ToString() <<endl;
    if ( _list.size() == 0 ) { Erroring("Object","Uncomplete List."); return ""; }
    if ( _list[0].type() == ObjectType::Symbol ){
      return GlobalPool.Symbols.GetKey( _list[0].ids() );
    }
    return "$ComplicateList$";
  }{ Erroring("Object::Head","Switch error"); return ""; }
}

const char* Object::Key()const{
  __check_not_null("");
  switch (type()){
  case ObjectType::Symbol: return _sym;
  case ObjectType::String: return _str.c_str();
  case ObjectType::Number: { Erroring("Object","Try to get Key from Number"); return ""; }
  case ObjectType::List: return _list[0].Key();
  }{ Erroring("Object::Key","Switch error"); return ""; }
}

u_int Object::SymbolId()const{//return 0 if cannot get the id
  if ( voidQ() ) return 0;
  switch (type()){
  case ObjectType::Symbol: return ids();
  case ObjectType::List:{
    return _list[0].SymbolId();
  }
  case ObjectType::String: case ObjectType::Number:
    return 0;
  }
  return 0;
}

u_int Object::SymbolWeight()const{
  __check(Symbol,0);
  return GlobalPool.Symbols.GetWeight( ids() );
}

char Object::Boolean()const{
  if ( voidQ() ) return 0;
  switch ( type() ){
  case ObjectType::Number: if ( re() == 0 ) return 0; else return 1;
  case ObjectType::String:
    if ( idx().zeroQ() )
      return 0;
    if ( _str == "" ) return 0;
    return 1;
  case ObjectType::Symbol:
    if ( ids() == 0 ) return 0;
    if ( ids() == SYMID_OF_True ) return 1;
    if ( ids() == SYMID_OF_False ) return 0;
    return -1;
      
  case ObjectType::List: 
    if ( ( idx().zeroQ() ) ) return 0;
    if ( _list[0].type() == ObjectType::Symbol ){
      if (  _list[0].ids() == SYMID_OF_List  or 
            _list[0].ids() == pond::SYMID_OF_Dict ){
        if ( _list.size() > 1 )
          return 1;
        else
          return 0;
      }
      return -1;
    }
    return -1;
  }
  { Erroring("Object::Boolean","Switch Error"); return 0; }
}

double Object::Number()const{
  __check_not_null(0);
  switch ( type() ){
  case ObjectType::Number: return re();
  case ObjectType::String: case ObjectType::Symbol: break;
  case ObjectType::List: break;
  }
  Erroring("Object","Try to transfer a Non-Number Object to number.");
  return 0;
}

complex Object::Complex()const{
  __check_not_null(0);
  switch ( type() ){
  case ObjectType::Number: return re();
  case ObjectType::String: case ObjectType::Symbol: break;
  case ObjectType::List:
    if ( _list[0].SymbolQ( SYMID_OF_Complex ) ){
      return complex( (double) _list[1], (double) _list[2] );
    }
    break;
  }
  { Erroring("Object","Try to transfer a Non-Number Object to complex number."); return 0; }
}

const char* Object::String()const{
  __check(String,"");
  return _str.c_str();
}
////////////////////////////////////////

int Object::Dimensions(const Object&list,Object&dim,int n){//dim will be claimed by outer context
  if ( n == 0 ) return 1;
  if ( list.NullQ() ) return 1;
  if ( list.type() != ObjectType::List ) return 1;
  if ( dim.NullQ() )
    dim.SetList();
  dim.PushBackNumber( list.Size() );
  Object dim1;
  if ( Dimensions( list(1),dim1, n-1 )< 0 ){
    dim.SetVoid();
    return -1;
  }
  for( u_int i=2; i<= list.Size(); i++ ){
    Object dim2;
    if ( Dimensions( list(i), dim2,n-1)<0 or dim1 != dim2 ){
      dim.SetVoid();
      return -1;
    }
  }
  if ( not dim1.NullQ() ){
    dim.InsertRef( dim.End(),dim1.Begin(),dim1.End());
  }
  return 1;
}

int Object::ElementsSize()const{
  if ( type() != __List__ ) return -1;
  return _list.size();
}
int Object::Size()const{
  if ( type() != __List__ ) return -1;
  return _list.size()-1;
}

Object &Object::First(){
  if ( _list.size() < 2 ) {
    Erroring("Object","Try to get First element form empty list.");
    return const_cast<Object&>(NullObject);
  }
  return _list[1];
}

Object &Object::Last(){
  if ( _list.size() < 2 ) {
    Erroring("Object","Try to get Last element form an empty list.");
    return const_cast<Object&>(NullObject);
  }
  return _list.back();
}

Object &Object::Back(){
  if ( _list.size() < 2 ) {
    Erroring("Object","Try to get Last element form an empty list.");
    return const_cast<Object&>(NullObject);
  }
  return _list.back();
}

Object Object::RefObject(){
  if ( type() != ObjectType::Symbol ) {
    Erroring("Object","Try to get RefObject for none Symbol Object.");
    return NullObject;
  }
  if ( idx().zeroQ() ) {
    Erroring("Object","Try to get RefObject for Symbol without Ref Value.");
    return NullObject;
  }
  return Object( idx() ) ;
}
Object::iterator Object::Begin()const{
  return _list.begin()+1;
}

Object::iterator Object::End()const{
  return _list.end();
}

Object::iterator Object::ElementsBegin()const{
  return _list.begin();
}

Object::iterator Object::ElementsEnd()const{
  return _list.end();
}

u_int Object::PositionOfIterator(const iterator iter)const{
  return iter - _list.begin();
}

Object &Object::Swap(Object&obj){
  _Object temp;
  temp = _obj;
  _obj = GlobalPool.Objects.Get(obj.objid);
  GlobalPool.Objects.Get(obj.objid) = temp;
  return (*this);
}

Object &Object::SwapRef(Object&obj){
  Index tid = obj.objid;
  obj.objid = objid;
  objid = tid;
  return (*this);
}

Object &Object::Flatten(){
  __check(List,*this);
  u_int i = 1;
  while ( i<= Size() ){
    if ( ElementAt(i).ListQ( _list[0] ) ){
      InsertRef( Begin()+i, ElementAt(i).Begin(), ElementAt(i).End() );
      //ElementAt(i).DeleteNoFree();
      Delete(i);
    }else{
      i++;
    }
  }
  return (*this);
}

Object &Object::FlattenSequence(){
  __check(List,*this);
  u_int i = 1;
  while ( i<= Size() ){
    if ( ElementAt(i).ListQ( pond::SYMID_OF_Sequence  ) ){
      if ( _list[0].SymbolQ( SYMID_OF_Dict ) ){ // a dict flatten can only done one by one
        Object obj = ElementAt(i);
        Delete(i);
        for (int j=1; j<= obj.Size(); j++){
          if ( obj[j].PairQ() ){
            (*this).DictInsertOrUpdatePairRef( obj[j] );
          }else{
            zhErroring("字典","试图将非键值对插入字典结构中, 该操作将被忽略" ) ||
              Erroring("Dict","Try insert non key value pair into a dict. Operation will be ignored.");
          }
        }
        
      }else{ // normal flattern can be done directly
        InsertRef( Begin()+i, ElementAt(i).Begin(), ElementAt(i).End() );
        Delete(i);
      }
    }else{
      i++;
    }
  }
  return (*this);
}
  
Object &Object::FlattenListAndSequence(){
  __check(List,*this);
  u_int i = 1;
  while ( i<= Size() ){
    if ( ElementAt(i).ListQ( pond::SYMID_OF_Sequence ) or 
         ElementAt(i).ListQ( _list[0] ) ){
      InsertRef( Begin()+i, ElementAt(i).Begin(), ElementAt(i).End() );
      //ElementAt(i).DeleteNoFree();
      Delete(i);
    }else{
      i++;
    }
  }
  return (*this);
}

Object &Object::ForceDeepen(){
  __check_not_null(*this);
  Object temp;
  temp = *this;
  Free();
  Malloc();
  set_type( ObjectType::List );
  set_idx( GlobalPool.Lists.New() );
  _list.push_back( temp );
  return (*this);
}

Object &Object::Deepen(){
  if ( type() == ObjectType::Number or type() == ObjectType::String )
    { Erroring("Object","Try to Deepen an Object of type "+TypeString()+"."); return *this; }
  ForceDeepen();
  return (*this);
}

Object &Object::DeepenAsFirst(){
  __check_not_null(*this);
  Object temp;
  temp = *this;
  Free();
  Malloc();
  set_type( ObjectType::List );
  set_idx( GlobalPool.Lists.New() );
  _list.push_back( NullObject );
  _list.push_back( temp );
  return (*this);
}

Object &Object::ToList(){
  if ( voidQ() ){
    Malloc();
    ForceDeepen();
    return (*this);
  }
  if ( type() == ObjectType::Number or type() == ObjectType::String ) {
    zhErroring("Pond对象","试图将类型为 "+TypeString()+" 的Pond对象转化为列表.") ||
      Erroring("Object","Try to make Object of type "+TypeString()+" to List.");
    return (*this);
  }
  if ( type() == ObjectType::List ) return *this;
  ForceDeepen();
  return (*this);
}

Object &Object::PushBackRef(const Object&obj){
  //dout<<"Check type"<<endl;
  __check(List,*this);
  //dout<<"push obj "<<obj<<endl;
  _list.push_back( obj );
  //dout<<"push finished"<<endl;
  //dout<<"size = "<<Size()<<endl;
  //dout<<"head = "<<ElementAt(0)<<endl;
  //dout<<"after push :"<<(*this)<<endl;
  return (*this);
}

Object &Object::PushBackCopy(const Object&obj){
  __check(List,*this);
  _list.push_back( obj.Copy() );
  return (*this);
}

Object &Object::PushBackNumber(const double num){
  __check(List,*this);
  Object obj;
  obj.SetNumber(num);
  _list.push_back( obj );
  return (*this);
}

// Object &Object::PushBackComplex(const complex num){
//   __check(List,*this);
//   Object obj;
//   obj.SetComplex(num);
//   _list.push_back( obj );
//   return (*this);
// }

Object &Object::PushBackNumber(const complex num){
  __check(List,*this);
  Object obj;
  obj.SetNumber(num);
  _list.push_back( obj );
  return (*this);
}

Object &Object::PushBackSymbol(const char*key){
  __check(List,*this);
  Object obj;
  obj.SetSymbol(key);
  _list.push_back( obj );
  return (*this);
}

Object &Object::PushBackSymbol(const u_int keyId){
  __check(List,*this);
  Object obj; obj.SetSymbol(keyId);
  _list.push_back( obj );
  return (*this);
}

Object &Object::PushBackList(const char*key){
  __check(List,*this);
  Object obj;
  obj.SetList(key);
  _list.push_back( obj );
  return (*this);
}

Object &Object::PushBackList(const u_int keyId){
  __check(List,*this);
  Object obj; obj.SetList(keyId);
  _list.push_back( obj );
  return (*this);
}

Object &Object::PushBackString(const char*key){
  __check(List,*this);
  Object obj;
  obj.SetString(key);
  _list.push_back( obj );
  return (*this);
}

Object &Object::PushBackString(const string&key){
  PushBackString( key.c_str() );
  return (*this);
}


Object &Object::PushBackNull(){
  __check(List,*this);
  Object obj ;
  _list.push_back(obj);
  return (*this);
}

Object &Object::InsertCopy(u_int pos, const Object&obj){
  __check(List,*this);
  if ( pos > ElementsSize() ) { Erroring("Object","Try to Insert at position out of list range."); return*this; }
  Object temp; temp.CopyObject( obj );
  _list.insert(_list.begin()+pos,temp);
  return (*this);
}

Object &Object::InsertCopy(iterator pos, const Object&obj){
  __check(List,*this);
  Object temp; temp.CopyObject( obj );
  _list.insert(pos,temp);
  return *this;
}

Object &Object::InsertRef(u_int pos, const Object&obj){
  __check(List,*this);
  if ( pos > ElementsSize() ) { Erroring("Object","Try to Insert at position out of list range."); return *this; }
  _list.insert(_list.begin()+pos, obj);
  return *this;
}

Object &Object::InsertRef(iterator pos, const Object&obj){
  __check(List,*this);
  _list.insert(pos, obj);
  return *this;
}

Object &Object::InsertRef(iterator pos, iterator spos, iterator epos){
  __check(List,*this);
  _list.insert(pos,spos,epos);
  return *this;
}

Object &Object::InsertRef(u_int pos, iterator spos, iterator epos){
  __check(List,*this);
  _list.insert(_list.begin()+pos,spos,epos);
  return *this;
}

Object &Object::InsertNumber(u_int pos,double val){
  __check(List,*this);
  if ( pos > ElementsSize() ) { Erroring("Object","Try to Insert at position out of list range."); return*this; }
  Object obj; obj.SetNumber( val );
  _list.insert(_list.begin()+pos, obj);
  return *this;
}

// Object &Object::InsertComplex(u_int pos,complex val){
//   __check(List,*this);
//   if ( pos > ElementsSize() ) { Erroring("Object","Try to Insert at position out of list range."); return*this; }
//   Object obj; obj.SetComplex( val );
//   _list.insert(_list.begin()+pos, obj);
//   return *this;
// }

// Object &Object::InsertNumber(u_int pos,complex val){
//   __check(List,*this);
//   if ( pos > ElementsSize() ) { Erroring("Object","Try to Insert at position out of list range."); return*this; }
//   Object obj; obj.SetNumber( val );
//   _list.insert(_list.begin()+pos, obj);
//   return *this;
// }

Object &Object::InsertSymbol(u_int pos, char*key){
  __check(List,*this);
  if ( pos > ElementsSize() ) { Erroring("Object","Try to Insert at position out of list range."); return*this; }
  Object obj; obj.SetSymbol( key );
  _list.insert(_list.begin()+pos, obj);
  return *this;
}

Object &Object::InsertSymbol(u_int pos, u_int key_id){
  __check(List,*this);
  if ( pos > ElementsSize() ) { Erroring("Object","Try to Insert at position out of list range."); return*this; }
  Object obj; obj.SetSymbol( key_id );
  _list.insert(_list.begin()+pos, obj);
  return *this;
}

Object &Object::InsertString(u_int pos,char *key){
  __check(List,*this);
  if ( pos > ElementsSize() ) {
    Erroring("Object","Try to Insert at position out of list range.");
    return*this;
  }
  Object obj; obj.SetString(key);
  _list.insert(_list.begin()+pos,obj);
  return *this;
}

Object &Object::InsertNull(u_int pos){
  __check(List,*this);
  if ( pos > ElementsSize() ) { Erroring("Object","Try to Insert at position out of list range."); return*this; }
  Object obj;
  _list.insert(_list.begin()+pos,obj);
  return *this;
}

Object &Object::PopBack(){
  __check(List,*this);
  if ( ElementsSize() ==  1 ) { Erroring("Object","Try to PopBack a empty List Object."); return*this; }
  _list.pop_back();
  return *this;
}

Object &Object::Delete(u_int pos){
  __check(List,*this);
  if ( pos > ElementsSize() ) { Erroring("Object","Try to Delete element out of range."); return*this; }
  _list.erase(_list.begin()+pos);
  return *this;
}

Object::iterator Object::Delete(iterator pos){
  __check(List,__iter);
  return _list.erase( pos );
}

Object &Object::Delete(iterator spos, iterator epos){
  __check(List,*this);
  _list.erase(spos,epos);
  return *this;
}

Object &Object::DeleteElements(){
  __check(List,*this);
  vector< Object > &li = GlobalPool.Lists.Get( idx() );
  li.erase( li.begin() +1, li.end() );
  return *this;
}
//////////////////////////////////////////////////////

string Object::ToFullFormString()const{
  if ( voidQ() ) return "Null";
  switch ( type() ){
  case ObjectType::Number: return pond::ToString( re() );
  case ObjectType::String: return (string)"\""+_str+"\"";
  case ObjectType::Symbol:
    if ( ids() == 0 )
      return "Null";
    if ( ids() == SYMID_OF___Variable ){
      return "$_"+pond::ToString( (int)re() );
    }else if ( ids() == SYMID_OF_FunctionVariable ){
      return "$"+pond::ToString( (int)re() );
    }else if ( ids() == SYMID_OF_SerialCode ){
      return "$$_"+pond::ToString( idx().row )+"_"+pond::ToString( idx().col );
    }
    return _sym;
  case ObjectType::List:{
    string str;
    str = _list[0].ToFullFormString();
    auto iter = _list.begin()+1;
    str +="(";
    if ( iter != _list.end() ){
      str+= (*iter).ToFullFormString(); iter++;
    }
    while ( iter != _list.end() ){
      str+=", "+(*iter).ToFullFormString(); iter++;
    }
    return str+")";
  }//end of case List
  }//end of switch
  { Erroring("Object::Switch","ToFullFormString"); return ""; }
}

string Object::ToString(const Object&parentList)const{
  // if ( ListQ( SYMID_OF_FullForm ) and Size() == 1 ){
  //   cerr<< "current list is FullForm when in ToString" << endl;
  //   return _ElementAt(1).ToFullFormString();
  // }else{
  int levp,levl;
  if ( parentList.NullQ() )
    levp = 1000;
  else{
    levp = min( GlobalPool.Symbols.GetPriorityLeft( parentList.SymbolId() ),
                GlobalPool.Symbols.GetPriorityRight( parentList.SymbolId() ) );
  }
  levl = max( GlobalPool.Symbols.GetPriorityLeft( SymbolId() ),
              GlobalPool.Symbols.GetPriorityRight( SymbolId() ) );
  if ( levp < levl ){
    return "("+ToString()+")";
  }
  return ToString();
  // }
}

#define should_be(val) if ( Size()!=(val) ) goto default_label_for_goto;
#define should_gt(val) if ( Size()<=(val) ) goto default_label_for_goto;
#define should_lt(val) if ( Size()>=(val) ) goto default_label_for_goto;
#define CT(key) case pond::RSimpleHashCodeOfString(#key)
//refed
string Object::ToString(const bool is_print)const{
  //dout<<"Try to print "<<ToRawString()<<endl;
  if ( voidQ() ) return "Null";
  string res;
  switch ( type() ){
  case ObjectType::Number:
    if ( re() < 0 )
      return "("+pond::ToString(re())+")";
    else
      return pond::ToString(re());
  case ObjectType::String:
    if ( is_print ) {
      return _str;
    } else {
      res = _str;
      specialCharReplacement( res );
      return "\""+res+"\"";
    }
  case ObjectType::Symbol:{
    if ( ids() == SYMID_OF___Variable ){
      return "$_"+pond::ToString( (int)re() );
    }else if ( ids() == SYMID_OF_FunctionVariable ){
      return "$"+pond::ToString( (int)re() );
    }else if ( ids() == SYMID_OF_SerialCode ){
      return "$$_"+pond::ToString( idx().row )+"_"+pond::ToString( idx().col );
    }
    return _sym;
  }
  case ObjectType::List:{
    if ( _list.size() == 0 )
      { Erroring("Object","Try to transform a List Object to string without head."); return ""; }
    //dout<<"Try Print List with head '"<<_list[0]<<"'"<<endl;
    //head is not a symbol or is null
    if ( not  _list[0].SymbolQ() or _list[0].NullQ() ){
      //dout<<"List head is not a Symbol"<<endl;
      auto iter = _list.begin()+1;
      if ( _list[0].ListQ() ){
        res += "("+_list[0].ToString()+")"+"(" ;
      }else{
        res += _list[0].ToString()+"(" ;
      }
      if ( iter!=_list.end() ){ res+=(*iter).ToString(); iter++; }
      while (iter!=_list.end()){res+=", "+(*iter).ToString(); iter++;}
      return res+")";
    }
    //head is a symbol
    iterator iter;
    switch( SimpleHashCodeOfString( GlobalPool.Symbols.GetKey(_list[0].ids() ) ) ){
      CT(List): { res += "["; iter=_list.begin()+1; if ( iter!=_list.end() ){res+=(*iter).ToString(); iter++;} while (iter!=_list.end()){res+=", "+(*iter).ToString(); iter++;} return res+"]";}
      CT(Dict): { res += "{"; iter=_list.begin()+1;
        if ( iter!=_list.end() ){
          while ( iter!= _list.end() and ( not (*iter).ListQ()  or (*iter).Size()< 2 ) ) iter++; 
          if ( iter == _list.end() ) return res+"}";
          res+=(*iter)[1].ToString()+":"+(*iter)[2].ToString();
          iter++;
        }
        while (iter!=_list.end()){
          if ( not (*iter).ListQ()  or (*iter).Size()< 2 ) continue;
          res+=", "+(*iter)[1].ToString()+":"+(*iter)[2].ToString();
          iter++;
        }
        return res+"}";
      }
      CT(Part):{ should_gt(1);  res += _list[1].ToString()+"["+_list[2].ToString(); iter=_list.begin()+3; while (iter!=_list.end()) {res+=", "+(*iter).ToString(); iter++;} return res+"]";}//
      // CT(Complex):{ should_be(2); return pond::ToString( Complex() ); }
      //<-To_String_Define
      CT(AddTo): { should_be(2); return _list[1].ToString(*this)+"+="+_list[2].ToString(*this); } 
      CT(And): { should_gt(0);  iter=_list.begin()+1; if ( iter!=_list.end() ){res+=(*iter).ToString(*this); iter++;} while (iter!=_list.end()){res+="&&"+(*iter).ToString(*this); iter++;} return res;}
      CT(Black): { should_lt(2); if ( Empty() ) return "~"; return "~"+_list[1].ToString(*this);} 
      CT(BlackSequence): { should_lt(2); if ( Empty() ) return "~~"; return "~~"+_list[1].ToString(*this); } 
      CT(BlackNullSequence): { should_lt(2); if ( Empty() ) return "~~~"; return "~~~"+_list[1].ToString(*this); } 
      CT(Conjunct): {should_be(2);return _list[1].ToString(*this)+"."+(_list)[2].ToString(*this);} 
      CT(CompoundExpression): {if (Empty()) return "";  res+=_list[1].ToString(*this); for ( u_int i =2; i<=Size(); i++) res+=";"+_list[i].ToString(*this); return res;} 
      CT(Decrement): { should_gt(1); return _list[1].ToString(*this)+"--"; } 
      CT(DivideBy): {should_be(2);return _list[1].ToString(*this)+"/="+_list[2].ToString(*this);} 
      //CT(Dot): {return _list[1].ToString(*this)+"."+(_list)[2].ToString(*this);}
      CT(Equal): {should_be(2); return _list[1].ToString(*this)+"=="+_list[2].ToString(*this);}
      CT(Greater): {should_be(2); return _list[1].ToString(*this)+">"+_list[2].ToString(*this);} 
      CT(GreaterEqual): {should_be(2); return _list[1].ToString(*this)+">="+_list[2].ToString(*this);} 
      CT(Increment): {should_be(1); return _list[1].ToString(*this)+"++"; } 
      CT(Less): {should_be(2); return _list[1].ToString(*this)+"<"+_list[2].ToString(*this);} 
      CT(LessEqual): {should_be(2); return _list[1].ToString(*this)+"<="+_list[2].ToString(*this);} 
      CT(Mod): {should_be(2); return _list[1].ToString(*this)+"%"+_list[2].ToString(*this);} 
      CT(ModBy): {should_be(2); return _list[1].ToString(*this)+"%="+_list[2].ToString(*this);} 
      CT(NoPrintExpression): {return "";} 
      CT(Not): {should_gt(1); return "!"+_list[1].ToString(*this);} 
      CT(Or): { should_be(2); return _list[1].ToString(*this)+"||"+_list[2].ToString(*this);} 
      CT(Sequence): {if (this->ElementsSize() == 0 ) return ""; iter=_list.begin()+1; if ( iter!=_list.end() ){res+=(*iter).ToString(); iter++;} while (iter!=_list.end()){res+=", "+(*iter).ToString(); iter++;} return res;}
      CT(Parenthesis): {if (this->ElementsSize() == 0 ) return "()"; res += "("; iter=_list.begin()+1; if ( iter!=_list.end() ){res+=(*iter).ToString(); iter++;} while (iter!=_list.end()){res+=", "+(*iter).ToString(); iter++;} return res+")";}
      CT(Tuple): {if (this->ElementsSize() <= 1 ) return "()"; res += "("; iter=_list.begin()+1; if ( iter!=_list.end() ){res+=(*iter).ToString(); iter++;} while (iter!=_list.end()){res+=","+(*iter).ToString(); iter++;} if ( this->ElementsSize() == 1 ) res+=", "; return res+")";}
      CT(ExpressionList): {if (this->ElementsSize() == 0 ) return "{}"; res += "{"; iter=_list.begin()+1; if ( iter!=_list.end() ){res+=(*iter).ToString(); iter++;} while (iter!=_list.end()){res+=", "+(*iter).ToString(); iter++;} return res+"}";}
      CT(Pattern): {should_gt(1);res += _list[1].ToString(); if ( not( _list[2].ListQ(SYMID_OF_Black)||_list[2].ListQ(SYMID_OF_BlackSequence)||_list[2].ListQ(SYMID_OF_BlackNullSequence) ) ) return res+=_list[2].ToString(*this); else return res+=_list[2].ToString(*this); } 
      CT(Query): {should_be(2);res += _list[1].ToString()+"?"+_list[2].ToString();return res;} 
      CT(Plus): {should_gt(1); res += _list[1].ToString(*this); for (iter = _list.begin()+2; iter!= _list.end(); iter++) res+="+"+(*iter).ToString(*this); return res;} 
      CT(Power): {should_be(2);  res += _list[1].ToString(*this)+"^"+_list[2].ToString(*this);; return res;} 
      CT(PreDecrement): {should_be(1); return "--"+_list[1].ToString(*this);} 
      CT(PreIncrement): {should_be(1); return "++"+_list[1].ToString(*this);} 
      CT(PureFunction): {should_be(1); return _list[1].ToString(*this)+"&";} 
      CT(ReplaceAll): {should_be(2); return _list[1].ToString(*this)+"/."+_list[2].ToString(*this);} 
      CT(ReplaceAllRepeated): {should_be(2); return _list[1].ToString(*this)+"//."+_list[2].ToString(*this);} 
      CT(Rule): {should_be(2); return _list[1].ToString(*this)+"->"+_list[2].ToString(*this);} 
      CT(RuleDelayed): {should_be(2); return _list[1].ToString(*this)+":>"+_list[2].ToString(*this);} 
      CT(KeyValuePair): {should_be(2); return _list[1].ToString(*this)+":"+_list[2].ToString(*this);} 
      CT(Set): {should_be(2); return _list[1].ToString(*this)+"="+_list[2].ToString(*this);} 
      CT(SetDelayed): {should_be(2); return _list[1].ToString(*this)+":="+_list[2].ToString(*this);;} 
      CT(Slot): { should_be(1); return "$_"+_list[1].ToString(*this); }
      CT(SubtractFrom): {return "";} 
      CT(Times): {should_gt(1);  if ( Size()<=0 ) return ""; res += _list[1].ToString(*this); for ( u_int i= 2; i<=Size(); i++ ) res+="*"+_list[i].ToString(*this); return res;} 
      CT(TimesBy): {should_be(2); return _list[1].ToString(*this)+"*="+_list[2].ToString(*this);} 
      CT(UnEqual): {should_be(2); return _list[1].ToString(*this)+"!="+_list[2].ToString(*this);} 
      CT(FullForm):{should_be(1); return _list[1].ToFullFormString(); }
      CT(Unpack): {should_be(1); return "*"+_list[1].ToString(*this);} 
      CT(UnpackDict): {should_be(1); return "**"+_list[1].ToString(*this);} 
    default:
    default_label_for_goto:
      {
        auto iter=_list.begin();
        res += (*iter).ToString(); iter++;
        res += "(";
        if ( iter!=_list.end() ){
          res +=(*iter).ToString(); iter++;
        }
        while (iter!=_list.end()){
          res +=", "+(*iter).ToString(); iter++;
        }
        return res+")";
      }
    }
  }//end of case List
  }//end of switch
  { Erroring("Object::ToString","Switch Error"); return ""; }
};

int Object::SimpleCompare(const Object&l1,const Object&l2){
  if ( l1.objid == l2.objid ) return 0;
  if ( l1.NullQ() ){ if ( l2.NullQ() ) return 0; else return -1;}else if ( l2.NullQ() ) return 1;
  switch ( l1.type() ){
  case ObjectType::Number:{
    switch ( l2.type() ){
    case ObjectType::Number: return compare( (double)l1, double(l2) );
    case ObjectType::String: case ObjectType::Symbol: case ObjectType::List: return -1;
    }
  }//end of Number
  case ObjectType::String:{
    switch ( l2.type() ){
    case ObjectType::Number:  return 1;
    case ObjectType::String: return compare( string(l1) ,string(l2) );
    case ObjectType::Symbol: case ObjectType::List: return -1;
    }
  }//end of String
  case ObjectType::Symbol:{
    switch ( l2.type() ){
    case ObjectType::Number: case ObjectType::String: return 1;
    case ObjectType::Symbol:
      if ( l1.ids() == l2.ids() ){
        if ( l1.ids() == SYMID_OF___Variable or
             l1.ids() == SYMID_OF_FunctionVariable or
             l1.ids() == SYMID_OF_SerialCode ){
          return compare( l1.re(), l2.re() );
        } else{
          return 0;
        }
      }
      // if ( l1.ids() == SYMID_OF_True or l1.ids() ==
      return compare( GlobalPool.Symbols.GetWeight(l1.ids()),
                      GlobalPool.Symbols.GetWeight(l2.ids()) );
    case ObjectType::List: return -1;
    }
  }//end of Symbol
  case ObjectType::List:{
    switch ( l2.type() ){
    case ObjectType::Number: case ObjectType::String: case ObjectType::Symbol: return 1;
    case ObjectType::List:{
      auto iter1 = l1.ElementsBegin();
      auto iter2 = l2.ElementsBegin();
      while ( iter1 != l1.End() && iter2 != l2.End() ){
        if ((*iter1).NullQ() ){
          if ( not (*iter2).NullQ() )
            return -1;
        }else{
          if ( (*iter2).NullQ() ) return 1;
          else{
            return_if_not(SimpleCompare(*iter1,*iter2),0);
          }
        }
        iter1++; iter2++;
      }
      if ( iter1 != l1.End() ) return 1;
      if ( iter2 != l2.End() ) return -1;
      return 0;
    }//end of ..List
    }
  }//end of Symbol
  }//end of switch
  { Erroring("Object::SimpleCompare","switch error"); return 0; }
}

#define exp_return(res) ({dprintf("%d, %s ?<> %s: %d",__LINE__,l1.ToString().c_str(),l2.ToString().c_str(), res); return res;})
int Object::ExpressionCompare(const Object&l1,const Object&l2){
  if ( l1.objid == l2.objid ) return(0);
  if ( l1.NullQ() ){ if ( l2.NullQ() ) return 0; else return -1;}else if ( l2.NullQ() ) return 1;

  //dprintf("%s ?<> %s",l1.ToString().c_str(), l2.ToString().c_str() );
  switch ( l1.type() ){
  case ObjectType::Number:{
    switch ( l2.type() ){
    case ObjectType::Number: return(compare( (double)l1, double(l2) ));
    case ObjectType::String: case ObjectType::Symbol: return(-1);
    case ObjectType::List:{
      ///////////////////////
#define l2_LIST_SITUATION_DEALING                                       \
      if ( l2.ListQ( pond::SYMID_OF_Times  ) and l2.Size() > 0 ){   \
        u_int p = 1;                                                    \
        while ( p< l2.Size() and l2(p).NumberQ() ) p++;                 \
        int res = ExpressionCompare( l1, l2(p) );                       \
        if ( res != 0 ) return( res );                                  \
      }else if ( l2.ListQ(pond::SYMID_OF_Power ) ){                 \
        if ( l2.Size()>=1 ){                                            \
          int res = ExpressionCompare( l1, l2(1) );                     \
          if ( res != 0 ) return( res );                                \
        }                                                               \
      }                                                                 
      ///////////////////////
      l2_LIST_SITUATION_DEALING;
      return(-1);
    }
    }
  }//end of Number
  case ObjectType::String:{
    switch ( l2.type() ){
    case ObjectType::Number:  return(1);
    case ObjectType::String: return(compare( string(l1) ,string(l2) ));
    case ObjectType::Symbol: return(-1);
    case ObjectType::List:{
      l2_LIST_SITUATION_DEALING;
      return(-1);
    }
    }
  }//end of String
  case ObjectType::Symbol:{
    switch ( l2.type() ){
    case ObjectType::Number: case ObjectType::String: return(1);
    case ObjectType::Symbol:
      if ( l1.ids() == l2.ids() ){
        if ( l1.ids() == SYMID_OF___Variable or
             l1.ids() == SYMID_OF_FunctionVariable or
             l1.ids() == SYMID_OF_SerialCode ){
          return compare( l1.re(), l2.re() );
        } else{
          return 0;
        }
      }
      return( compare( GlobalPool.Symbols.GetWeight( l1.ids() ),
                       GlobalPool.Symbols.GetWeight( l2.ids() )
                       ) );
    case ObjectType::List:{
      l2_LIST_SITUATION_DEALING;
      return(-1);
    }
    }
  }//end of Symbol
#undef l2_LIST_SITUATION_DEALING               
  case ObjectType::List:{
    switch ( l2.type() ){
    case ObjectType::Number: case ObjectType::String: return(1);
    case ObjectType::Symbol:{
      // Even if l1 is Times or Power, here will surely bigger than a single symbol
      // no no ,,, previous line is not right, for example: a?<>a^2   a?<>b^2   b?<>a^2
      if ( l1(0).SymbolQ( SYMID_OF_Times ) and l1.Size() > 0 ){
        u_int p = 1;
        while ( p < l1.Size() and l1(p).NumberQ() ) p++;
        int res = ExpressionCompare( l1(p), l2 );
        if ( res != 0 ) return( res );
      }else if ( l1(0).SymbolQ( SYMID_OF_Power ) ){
        if ( l1.Size() == 2 ){
          int res = ExpressionCompare( l1(1), l2 );
          if ( res != 0 ) return( res );
          return(1);
        }
      }
      return(1);
    }
    case ObjectType::List:{
      //dprintf("List Part %s ?<> %s",l1.ToString().c_str(), l2.ToString().c_str() );
      bool isOP1 = false,isOP2=false;
      if ( l1(0).SymbolQ( SYMID_OF_Times ) ){
        isOP1 = true;
        if ( l1.Size() < 1 ) goto l1_is_normal_expr_compr_label;
        if ( l2(0).SymbolQ( SYMID_OF_Times ) ){// 1: both are times
          isOP2 = true;
          goto oper_expression_compare_label;
        }else if ( l2(0).SymbolQ( SYMID_OF_Power) ){ // l1 times  l2 Power
          if ( l2.Size() != 2 ) goto oper_expression_compare_label;
          isOP2 = true;
          u_int p1=1; if ( l1.Size()>=2 and l1(1).NumberQ() ) p1 ++;
          return_if_not( ExpressionCompare( l1(p1), l2(1) ), 0 );
          if ( l1.Size() == p1 )
            return(-1);
          return(1); 
        }
        // l1 is times , l2 are normal
        u_int p1=1; if ( l1.Size()>=2 and l1(1).NumberQ() ) p1 ++;
        return_if_not( ExpressionCompare( l1(p1), l2), 0);
        return(1);
      }else if ( l1(0).SymbolQ( SYMID_OF_Power ) ){
        //dprintf("l1 p, l2 ?::: %s ?<> %s",l1.ToString().c_str(), l2.ToString().c_str() );
        isOP1 = true;
        if ( l1.Size() != 2 ) goto l1_is_normal_expr_compr_label;
        if ( l2(0).SymbolQ( SYMID_OF_Times ) ){// l1 is times l2 times
          if ( l2.Size() > 0 ){  
            isOP2 = true;
            u_int p2 = 1; if ( l2.Size() >= 2 and  l2(1).NumberQ() ) p2 ++;
            return_if_not( ExpressionCompare( l1(1), l2(p2) ),0 );
            if ( l2.Size() == p2 )
              return(1);
            return(-1); 
          }
        }else if ( l2(0).SymbolQ( SYMID_OF_Power) ){ // l1 power l2 Power
          isOP2 = true;
          goto oper_expression_compare_label;
        }
        // l1 is power, l2 are normal
        return_if_not( ExpressionCompare( l1(1), l2),0 );
        //dprintf("l1 p, l2 n::: %s ?== %s",l1.ToString().c_str(), l2.ToString().c_str() );
        return(1); 
      }else{//l1 is normal, test l2
      l1_is_normal_expr_compr_label:
        //dprintf("no diff %s ?<> %s",l1.ToString().c_str(), l2.ToString().c_str() );
        if ( l2(0).SymbolQ( pond::SYMID_OF_Times  ) ){//l1 normal, l2 is times
          if ( l2.Size() < 1 ) goto normal_expression_compare_label;
          isOP2 = true;
          int p = 1; if ( l2.Size()>1 and l2(1).NumberQ() ) p++;
          return_if_not( ExpressionCompare(l1, l2(p)), 0);
          return( -1 );
        }else if ( l2(0).SymbolQ(pond::SYMID_OF_Power ) ){
          if ( l2.Size() != 2 ) goto normal_expression_compare_label;
          isOP2 = true;
          return_if_not( ExpressionCompare(l1,l2(1)),0);
          return( -1 );
        }
        //both are normal
        goto normal_expression_compare_label;
      }
      normal_expression_compare_label:
      return_if_not( ExpressionCompare( l1(0), l2(0) ), 0 );
      //ExpressionCompare elements after same head
      //first non_number then numbers but numbers will be smaller
      oper_expression_compare_label:
      //dprintf("normal %s ?<> %s",l1.ToString().c_str(), l2.ToString().c_str() );
      auto iter1 = l1.Begin();
      auto iter2 = l2.Begin();
      // non numbers
      while ( iter1 != l1.End() and (*iter1).NumberQ() ) iter1++;
      while ( iter2 != l2.End() and (*iter2).NumberQ() ) iter2++;
      auto niter1 = iter1; auto niter2 = iter2;
      //dprintf("niter td");
      while ( iter1 != l1.End() and iter2 != l2.End() ){
        if ( (*iter1).NullQ() ){
          if ( (*iter2).NullQ() ){
            continue;
          }else{
            return( -1 );
          }
        }else{
          if ( (*iter2).NullQ() )
            return( 1 );
          //both are not empty
        }
        //dprintf("%s?<>%s",iter1->ToString().c_str(), iter2->ToString().c_str() );
        return_if_not(ExpressionCompare(*iter1,*iter2),0);
        iter1++; iter2++;
      }
      //dprintf("eles td");
      if ( iter1 != l1.End() ) return(1);
      if ( iter2 != l2.End() ) return(-1);
      if ( niter1 != l1.Begin() ){
        if ( niter2 != l2.Begin() ){
          return_if_not(compare( l1(1).Number(),l2(1).Number() ),0 ) ;
        }else{
          return( 1 );// return(1);
        }
      }else{
        if ( niter2 != l2.Begin() ){
          return( -1 );
        }
      }
      //dprintf("%s ?== %s",l1.ToString().c_str(), l2.ToString().c_str() );
      if ( isOP1 or isOP2 )
        return(ExpressionCompare( l1(0),l2(0) ));
      else
        return(0);
    }//end of ..List
    }
  }//end of Symbol
  }//end of switch
  { Erroring("Object::ExpressionCompare","switch error"); return 0; }
}

#undef exp_return

Object &Object::Sort(const ObjectCompareFunction lcf){
  if ( ElementsSize() > 1 ){
    sort( Begin(),End(), ObjectCompareFunctionLess(lcf) );
  }
  return *this;
}

Object &Object::SimpleSort(){
  static ObjectCompareFunctionLess lessFunc(SimpleCompare);
  if ( ElementsSize() > 1 ){
    sort(Begin(),End() , lessFunc);
  }
  return *this;
}

Object &Object::ExpressionSort(){
  static ObjectCompareFunctionLess lessFunc(ExpressionCompare);
  if ( ElementsSize() > 1 ){
    sort(Begin(),End() , lessFunc);
  }
  return *this;
}

Object &Object::Reverse(){
  __check(List,*this)
  reverse( _list.begin()+1, _list.end() );
  return *this;
}

bool Object::operator<(const Object&other)const{
  return SimpleCompare( *this, other)< 0 ;
}

bool Object::operator==(const Object&other)const{
  return SimpleCompare(*this,other)==0;
}

bool Object::operator!=(const Object&other)const{
  return SimpleCompare(*this,other)!=0;
}

////////////////////////////////////////////////// 
////////////////////////////////////////////////// 
////////////////////////////////////////////////// 
// no need to deal reference in following functions for all of them are using Referenced Functions
int Object::DictGetPosition(const Object&left_value,iterator &iter_pos){
  //dout<<"DictGetPosition: get "<<left_value<<" from table "<<(*this)<<endl;
  int first=1,final = Size(), result=-999;
  u_int pos= final+1;
  while ( first <= final ){
    pos = first+(final-first)/2;
    if ( not ElementAt(pos).ListQ() or ElementAt(pos).Size() < 2 ){
      first = pos+1;
      result = 1;
      continue;
    }
    result = SimpleCompare( left_value, ElementAt(pos)(1) );
    if ( result == 0 ){
      iter_pos = ElementsBegin()+pos;
      //dout<<"find object at "<<pos<<endl;
      return 0;
    }else if ( result < 0 ){
      final = pos-1;
    }else{// result > 0
      first = pos+1;
    }
  }
  //dout<<"find object near "<<pos<<endl;
  if ( pos > Size() ){
    iter_pos = End();
    return -1;
  }
  iter_pos = ElementsBegin()+pos;
  if ( result > 0 ){
    iter_pos ++;
  }
  return -1;
}

Object  Object::DictGetPair(const Object&left_value){
  iterator iter;
  if ( DictGetPosition(left_value,iter) == 0 ){
    return  (*iter);
  }
  return NullObject;
}

Object Object::DictGetOrNewPair(const Object&left_value){
  iterator iter;
  int result;
  if ( (result = DictGetPosition( left_value,iter) ) == 0 ){
    return (*iter);
  }else{
    Object pair(ObjectType::List,SYMID_OF_KeyValuePair );
    pair.PushBackCopy( left_value )
      .PushBackNull();
    InsertRef(iter, pair);
    return pair;
  }
}

Object Object::DictValueAt(const Object&left_value){
  iterator iter;
  if ( DictGetPosition(left_value, iter) == 0 ){
    return (*iter)[2];
  }
  return NullObject;
}

bool  Object::DictInsertPairRef(Object&pair){
  //if ( not pair.PairQ( SYMID_OF_KeyValuePair ) ){
  if ( not pair.PairQ() ){
    zhErroring("字典","只有键值对才能被插入字典中.") ||
      Erroring("Dict","Only key value pair can be inserted into a dict.") ;
    return false;
  }
  iterator iter;
  if ( DictGetPosition( pair[1], iter ) == 0 ){
    return false;
  }else{
    InsertRef( iter, pair );
    return true;
  }
}

bool Object::DictInsertOrUpdatePairRef(Object&pair){
  iterator iter;
  if ( DictGetPosition(pair[1] ,iter ) == 0 ){
    (*iter)[2] = pair[2];
    return true;
  }else{
    InsertRef( iter, pair);
    return true;
  }
}

Object Object::DictPop(const Object&left_value){
  iterator iter;
  if ( DictGetPosition( left_value,iter) == 0 ){
    Object res = (*iter)[2];
    //DeleteNoFree(iter);
    return res;
  }else{
    return NullObject;
  }
}

bool Object::DictDelete(const Object&left_value){
  iterator iter;
  if ( DictGetPosition( left_value,iter) == 0 ){
    Delete(iter);
    return true;
  }else{
    return false;
  }
}

Object& Object::DictSort(){
  sort( Begin(),End());
  return *this;
}

Object::Object(const ObjectType type, const u_int val){
  objid.zero();
  switch ( type ){
  case ObjectType::Symbol:SetSymbol(val); return;
  case ObjectType::Number:SetNumber(val); return;
  case ObjectType::List:  SetList(val);   return;
  default: break;
  }
  Erroring("Object::Object","No constructor for Object("+pond::ToString(type)+",const u_int)"); 
  return; 
}

Object::Object(const ObjectType type, const int val){
  objid.zero();
  switch ( type ){
  case ObjectType::Symbol:SetSymbol(val); return;
  case ObjectType::Number:SetNumber(val); return;
  case ObjectType::List:  SetList(val==0?SYMID_OF_List:val);   return;
  default: break;
  }
  { Erroring("Object::Object","No constructor for Object("+pond::ToString(type)+",const int)"); return; }
}

Object::Object(const ObjectType type, const double val){
  objid.zero();
  switch ( type ){
  case ObjectType::Number:SetNumber(val); return;
  default: break;
  }
  { Erroring("Object::Object","No constructor for Object("+pond::ToString(type)+",const double)"); return; }
}

Object::Object(const ObjectType type, const char* val){
  objid.zero();
  switch ( type ){
  case ObjectType::Symbol:SetSymbol(val); return;
  case ObjectType::String:SetString(val); return;
  case ObjectType::List:  SetList(val);   return;
  default: break;
  }
  { Erroring("Object::Object","No constructor for Object("+pond::ToString(type)+",const char*)"); return; }
}

Object::Object(const ObjectType type, const string&val){
  objid.zero();
  switch ( type ){
  case ObjectType::Symbol:SetSymbol(val.c_str() ); return;
  case ObjectType::String:SetString(val.c_str() ); return;
  case ObjectType::List:  SetList(val.c_str() );   return;
  default: break;
  }
  { Erroring("Object::Object","No constructor for Object("+pond::ToString(type)+",string&)"); return; }
}

Object Object::Copy()const{
  if ( objid.zeroQ() ) return NullObject;
  Object tmp;
  tmp.CopyObject( *this );
  return tmp;
}

string Object::DumpToJson(bool isLeft)const{
  if ( voidQ() ) return "undefined";
  string str;   
  switch ( type() ){
  case ObjectType::Number:
    if ( isLeft ) return "\""+pond::ToString( re() )+"\"";
    return pond::ToString( re() );
  case ObjectType::String:
    str = _str;
    specialCharReplacement( str );
    return (string)"\""+str+"\"";
  case ObjectType::Symbol:
    if ( ids() == 0 )
      return "null";
    if ( ids() == SYMID_OF_False ) return "false";
    if ( ids() == SYMID_OF_True ) return "true";
    if ( ids() == SYMID_OF___Variable ){
      return "\"$_"+pond::ToString( (int)re() )+"\"";
    }else if ( ids() == SYMID_OF_FunctionVariable ){
      return "\"$"+pond::ToString( (int)re() )+"\"";
    }else if ( ids() == SYMID_OF_SerialCode ){
      return "\"$$_"+pond::ToString( idx().row )+"_"+pond::ToString( idx().col )+"\"";
    }
    return string("\"")+_sym+"\"";
  case ObjectType::List:{
    //dout<<" try dump "<<(*this)<< " to json"<<endl;
    if ( isLeft ){
      return string("\"")+(*this).ToString()+"\"";
    }
    static INIT_SYMID_OF( JSON );
    if ( Size() < 1 )  return "{}"; 
    bool is_head = true;
    bool is_array = true;
    for( int i=1; i<= Size() ; i++ ){
      if ( _list[i].NullQ() ){
        continue;
      }
      if ( _list[i].ListQ() and _list[i].Size() == 2 and _list[i][1].NullQ() ){
        continue;
      }
      // if (  i == Size() and _list[i].NullQ() ){
      if ( not is_head ) str += ", "; else is_head = false;
      if ( _list[i].PairQ(SYMID_OF_KeyValuePair ) ) {
        str +=  _list[i](1).DumpToJson(true) + ":" + _list[i](2).DumpToJson();
        is_array = false;
        continue;
      }
      if ( not is_array ) str += "\"" + pond::ToString( i-1 ) + "\":";
      if( _list[i].ListQ( SYMID_OF_Complex ) ){
        str += "\"complex("+_list[i](1).ToString() + "," + _list[i](2).ToString()+")\"";
      }else{
        str += _list[i].DumpToJson();
      }
    }

    if ( is_array ) str = "[" + str + "]";
    else            str = "{" + str + "}";
    bool isJSON = (ListQ( SYMID_OF_JSON ) or ListQ( SYMID_OF_Dict )); 
    bool isArray = ( ListQ( SYMID_OF_List) or 
                     ListQ( SYMID_OF_Parenthesis) or
                     ListQ( SYMID_OF_ExpressionList) );
    if ( isJSON or isArray ){
      return str;
    }else{
      return "[\"$POND$"+_list[0].ToString()+"\","+str+"]";
    }
    
  }//end of case List
  }//end of switch
  Erroring("Object::Switch","DumpToJson");
  return "";

}

Object&Object::LoadFromJson(const std::string str){
  return (*this);
}

double &Object::ref_double(){
  return _obj.val._re;
};
string &Object::ref_string(){
  return _str;
};
vector<Object> &Object::ref_list(){
  return _list;
}


#undef _obj
#undef _sym
#undef _str
#undef _list

