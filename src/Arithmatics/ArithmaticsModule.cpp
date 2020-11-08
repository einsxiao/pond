#include<pond>
#include<math.h>
#include"ArithmaticsModule.h"
using namespace std;
using namespace pond;

// static INIT_OPER_SYMID_OF(且,                    170,169,   3,1);
// static INIT_OPER_SYMID_OF(或,                    175,174,   3,1);
// static INIT_OPER_SYMID_OF(非,                      0,168,   1,1);
// static INIT_OPER_SYMID_OF(大于,                  130,129,   3,1);
// static INIT_OPER_SYMID_OF(大于等于,              130,129,   3,1);
// static INIT_OPER_SYMID_OF(小于,                  130,129,   3,1);
// static INIT_OPER_SYMID_OF(小于等于,              130,129,   3,1);
// static INIT_OPER_SYMID_OF(等于,                  130,129,   3,1);
// static INIT_OPER_SYMID_OF(不等于,                130,129,   3,1); 

__DEBUG_MAIN__("input.pd");

ArithmaticsModule::ArithmaticsModule():Module(MODULE_NAME){
  {
    Object S(ObjectType::Symbol,"Pi"),  v(ObjectType::Number,3.1415926535897932385);

    EvaKernel->InsertOrUpdateBottomPairValue(S,v );
    AddAttribute("Pi",AttributeType::Protected );
  }
  {
    Object S(ObjectType::Symbol,"E"),   v(ObjectType::Number,2.7182818284590452353 );
    EvaKernel->InsertOrUpdateBottomPairValue(S,v );
    AddAttribute("E",AttributeType::Protected );
  }
  ///////////////////////////////////////////////////////////
  // arithmetic functions
  {
    RegisterFunction("Plus",Plus,this);
    AddAttribute("Plus",AttributeType::Flat);
    AddAttribute("Plus",AttributeType::Orderless);
    AddAttribute("Plus",AttributeType::Listable);
    AddAttribute("Plus",AttributeType::OneIdentity);
  }
  {
    RegisterFunction("Times",Times,this);
    AddAttribute("Times",AttributeType::Flat);
    AddAttribute("Times",AttributeType::Orderless);
    AddAttribute("Times",AttributeType::Listable);
    AddAttribute("Times",AttributeType::OneIdentity);
  }
  {
    RegisterFunction("Power",Power,this);
    AddAttribute("Power",AttributeType::Listable);
  }
  {
    RegisterFunction("Log",Log,this);
    AddAttribute("Log",AttributeType::Listable);
    RegisterFunction("ln",Log,this);
    AddAttribute("ln",AttributeType::Listable);
  }
  ///////////////////////////////
  {
    RegisterFunction("AddTo",AddTo,this);
    AddAttribute("AddTo",AttributeType::HoldFirst);
  }
  {
    RegisterFunction("SubtractFrom",SubtractFrom,this);
    AddAttribute("SubtractFrom",AttributeType::HoldFirst);
  }
  {
    RegisterFunction("TimesBy",TimesBy,this);
    AddAttribute("TimesBy",AttributeType::HoldFirst);
  }
  {
    RegisterFunction("DivideBy",DivideBy,this);
    AddAttribute("DivideBy",AttributeType::HoldFirst);
  }
  {
    RegisterFunction("PowerWith",PowerWith,this);
    AddAttribute("PowerWith",AttributeType::HoldFirst);
  }
  ///////////////////////////////////////////////////////////////////////
  {
    RegisterFunction("Increment",Increment,this);
    AddAttribute("Increment",AttributeType::HoldFirst);
  }
  {
    RegisterFunction("PreIncrement",PreIncrement,this);
    AddAttribute("PreIncrement",AttributeType::HoldFirst);
  }
  {
    RegisterFunction("Decrement",Decrement,this);
    AddAttribute("Decrement",AttributeType::HoldFirst);
  }
  {
    RegisterFunction("PreDecrement",PreDecrement,this);
    AddAttribute("PreDecrement",AttributeType::HoldFirst);
  }
  ///////////////////////////////////////////////////////////
  {
    RegisterFunction("Greater",Greater,this);
    RegisterFunction("GreaterEqual",GreaterEqual,this);
    RegisterFunction("Less",Less,this);
    RegisterFunction("LessEqual",LessEqual,this);
    RegisterFunction("Equal",Equal,this);
    RegisterFunction("UnEqual",UnEqual,this);

    RegisterFunction("大于",Greater,this);
    RegisterFunction("大于等于",GreaterEqual,this);
    RegisterFunction("小于",Less,this);
    RegisterFunction("小于等于",LessEqual,this);
    RegisterFunction("等于",Equal,this);
    RegisterFunction("不等于",UnEqual,this);
  }
  ///////////////////////////////////////////////////////////
  {
    RegisterFunction("And",And,this);
    AddAttribute("And",AttributeType::HoldAll);
    AddAttribute("And",AttributeType::Flat);
    AddAttribute("And",AttributeType::OneIdentity);

    RegisterFunction("and",And,this);
    AddAttribute("and",AttributeType::HoldAll);
    AddAttribute("and",AttributeType::Flat);
    AddAttribute("and",AttributeType::OneIdentity);

    RegisterFunction("且",And,this);
    AddAttribute("且",AttributeType::HoldAll);
    AddAttribute("且",AttributeType::Flat);
    AddAttribute("且",AttributeType::OneIdentity);
  }
  {
    RegisterFunction("Or",Or,this);
    AddAttribute("Or",AttributeType::HoldAll);
    AddAttribute("Or",AttributeType::Flat);
    AddAttribute("Or",AttributeType::OneIdentity);

    RegisterFunction("or",Or,this);
    AddAttribute("or",AttributeType::HoldAll);
    AddAttribute("or",AttributeType::Flat);
    AddAttribute("or",AttributeType::OneIdentity);

    RegisterFunction("或",Or,this);
    AddAttribute("或",AttributeType::HoldAll);
    AddAttribute("或",AttributeType::Flat);
    AddAttribute("或",AttributeType::OneIdentity);
  }
  {
    RegisterFunction("Not",Not,this);
    RegisterFunction("not",Not,this);
    RegisterFunction("非",Not,this);
  }
  ///////////////////////////////////////////////////////////
  {
    RegisterFunction("Abs",Abs,this);
    AddAttribute("Abs",AttributeType::Listable);
  }
  // RegisterFunction("Factorial",Factorial,this);
  // normal member functions
  RegisterFunction("Random",Random,this,"give a uniform random number in the range of (0,1).");
  RegisterFunction("rand",Random,this,"give a uniform random number in the range of (0,1).");
  RegisterFunction("随机数",Random,this,"给出 (0,1) 范围的平均分布的随机数.");
  srand( (unsigned)time(NULL) );
};

ArithmaticsModule::~ArithmaticsModule(){};

int ArithmaticsModule::Plus(Object &ARGV){
  //dout<<"  step into Plus"<<endl;
  // ReturnIfLessThan(1);
  ARGV.FlattenListAndSequence();
  // if ( ARGV.Size() == 1 ){
  //   ARGV = ARGV[1];
  //   ReturnHold;
  // }
  if ( ARGV.Size() > 0 && ARGV[1].StringQ() ){
    INIT_SYMID_OF( ToString );
    ARGV[0].SetSymbol(SYMID_OF_ToString);
    return EvaKernel->Evaluate(ARGV);

  }
  double sum = 0;
  u_int p = ARGV.Size();
  while ( p >=1 ) {
    if ( ARGV[p].RealQ() ){
      sum+=(double)ARGV[p];
      ARGV.Delete(p);
    }
    p--;
  }
  //dout<<"  treat all elements"<<endl;
  // cout<<ARGV<<endl;
  if ( ARGV.Size() == 0 ){
    ReturnNumber( sum );
  }
  if ( sum != 0.0 ){ //!=0
    ARGV.InsertNumber(1,sum);
  }
  if ( ARGV.Size() >= 2 and ( ARGV[1].StringQ() or ARGV[2].StringQ() ) ){
    string res;
    if ( ARGV[1].StringQ() ) res += ARGV[1].Key();
    else res += ARGV[1].ToString();

    if ( ARGV.Size() == 2 ){
      ARGV.SetString( res );
      ReturnHold;
    }else{
      ARGV[1].SetString( res );
      ARGV.Delete(2);
      return EvaKernel->Evaluate( ARGV );
    }
    ReturnHold;
  }
  ARGV.ExpressionSort();
  ReturnHold;
}

int ArithmaticsModule::Times(Object &ARGV){
  CheckShouldNoLessThan(1);
  ARGV.FlattenListAndSequence();
  // if ( ARGV.Size() == 1 ){
  //   ARGV = ARGV[1];
  //   ReturnHold;
  // }
  //dprintf("Calculate %s",ARGV.ToString().c_str() );
  double res=1;
  u_int p = ARGV.Size();
  while ( p >=1 ) {
    //EvaKernel->Evaluate(ARGV[p]);
    if ( ARGV[p].RealQ() ){
      res*=(double)ARGV[p];
      ARGV.Delete(p);
    }
    p--;
  }
  // cout<<ARGV<<endl;
  if ( res == 0 or ARGV.Size() == 0 ){
    ReturnNumber( res );
  }
  if ( res != 1.0 ){ //!=0
    ARGV.InsertNumber(1,res);
    ReturnHold;
  }
  ReturnHold;
}

int ArithmaticsModule::Power(Object &ARGV){
  CheckShouldEqual(2);
  if ( ARGV[2].RealQ() ){
    double index = (double) ARGV[2];
    if ( index == 0 )
      ReturnNumber(1);
    if ( index == 1 ){
      ARGV = ARGV[1];
      ReturnNormal;
    }
    if ( ARGV[1].RealQ() )
      ReturnNumber( pow((double)ARGV[1],index ) );
  }
  ReturnHold;
}

int ArithmaticsModule::Log(Object &ARGV){
  CheckShouldBeWithin(1,2);
  if ( not ARGV[1].RealQ() ) ReturnNormal;
  if ( ARGV.Size() == 1 ){
    double x = (double)ARGV[1];
    ReturnNumber( log(x) );
  }
  if ( not ARGV[2].RealQ() ) ReturnNormal;
  ReturnNumber( log((double)ARGV[2])/log((double)ARGV[1]) );
}

// int ArithmaticsModule::Complex(Object &ARGV){
//   CheckShouldEqual(2);
//   //EvaKernel->Evaluate( ARGV[1] );
//   //EvaKernel->Evaluate( ARGV[2] );
//   if ( ARGV[1].RealQ() && ARGV[2].RealQ() ){
//     ReturnNormal;
//   }
//   ReturnHold;
// }

int ArithmaticsModule::AddTo(Object &ARGV){
  CheckShouldEqual(2);
  Object &var = ARGV[1];
  Object &obj = ARGV[2];
  if ( var.SymbolQ() ){
    Object value = EvaKernel->GetValuePairRef( var );
    if ( not value.NullQ() ){
      //EvaKernel->Evaluate( obj );
      if ( value[2].NumberQ() && obj.NumberQ() ){
        value[2].SetNumber( (double)value[2]+(double)obj );
        ARGV.CopyObject(value[2]);
        ReturnNormal;
      }
    }else{
      Erroring(ARGV.Key(),ARGV[1].ToString()+" is not a variable with a value.");
      ReturnError;
    }
  }
  //change to form of x = x + 1
  Object newS; newS.SetList( SYMID_OF_Set );
  Object newR; newR.SetList( SYMID_OF_Plus );
  newR.PushBackCopy( var ); newR.PushBackRef( obj );
  newS.PushBackRef( var ); newS.PushBackRef( newR );
  ARGV = newS;
  return EvaKernel->Evaluate( ARGV );
}

int ArithmaticsModule::SubtractFrom(Object &ARGV){
  CheckShouldEqual(2);
  Object &var = ARGV[1];
  Object &obj = ARGV[2];
  if ( var.SymbolQ() ){
    Object value = EvaKernel->GetValuePairRef( var );
    if ( not value.NullQ() ){
      //EvaKernel->Evaluate( obj );
      if ( value[2].NumberQ() && obj.NumberQ() ){
        value[2].SetNumber( (double)value[2]-(double)obj );
        ARGV.CopyObject(value[2]);
        ReturnNormal;
      }
    }else{
      Erroring(ARGV.Key(),ARGV[1].ToString()+" is not a variable with a value.");
      ReturnError;
    }
  }
  //change to form of x = x + (-1*a)
  Object newS; newS.SetList( SYMID_OF_Set );
  Object newR; newR.SetList( SYMID_OF_Plus );
  newR.PushBackCopy( var );
  Object tobj; tobj.SetList( SYMID_OF_Times ); tobj.PushBackNumber(-1); tobj.PushBackRef( obj );
  newR.PushBackRef( tobj );
  newS.PushBackRef( var ); newS.PushBackRef( newR );
  ARGV = newS;
  return EvaKernel->Evaluate( ARGV );
}

int ArithmaticsModule::TimesBy(Object &ARGV){
  CheckShouldEqual(2);
  Object &var = ARGV[1];
  Object &obj = ARGV[2];
  if ( var.SymbolQ() ){
    Object value = EvaKernel->GetValuePairRef( var );
    if ( not value.NullQ() ){
      //EvaKernel->Evaluate( obj );
      if ( value[2].NumberQ() && obj.NumberQ() ){
        value[2].SetNumber( (double)value[2]*(double)obj );
        ARGV.CopyObject(value[2]);
        ReturnNormal;
      }
    }else{
      Erroring(ARGV.Key(),ARGV[1].ToString()+" is not a variable with a value.");
      ReturnError;
    }
  }
  //change to form of x = x * a
  Object newS; newS.SetList( SYMID_OF_Set );
  Object newR; newR.SetList( SYMID_OF_Times );
  newR.PushBackCopy( var ); newR.PushBackRef( obj );
  newS.PushBackRef( var ); newS.PushBackRef( newR );
  ARGV = newS;
  return EvaKernel->Evaluate( ARGV );
}

int ArithmaticsModule::DivideBy(Object &ARGV){
  CheckShouldEqual(2);
  Object &var = ARGV[1];
  Object &obj = ARGV[2];
  if ( var.SymbolQ() ){
    Object value = EvaKernel->GetValuePairRef( var );
    if ( not value.NullQ() ){
      //EvaKernel->Evaluate( obj );
      if ( value[2].NumberQ() && obj.NumberQ() ){
        value[2].SetNumber( (double)value[2]/(double)obj );
        ARGV.CopyObject(value[2]);
        ReturnNormal;
      }
    }else{
      Erroring(ARGV.Key(),ARGV[1].ToString()+" is not a variable with a value.");
      ReturnError;
    }
  }
  //change to form of x = x + (-1*a)
  Object newS; newS.SetList( SYMID_OF_Set );
  Object newR; newR.SetList( SYMID_OF_Times );
  newR.PushBackCopy( var );
  Object tobj; tobj.SetList( SYMID_OF_Power ); tobj.PushBackRef( obj );  tobj.PushBackNumber(-1);
  newR.PushBackRef( tobj );
  newS.PushBackRef( var ); newS.PushBackRef( newR );
  ARGV = newS;
  return EvaKernel->Evaluate( ARGV );
}

int ArithmaticsModule::PowerWith(Object &ARGV){
  CheckShouldEqual(2);
  Object &var = ARGV[1];
  Object &obj = ARGV[2];
  if ( var.SymbolQ() ){
    Object value = EvaKernel->GetValuePairRef( var );
    if ( not value.NullQ() ){
      //EvaKernel->Evaluate( obj );
      if ( value[2].RealQ() && obj.RealQ() ){
        value[2].SetNumber( pow( (double)value[2],(double)obj ) );
        ARGV.CopyObject(value[2]);
        ReturnNormal;
      }
    }else{
      Erroring(ARGV.Key(),ARGV[1].ToString()+" is not a variable with a value.");
      ReturnError;
    }
  }
  //change to form of x = x ^ a
  Object newS; newS.SetList( SYMID_OF_Set );
  Object newR; newR.SetList( SYMID_OF_Power );
  newR.PushBackCopy( var ); newR.PushBackRef( obj );
  newS.PushBackRef( var ); newS.PushBackRef( newR );
  ARGV = newS;
  return EvaKernel->Evaluate( ARGV );
}



int ArithmaticsModule::Increment(Object &ARGV){
  CheckShouldEqual(1);
  Object&var = ARGV[1];
  if ( var.SymbolQ() ){
    Object value = EvaKernel->GetValuePairRef( var );
    if ( not value.NullQ() ){
      if ( value[2].NumberQ() ){
        ARGV.CopyObject( value[2] );
        value[2].SetNumber( (double)value[2]+1.0 );
        ReturnNormal;
      }
    }else{
      Erroring(ARGV.Key(),ARGV[1].ToString()+" is not a variable with a value.");
      ReturnError;
    }
  }
  //change to form of x = x + 1 
  Object newS; newS.SetList( SYMID_OF_Set );
  Object newR; newR.SetList( SYMID_OF_Plus );
  newR.PushBackRef( var ); newR.PushBackNumber( 1.0 );
  newS.PushBackCopy( var ); newS.PushBackRef( newR );
  //EvaKernel->Evaluate(var);
  ARGV = ARGV[ 1 ] ;
  EvaKernel->Evaluate( newS );
  ReturnNormal;
 };

int ArithmaticsModule::Decrement(Object &ARGV){
  CheckShouldEqual(1);
  Object&var = ARGV[1];
  if ( var.SymbolQ() ){
    Object value = EvaKernel->GetValuePairRef( var );
    if ( not value.NullQ() ){
      if ( value[2].NumberQ() ){
        ARGV.CopyObject( value[2] );
        value[2].SetNumber( (double)value[2]-1.0 );
        ReturnNormal;
      }
    }else{
      Erroring(ARGV.Key(),ARGV[1].ToString()+" is not a variable with a value.");
      ReturnError;
    }
  }
  //change to form of x = x + 1 
  Object newS; newS.SetList( SYMID_OF_Set );
  Object newR; newR.SetList( SYMID_OF_Plus );
  newR.PushBackRef( var ); newR.PushBackNumber( -1.0 );
  newS.PushBackCopy( var ); newS.PushBackRef( newR );
  //EvaKernel->Evaluate(var);
  ARGV = ARGV[ 1 ] ;
  EvaKernel->Evaluate( newS );
  ReturnNormal;
 };

int ArithmaticsModule::PreIncrement(Object &ARGV){
  CheckShouldEqual(1);
  Object &var = ARGV[1];
  if ( var.SymbolQ() ){
    Object value = EvaKernel->GetValuePairRef( var );
    if ( not value.NullQ() ){
      if ( value[2].NumberQ() ){
        value[2].SetNumber( (double)value[2]+1.0 );
        ARGV.CopyObject( value[2] );
        ReturnNormal;
      }
    }else{
      Erroring(ARGV.Key(),ARGV[1].ToString()+" is not a variable with a value.");
      ReturnError;
    }
  }
  //change to form of x = x ^ a
  Object newS; newS.SetList( SYMID_OF_Set );
  Object newR; newR.SetList( SYMID_OF_Plus );
  newR.PushBackRef( var ); newR.PushBackNumber( 1.0 );
  newS.PushBackCopy( var ); newS.PushBackRef( newR );
  ARGV = newS;
  return EvaKernel->Evaluate( ARGV );
};


int ArithmaticsModule::PreDecrement(Object &ARGV){
  CheckShouldEqual(1);
  Object &var = ARGV[1];
  if ( var.SymbolQ() ){
    Object value = EvaKernel->GetValuePairRef( var );
    if ( not value.NullQ() ){
      if ( value[2].NumberQ() ){
        value[2].SetNumber( (double)value[2]-1.0 );
        ARGV.CopyObject( value[2] );
        ReturnNormal;
      }
    }else{
      Erroring(ARGV.Key(),ARGV[1].ToString()+" is not a variable with a value.");
      ReturnError;
    }
  }
  //change to form of x = x ^ a
  Object newS; newS.SetList( SYMID_OF_Set );
  Object newR; newR.SetList( SYMID_OF_Plus );
  newR.PushBackRef( var ); newR.PushBackNumber( -1.0 );
  newS.PushBackCopy( var ); newS.PushBackRef( newR );
  ARGV = newS;
  return EvaKernel->Evaluate( ARGV );
};

// is to deal with form like (a < b) < c or (a < b) >c
// will be transformed to    And[a<b,b<c]  And[a<b,b>c]
#define __pre_treatment                                       \
  if ( ( left.ListQ(SYMID_OF_Greater ) )                      \
       ||left.ListQ(SYMID_OF_GreaterEqual )                   \
       ||left.ListQ(SYMID_OF_Less )                           \
       ||left.ListQ(SYMID_OF_LessEqual )                      \
       ||left.ListQ(SYMID_OF_Equal )                          \
       ||left.ListQ(SYMID_OF_UnEqual )                        \
  ){                                                          \
    CheckArgsShouldEqual(left,2);                             \
    Object newleft;  newleft = left; ARGV.Delete(1);          \
    Object newright; newright.SetList( ARGV[0].ids() );       \
    newright.PushBackCopy( newleft[2] );                      \
    newright.PushBackRef( ARGV[1] );                          \
    ARGV.SetList( pond::SYMID_OF_And );                       \
    ARGV.PushBackRef( newleft ); ARGV.PushBackRef(newright);  \
    EvaKernel->Evaluate(ARGV);                                \
    ReturnNormal;                                             \
  }                                                           \

int ArithmaticsModule::Greater(Object &ARGV){
  CheckShouldEqual(2);
  Object&left  = ARGV(1);
  Object&right = ARGV(2);
  __pre_treatment;
  //EvaKernel->Evaluate( left );
  //EvaKernel->Evaluate( right );
  if ( left.RealQ() && right.RealQ() ){
    ReturnBoolean( (double)left> (double)right );
    ReturnNormal;
  }
  ReturnHold;
}

int ArithmaticsModule::GreaterEqual(Object &ARGV){
  CheckShouldEqual(2);
  Object&left = ARGV(1);
  Object&right= ARGV(2);
  __pre_treatment;
  //EvaKernel->Evaluate( left );
  //EvaKernel->Evaluate( right );
  if ( left.RealQ() && right.RealQ() ){
    ReturnBoolean(  (double)left>=double(right) );
  }
  ReturnHold;
}

int ArithmaticsModule::Less(Object &ARGV){
  CheckShouldEqual(2);
  Object&left = ARGV(1);
  Object&right= ARGV(2);
  __pre_treatment;
  //EvaKernel->Evaluate( left );
  //EvaKernel->Evaluate( right );
  if ( left.RealQ() && right.RealQ() ){
    ReturnBoolean( double(left)<double(right) );
  }
  ReturnHold;
}

int ArithmaticsModule::LessEqual(Object &ARGV){
  CheckShouldEqual(2);
  Object&left = ARGV(1);
  Object&right= ARGV(2);
  __pre_treatment;
  // EvaKernel->Evaluate( left );
  // EvaKernel->Evaluate( right );
  if ( left.RealQ() && right.RealQ() ){
    ReturnBoolean( double(left) <= double(right) );
  }
  ReturnHold;
}

int ArithmaticsModule::Equal(Object &ARGV){
  CheckShouldEqual(2);
  Object&left = ARGV(1);
  Object&right= ARGV(2);
  __pre_treatment;
  // EvaKernel->Evaluate( left );
  // EvaKernel->Evaluate( right );
  if ( left.NumberQ() && right.NumberQ() ){
    ReturnBoolean( left == right );
  }else{
    if ( left == right ){
      ReturnBoolean( true );
    }
  }
  ReturnNormal;
}
int ArithmaticsModule::UnEqual(Object &ARGV){
  CheckShouldEqual(2);
  Object&left = ARGV(1);
  Object&right= ARGV(2);
  __pre_treatment;
  // EvaKernel->Evaluate( left );
  // EvaKernel->Evaluate( right );
  if ( left.NumberQ() && right.NumberQ() ){
    ARGV.SetBoolean( double(left) != double(right) );
    ReturnNormal;
  }
  ReturnHold;
}

#undef __pre_treatment

int ArithmaticsModule::And(Object &ARGV){
  CheckShouldNoLessThan(2);
  bool unkonwn = false;
  for ( u_int i = 1; i<=ARGV.Size(); i++ ){
    EvaKernel->Evaluate( ARGV[i] );
    if ( ARGV[i].Boolean() == 0 ){ //false
      ReturnBoolean(false);
    }else if ( ARGV[i].Boolean() < 0 ){
      unkonwn = true;
    }
  }
  if ( unkonwn )
    ReturnHold;
  ReturnBoolean(true);
}

int ArithmaticsModule::Or(Object &ARGV){
  CheckShouldNoLessThan(2);
  int unknownExpr= false;
  for ( u_int i = 1; i<=ARGV.Size(); i++ ){
    EvaKernel->Evaluate( ARGV[i] );
    if ( ARGV[i].Boolean() > 0 ){
      ReturnBoolean(true);
    }else if ( ARGV[i].Boolean()< 0 ){
      unknownExpr = true;
    }
  }
  if ( unknownExpr )
    ReturnHold;
  ReturnBoolean(false);
}

int ArithmaticsModule::Not(Object &ARGV){
  CheckShouldEqual(1);
  // EvaKernel->Evaluate( ARGV[1] );
  if ( ARGV[1].Boolean() > 0 ){
    ReturnBoolean( false );
  }else if ( ARGV[1].Boolean() == 0 ){
    ReturnBoolean(true);
  }
  ReturnHold;
}

// int ArithmaticsModule::Factorial(Object &ARGV){
//   CheckShouldEqual(1);
//   if ( not ARGV[1].RealQ() ) ReturnHold ;
//   long res=1, num = ARGV[1].Number();
//   for (int i = 2 ;i <=num; i++)
//     res*=i;
//   ARGV.SetNumber((double)res);
//   ReturnNormal ;
// }

int ArithmaticsModule::Abs(Object &ARGV){
  CheckShouldEqual(1);
  if ( ARGV[1].RealQ() ){
    ReturnNumber( abs((double)ARGV[1]) );
  }
  ReturnHold;
}

int ArithmaticsModule::SetRandomSeed(Object &ARGV){
  CheckShouldEqual(1);
  ReturnIfNotNumber(1);
  srand( (int)ARGV[1] );
  ReturnNumber(  (double)rand()/RAND_MAX );
}

int ArithmaticsModule::Random(Object &ARGV){
  ReturnNumber((double)rand()/RAND_MAX);
}

int ArithmaticsModule::PD_Sin(Object&ARGV){
  CheckShouldEqual(1);
  if ( ARGV[1].RealQ() ){
    ReturnNumber( sin((double)ARGV[1]) );
  }
  ReturnHold;
}

int ArithmaticsModule::PD_Cos(Object&ARGV){
  CheckShouldEqual(1);
  if ( ARGV[1].RealQ() ){
    ReturnNumber( cos((double)ARGV[1]) );
  }
  ReturnNormal;
}

int ArithmaticsModule::PD_Tan(Object&ARGV){
  CheckShouldEqual(1);
  if ( ARGV[1].RealQ() ){
    ReturnNumber( tan((double)ARGV[1]) );
  }
  ReturnHold;
}

int ArithmaticsModule::PD_Ln(Object&ARGV){
  CheckShouldEqual(1);
  if ( not ARGV[1].RealQ() ) ReturnHold;
  double x = (double)ARGV[1];
  ReturnNumber( log(x) );
}

int ArithmaticsModule::PD_Mod(Object&ARGV){
  /*
    ArgvN: 2
    Attributes: Listable 
    求x/y的余数
   */
  CheckShouldEqual(2);
  if ( not ARGV[1].RealQ() or not ARGV[2].RealQ() ) ReturnHold;
  double x = ARGV[1].Number(); 
  double y = ARGV[2].Number();
  ReturnNumber( x - long(x/y)*y );
}

int ArithmaticsModule::PD_Floor(Object&ARGV){
  /*Attributes: Listable 
    ArgvN: 1
    对实数下取整
   */
  if ( not ARGV[1].RealQ() ) ReturnHold;
  double x = (double)ARGV[1]; 
  ReturnNumber( floor(x) );
}

int ArithmaticsModule::PD_Ceil(Object&ARGV){
  /*Attributes: Listable 
    ArgvN: 1
    对实数上取整
   */
  if ( not ARGV[1].RealQ() ) ReturnHold;
  double x = (double)ARGV[1]; 
  ReturnNumber( ceil(x) );
}

int ArithmaticsModule::PD_Round(Object&ARGV){
  /*Attributes: Listable 
    ArgvN: 1
    对实数四舍五入取整
   */
  if ( not ARGV[1].RealQ() ) ReturnHold;
  double x = (double)ARGV[1]; 
  ReturnNumber( round(x) );
}

int ArithmaticsModule::PD_Sqrt(Object&ARGV){
  /*Attributes: Listable 
    ArgvN: 1
    对实数四舍五入取整
   */
  if ( not ARGV[1].RealQ() ) ReturnHold;
  ReturnNumber( sqrt((double)ARGV[1]) );
}

int ArithmaticsModule::PD_Max(Object&ARGV){
  /*Attributes: Listable 
    ArgvN: 1
    对实数四舍五入取整
   */
  bool find = false;
  double v;
  for (auto iter = ARGV.Begin(); iter != ARGV.End(); iter++){
    if ( iter->RealQ() ){
      if ( find ){
        v = max( (double)(*iter), v );
      }else{
        v = (double)(*iter);
        find = true;
      }
    }else{
      ReturnHold;
    }
  }
  ReturnNumber( v );
}

int ArithmaticsModule::PD_Min(Object&ARGV){
  /*Attributes: Listable 
    ArgvN: 1
    对实数四舍五入取整
   */
  bool find = false;
  double v;
  for (auto iter = ARGV.Begin(); iter != ARGV.End(); iter++){
    if ( iter->RealQ() ){
      if ( find ){
        v = min( (double)(*iter), v );
      }else{
        v = (double)(*iter);
        find = true;
      }
    }else{
      ReturnHold;
    }
  }
  ReturnNumber( v );
}


int ArithmaticsModule::PD_RealQ(Object&ARGV){
  CheckShouldEqual(1);
  ReturnBoolean( ARGV[1].RealQ() );
}

int ArithmaticsModule::PD_IntegerQ(Object&ARGV){
  CheckShouldEqual(1);
  ReturnBoolean( ARGV[1].IntegerQ() );
}

int ArithmaticsModule::PD_EvenQ(Object&ARGV){
  CheckShouldEqual(1);
  double x = ARGV[1].Number();
  if ( x != round(x) ) ReturnBoolean(false);
  if ( ((long)x)%2 == 0 ) ReturnBoolean( true );
  ReturnBoolean(false);
}

int ArithmaticsModule::PD_OddQ(Object&ARGV){
  CheckShouldEqual(1);
  double x = ARGV[1].Number();
  if ( x != round(x) ) ReturnBoolean(false);
  if ( ((long)x)%2 == 0 ) ReturnBoolean( false );
  ReturnBoolean( true );
}
