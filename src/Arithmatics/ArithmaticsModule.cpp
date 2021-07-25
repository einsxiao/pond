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

static INIT_SYMID_OF( INF );

__DEBUG_MAIN__("input.pd");

ArithmaticsModule::ArithmaticsModule():Module(MODULE_NAME){
  AddAttribute("INF",AttributeType::Protected );

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
    //RegisterFunction("Plus",Plus,this);
    AddAttribute("Plus",AttributeType::Flat);
    AddAttribute("Plus",AttributeType::Orderless);
    AddAttribute("Plus",AttributeType::Listable);
    AddAttribute("Plus",AttributeType::OneIdentity);
  }
  {
    AddAttribute("Times",AttributeType::Flat);
    AddAttribute("Times",AttributeType::Orderless);
    AddAttribute("Times",AttributeType::Listable);
    AddAttribute("Times",AttributeType::OneIdentity);
  }
  {
    AddAttribute("Power",AttributeType::Listable);
  }
  {
    AddAttribute("Log",AttributeType::Listable);
    RegisterFunction("ln",(MemberFunction)&ArithmaticsModule::PD_Log,this);
    AddAttribute("ln",AttributeType::Listable);
  }
  ///////////////////////////////
  {
    AddAttribute("AddTo",AttributeType::HoldFirst);
  }
  {
    AddAttribute("SubtractFrom",AttributeType::HoldFirst);
  }
  {
    AddAttribute("TimesBy",AttributeType::HoldFirst);
  }
  {
    AddAttribute("DivideBy",AttributeType::HoldFirst);
  }
  {
    AddAttribute("PowerWith",AttributeType::HoldFirst);
  }
  ///////////////////////////////////////////////////////////////////////
  {
    AddAttribute("Increment",AttributeType::HoldFirst);
  }
  {
    AddAttribute("PreIncrement",AttributeType::HoldFirst);
  }
  {
    AddAttribute("Decrement",AttributeType::HoldFirst);
  }
  {
    AddAttribute("PreDecrement",AttributeType::HoldFirst);
  }
  ///////////////////////////////////////////////////////////
  {

    RegisterFunction("大于",(MemberFunction)&ArithmaticsModule::PD_Greater,this);
    RegisterFunction("大于等于",(MemberFunction)&ArithmaticsModule::PD_GreaterEqual,this);
    RegisterFunction("小于",(MemberFunction)&ArithmaticsModule::PD_Less,this);
    RegisterFunction("小于等于",(MemberFunction)&ArithmaticsModule::PD_LessEqual,this);
    RegisterFunction("等于",(MemberFunction)&ArithmaticsModule::PD_Equal,this);
    RegisterFunction("不等于",(MemberFunction)&ArithmaticsModule::PD_UnEqual,this);
  }
  ///////////////////////////////////////////////////////////
  {
    AddAttribute("And",AttributeType::HoldAll);
    AddAttribute("And",AttributeType::Flat);
    AddAttribute("And",AttributeType::OneIdentity);

    RegisterFunction("and",(MemberFunction)&ArithmaticsModule::PD_And,this);
    AddAttribute("and",AttributeType::HoldAll);
    AddAttribute("and",AttributeType::Flat);
    AddAttribute("and",AttributeType::OneIdentity);

    RegisterFunction("且",(MemberFunction)&ArithmaticsModule::PD_And,this);
    AddAttribute("且",AttributeType::HoldAll);
    AddAttribute("且",AttributeType::Flat);
    AddAttribute("且",AttributeType::OneIdentity);
  }
  {
    //RegisterFunction("Or",(MemberFunction)&ArithmaticsModule::PD_Or,this);
    AddAttribute("Or",AttributeType::HoldAll);
    AddAttribute("Or",AttributeType::Flat);
    AddAttribute("Or",AttributeType::OneIdentity);

    //RegisterFunction("or",Or,this);
    AddAttribute("or",AttributeType::HoldAll);
    AddAttribute("or",AttributeType::Flat);
    AddAttribute("or",AttributeType::OneIdentity);

    RegisterFunction("或",(MemberFunction)&ArithmaticsModule::PD_Or,this);
    AddAttribute("或",AttributeType::HoldAll);
    AddAttribute("或",AttributeType::Flat);
    AddAttribute("或",AttributeType::OneIdentity);
  }
  {
    //RegisterFunction("Not",Not,this);
    RegisterFunction("not",(MemberFunction)&ArithmaticsModule::PD_Not,this);
    RegisterFunction("非",(MemberFunction)&ArithmaticsModule::PD_Not,this);
  }
  ///////////////////////////////////////////////////////////
  {
    //RegisterFunction("Abs",Abs,this);
    AddAttribute("Abs",AttributeType::Listable);
  }
  // RegisterFunction("Factorial",Factorial,this);
  // normal member functions
  //RegisterFunction("Random",Random,this,"give a uniform random number in the range of (0,1).");
  RegisterFunction("rand",(MemberFunction)&ArithmaticsModule::PD_Random,this,"give a uniform random number in the range of (0,1).");
  RegisterFunction("随机数",(MemberFunction)&ArithmaticsModule::PD_Random,this,"给出 (0,1) 范围的平均分布的随机数.");
  srand( (unsigned)time(NULL) );
};

ArithmaticsModule::~ArithmaticsModule(){};

int ArithmaticsModule::PD_Plus(Object &argv){
  //dout<<"  step into Plus"<<endl;
  // ReturnIfLessThan(1);
  argv.FlattenListAndSequence();
  // if ( argv.Size() == 1 ){
  //   argv = argv[1];
  //   ReturnHold;
  // }
  double sum = 0;
  u_int p = argv.Size();
  while ( p >=1 ) {
    if ( argv[p].RealQ() ){
      sum+=(double)argv[p];
      argv.Delete(p);
    }
    p--;
  }
  //dout<<"  treat all elements"<<endl;
  //cout<<argv<<endl;
  if ( argv.Size() == 0 ){
    ReturnNumber( sum );
  }
  if ( sum != 0.0 ){ //!=0
    argv.InsertNumber(1,sum);
  }
  argv.ExpressionSort();
  ReturnHold;
}

int ArithmaticsModule::PD_Times(Object &argv){
  CheckShouldNoLessThan(1);
  argv.FlattenListAndSequence();
  double res=1;
  u_int p = argv.Size();
  while ( p >=1 ) {
    //EvaKernel->Evaluate(argv[p]);
    if ( argv[p].RealQ() ){
      res*=(double)argv[p];
      argv.Delete(p);
    }
    p--;
  }
  //cout<<argv<<endl;
  if ( res == 0 or argv.Size() == 0 ){
    ReturnNumber( res );
  }
  if ( res != 1.0 ){ //!=0
    argv.InsertNumber(1,res);
    ReturnHold;
  }
  ReturnHold;
}

int ArithmaticsModule::PD_Power(Object &argv){
  CheckShouldEqual(2);
  if ( argv[2].RealQ() ){
    double index = (double) argv[2];
    if ( index == 0 )
      ReturnNumber(1);
    if ( index == 1 ){
      argv = argv[1];
      ReturnNormal;
    }
    if ( argv[1].RealQ() )
      ReturnNumber( pow((double)argv[1],index ) );
  }
  ReturnHold;
}

int ArithmaticsModule::PD_Log(Object &argv){
  CheckShouldBeWithin(1,2);
  if ( not argv[1].RealQ() ) ReturnNormal;
  if ( argv.Size() == 1 ){
    double x = (double)argv[1];
    ReturnNumber( log(x) );
  }
  if ( not argv[2].RealQ() ) ReturnNormal;
  ReturnNumber( log((double)argv[2])/log((double)argv[1]) );
}

// int ArithmaticsModule::PD_Complex(Object &argv){
//   CheckShouldEqual(2);
//   //EvaKernel->Evaluate( argv[1] );
//   //EvaKernel->Evaluate( argv[2] );
//   if ( argv[1].RealQ() && argv[2].RealQ() ){
//     ReturnNormal;
//   }
//   ReturnHold;
// }

int ArithmaticsModule::PD_AddTo(Object &argv){
  CheckShouldEqual(2);
  Object &var = argv[1];
  Object &obj = argv[2];
  if ( var.SymbolQ() ){
    Object value = EvaKernel->GetValuePairRef( var );
    if ( not value.NullQ() ){
      //EvaKernel->Evaluate( obj );
      if ( value[2].NumberQ() && obj.NumberQ() ){
        value[2].SetNumber( (double)value[2]+(double)obj );
        argv.CopyObject(value[2]);
        ReturnNormal;
      }
    }else{
      Erroring(argv.Key(),argv[1].ToString()+" is not a variable with a value.");
      ReturnError;
    }
  }
  //change to form of x = x + 1
  Object newS; newS.SetList( SYMID_OF_Set );
  Object newR; newR.SetList( SYMID_OF_Plus );
  newR.PushBackCopy( var ); newR.PushBackRef( obj );
  newS.PushBackRef( var ); newS.PushBackRef( newR );
  argv = newS;
  return EvaKernel->Evaluate( argv );
}

int ArithmaticsModule::PD_SubtractFrom(Object &argv){
  CheckShouldEqual(2);
  Object &var = argv[1];
  Object &obj = argv[2];
  if ( var.SymbolQ() ){
    Object value = EvaKernel->GetValuePairRef( var );
    if ( not value.NullQ() ){
      //EvaKernel->Evaluate( obj );
      if ( value[2].NumberQ() && obj.NumberQ() ){
        value[2].SetNumber( (double)value[2]-(double)obj );
        argv.CopyObject(value[2]);
        ReturnNormal;
      }
    }else{
      Erroring(argv.Key(),argv[1].ToString()+" is not a variable with a value.");
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
  argv = newS;
  return EvaKernel->Evaluate( argv );
}

int ArithmaticsModule::PD_TimesBy(Object &argv){
  CheckShouldEqual(2);
  Object &var = argv[1];
  Object &obj = argv[2];
  if ( var.SymbolQ() ){
    Object value = EvaKernel->GetValuePairRef( var );
    if ( not value.NullQ() ){
      //EvaKernel->Evaluate( obj );
      if ( value[2].NumberQ() && obj.NumberQ() ){
        value[2].SetNumber( (double)value[2]*(double)obj );
        argv.CopyObject(value[2]);
        ReturnNormal;
      }
    }else{
      Erroring(argv.Key(),argv[1].ToString()+" is not a variable with a value.");
      ReturnError;
    }
  }
  //change to form of x = x * a
  Object newS; newS.SetList( SYMID_OF_Set );
  Object newR; newR.SetList( SYMID_OF_Times );
  newR.PushBackCopy( var ); newR.PushBackRef( obj );
  newS.PushBackRef( var ); newS.PushBackRef( newR );
  argv = newS;
  return EvaKernel->Evaluate( argv );
}

int ArithmaticsModule::PD_DivideBy(Object &argv){
  CheckShouldEqual(2);
  Object &var = argv[1];
  Object &obj = argv[2];
  if ( var.SymbolQ() ){
    Object value = EvaKernel->GetValuePairRef( var );
    if ( not value.NullQ() ){
      //EvaKernel->Evaluate( obj );
      if ( value[2].NumberQ() && obj.NumberQ() ){
        value[2].SetNumber( (double)value[2]/(double)obj );
        argv.CopyObject(value[2]);
        ReturnNormal;
      }
    }else{
      Erroring(argv.Key(),argv[1].ToString()+" is not a variable with a value.");
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
  argv = newS;
  return EvaKernel->Evaluate( argv );
}

int ArithmaticsModule::PD_PowerWith(Object &argv){
  CheckShouldEqual(2);
  Object &var = argv[1];
  Object &obj = argv[2];
  if ( var.SymbolQ() ){
    Object value = EvaKernel->GetValuePairRef( var );
    if ( not value.NullQ() ){
      //EvaKernel->Evaluate( obj );
      if ( value[2].RealQ() && obj.RealQ() ){
        value[2].SetNumber( pow( (double)value[2],(double)obj ) );
        argv.CopyObject(value[2]);
        ReturnNormal;
      }
    }else{
      Erroring(argv.Key(),argv[1].ToString()+" is not a variable with a value.");
      ReturnError;
    }
  }
  //change to form of x = x ^ a
  Object newS; newS.SetList( SYMID_OF_Set );
  Object newR; newR.SetList( SYMID_OF_Power );
  newR.PushBackCopy( var ); newR.PushBackRef( obj );
  newS.PushBackRef( var ); newS.PushBackRef( newR );
  argv = newS;
  return EvaKernel->Evaluate( argv );
}



int ArithmaticsModule::PD_Increment(Object &argv){
  CheckShouldEqual(1);
  Object&var = argv[1];
  if ( var.SymbolQ() ){
    Object value = EvaKernel->GetValuePairRef( var );
    if ( not value.NullQ() ){
      if ( value[2].NumberQ() ){
        argv.CopyObject( value[2] );
        value[2].SetNumber( (double)value[2]+1.0 );
        ReturnNormal;
      }
    }else{
      Erroring(argv.Key(),argv[1].ToString()+" is not a variable with a value.");
      ReturnError;
    }
  }
  //change to form of x = x + 1 
  Object newS; newS.SetList( SYMID_OF_Set );
  Object newR; newR.SetList( SYMID_OF_Plus );
  newR.PushBackRef( var ); newR.PushBackNumber( 1.0 );
  newS.PushBackCopy( var ); newS.PushBackRef( newR );
  //EvaKernel->Evaluate(var);
  argv = argv[ 1 ] ;
  EvaKernel->Evaluate( newS );
  ReturnNormal;
 };

int ArithmaticsModule::PD_Decrement(Object &argv){
  CheckShouldEqual(1);
  Object&var = argv[1];
  if ( var.SymbolQ() ){
    Object value = EvaKernel->GetValuePairRef( var );
    if ( not value.NullQ() ){
      if ( value[2].NumberQ() ){
        argv.CopyObject( value[2] );
        value[2].SetNumber( (double)value[2]-1.0 );
        ReturnNormal;
      }
    }else{
      Erroring(argv.Key(),argv[1].ToString()+" is not a variable with a value.");
      ReturnError;
    }
  }
  //change to form of x = x + 1 
  Object newS; newS.SetList( SYMID_OF_Set );
  Object newR; newR.SetList( SYMID_OF_Plus );
  newR.PushBackRef( var ); newR.PushBackNumber( -1.0 );
  newS.PushBackCopy( var ); newS.PushBackRef( newR );
  //EvaKernel->Evaluate(var);
  argv = argv[ 1 ] ;
  EvaKernel->Evaluate( newS );
  ReturnNormal;
 };

int ArithmaticsModule::PD_PreIncrement(Object &argv){
  CheckShouldEqual(1);
  Object &var = argv[1];
  if ( var.SymbolQ() ){
    Object value = EvaKernel->GetValuePairRef( var );
    if ( not value.NullQ() ){
      if ( value[2].NumberQ() ){
        value[2].SetNumber( (double)value[2]+1.0 );
        argv.CopyObject( value[2] );
        ReturnNormal;
      }
    }else{
      Erroring(argv.Key(),argv[1].ToString()+" is not a variable with a value.");
      ReturnError;
    }
  }
  //change to form of x = x ^ a
  Object newS; newS.SetList( SYMID_OF_Set );
  Object newR; newR.SetList( SYMID_OF_Plus );
  newR.PushBackRef( var ); newR.PushBackNumber( 1.0 );
  newS.PushBackCopy( var ); newS.PushBackRef( newR );
  argv = newS;
  return EvaKernel->Evaluate( argv );
};


int ArithmaticsModule::PD_PreDecrement(Object &argv){
  CheckShouldEqual(1);
  Object &var = argv[1];
  if ( var.SymbolQ() ){
    Object value = EvaKernel->GetValuePairRef( var );
    if ( not value.NullQ() ){
      if ( value[2].NumberQ() ){
        value[2].SetNumber( (double)value[2]-1.0 );
        argv.CopyObject( value[2] );
        ReturnNormal;
      }
    }else{
      Erroring(argv.Key(),argv[1].ToString()+" is not a variable with a value.");
      ReturnError;
    }
  }
  //change to form of x = x ^ a
  Object newS; newS.SetList( SYMID_OF_Set );
  Object newR; newR.SetList( SYMID_OF_Plus );
  newR.PushBackRef( var ); newR.PushBackNumber( -1.0 );
  newS.PushBackCopy( var ); newS.PushBackRef( newR );
  argv = newS;
  return EvaKernel->Evaluate( argv );
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
    Object newleft;  newleft = left; argv.Delete(1);          \
    Object newright; newright.SetList( argv[0].ids() );       \
    newright.PushBackCopy( newleft[2] );                      \
    newright.PushBackRef( argv[1] );                          \
    argv.SetList( pond::SYMID_OF_And );                       \
    argv.PushBackRef( newleft ); argv.PushBackRef(newright);  \
    EvaKernel->Evaluate(argv);                                \
    ReturnNormal;                                             \
  }                                                           \

int ArithmaticsModule::PD_Greater(Object &argv){
  CheckShouldEqual(2);
  Object&left  = argv(1);
  Object&right = argv(2);
  __pre_treatment;
  //EvaKernel->Evaluate( left );
  //EvaKernel->Evaluate( right );
  if ( left.RealQ() && right.RealQ() ){
    ReturnBoolean( (double)left> (double)right );
    ReturnNormal;
  }
  ReturnHold;
}

int ArithmaticsModule::PD_GreaterEqual(Object &argv){
  CheckShouldEqual(2);
  Object&left = argv(1);
  Object&right= argv(2);
  __pre_treatment;
  //EvaKernel->Evaluate( left );
  //EvaKernel->Evaluate( right );
  if ( left.RealQ() && right.RealQ() ){
    ReturnBoolean(  (double)left>=double(right) );
  }
  ReturnHold;
}

int ArithmaticsModule::PD_Less(Object &argv){
  CheckShouldEqual(2);
  Object&left = argv(1);
  Object&right= argv(2);
  __pre_treatment;
  //EvaKernel->Evaluate( left );
  //EvaKernel->Evaluate( right );
  if ( left.RealQ() && right.RealQ() ){
    ReturnBoolean( double(left)<double(right) );
  }
  ReturnHold;
}

int ArithmaticsModule::PD_LessEqual(Object &argv){
  CheckShouldEqual(2);
  Object&left = argv(1);
  Object&right= argv(2);
  __pre_treatment;
  // EvaKernel->Evaluate( left );
  // EvaKernel->Evaluate( right );
  if ( left.RealQ() && right.RealQ() ){
    ReturnBoolean( double(left) <= double(right) );
  }
  ReturnHold;
}

int ArithmaticsModule::PD_Equal(Object &argv){
  CheckShouldEqual(2);
  Object&left = argv(1);
  Object&right= argv(2);
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
int ArithmaticsModule::PD_UnEqual(Object &argv){
  CheckShouldEqual(2);
  Object&left = argv(1);
  Object&right= argv(2);
  __pre_treatment;
  // EvaKernel->Evaluate( left );
  // EvaKernel->Evaluate( right );
  if ( left.NumberQ() && right.NumberQ() ){
    argv.SetBoolean( double(left) != double(right) );
    ReturnNormal;
  }
  ReturnHold;
}

#undef __pre_treatment

int ArithmaticsModule::PD_And(Object &argv){
  CheckShouldNoLessThan(2);
  bool unkonwn = false;
  for ( u_int i = 1; i<=argv.Size(); i++ ){
    EvaKernel->Evaluate( argv[i] );
    if ( argv[i].Boolean() == 0 ){ //false
      ReturnBoolean(false);
    }else if ( argv[i].Boolean() < 0 ){
      unkonwn = true;
    }
  }
  if ( unkonwn )
    ReturnHold;
  ReturnBoolean(true);
}

int ArithmaticsModule::PD_Or(Object &argv){
  CheckShouldNoLessThan(2);
  int unknownExpr= false;
  for ( u_int i = 1; i<=argv.Size(); i++ ){
    EvaKernel->Evaluate( argv[i] );
    if ( argv[i].Boolean() > 0 ){
      ReturnBoolean(true);
    }else if ( argv[i].Boolean()< 0 ){
      unknownExpr = true;
    }
  }
  if ( unknownExpr )
    ReturnHold;
  ReturnBoolean(false);
}

int ArithmaticsModule::PD_Not(Object &argv){
  CheckShouldEqual(1);
  // EvaKernel->Evaluate( argv[1] );
  if ( argv[1].Boolean() > 0 ){
    ReturnBoolean( false );
  }else if ( argv[1].Boolean() == 0 ){
    ReturnBoolean(true);
  }
  ReturnHold;
}

// int ArithmaticsModule::PD_Factorial(Object &argv){
//   CheckShouldEqual(1);
//   if ( not argv[1].RealQ() ) ReturnHold ;
//   long res=1, num = argv[1].Number();
//   for (int i = 2 ;i <=num; i++)
//     res*=i;
//   argv.SetNumber((double)res);
//   ReturnNormal ;
// }

int ArithmaticsModule::PD_Abs(Object &argv){
  CheckShouldEqual(1);
  if ( argv[1].RealQ() ){
    ReturnNumber( abs((double)argv[1]) );
  }
  ReturnHold;
}

int ArithmaticsModule::PD_SetRandomSeed(Object &argv){
  CheckShouldEqual(1);
  ReturnIfNotNumber(1);
  srand( (int)argv[1] );
  ReturnNumber(  (double)rand()/RAND_MAX );
}

int ArithmaticsModule::PD_Random(Object &argv){
  ReturnNumber((double)rand()/RAND_MAX);
}

int ArithmaticsModule::PD_Sin(Object&argv){
  CheckShouldEqual(1);
  if ( argv[1].RealQ() ){
    ReturnNumber( sin((double)argv[1]) );
  }
  ReturnHold;
}

int ArithmaticsModule::PD_Cos(Object&argv){
  CheckShouldEqual(1);
  if ( argv[1].RealQ() ){
    ReturnNumber( cos((double)argv[1]) );
  }
  ReturnNormal;
}

int ArithmaticsModule::PD_Tan(Object&argv){
  CheckShouldEqual(1);
  if ( argv[1].RealQ() ){
    ReturnNumber( tan((double)argv[1]) );
  }
  ReturnHold;
}

int ArithmaticsModule::PD_Ln(Object&argv){
  CheckShouldEqual(1);
  if ( not argv[1].RealQ() ) ReturnHold;
  double x = (double)argv[1];
  ReturnNumber( log(x) );
}

int ArithmaticsModule::PD_Mod(Object&argv){
  /*
    ArgvN: 2
    Attributes: Listable 
    求x/y的余数
   */
  CheckShouldEqual(2);
  if ( not argv[1].RealQ() or not argv[2].RealQ() ) ReturnHold;
  double x = argv[1].Number(); 
  double y = argv[2].Number();
  ReturnNumber( x - long(x/y)*y );
}

int ArithmaticsModule::PD_Floor(Object&argv){
  /*Attributes: Listable 
    ArgvN: 1
    对实数下取整
   */
  if ( not argv[1].RealQ() ) ReturnHold;
  double x = (double)argv[1]; 
  ReturnNumber( floor(x) );
}

int ArithmaticsModule::PD_Ceil(Object&argv){
  /*Attributes: Listable 
    ArgvN: 1
    对实数上取整
   */
  if ( not argv[1].RealQ() ) ReturnHold;
  double x = (double)argv[1]; 
  ReturnNumber( ceil(x) );
}

int ArithmaticsModule::PD_Round(Object&argv){
  /*Attributes: Listable 
    ArgvN: 1
    对实数四舍五入取整
   */
  if ( not argv[1].RealQ() ) ReturnHold;
  double x = (double)argv[1]; 
  ReturnNumber( round(x) );
}

int ArithmaticsModule::PD_Sqrt(Object&argv){
  /*Attributes: Listable 
    ArgvN: 1
    对实数四舍五入取整
   */
  if ( not argv[1].RealQ() ) ReturnHold;
  ReturnNumber( sqrt((double)argv[1]) );
}

int ArithmaticsModule::PD_Max(Object&argv){
  /*Attributes: Listable 
    ArgvN: 1
    对实数四舍五入取整
   */
  bool find = false;
  double v;
  for (auto iter = argv.Begin(); iter != argv.End(); iter++){
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

int ArithmaticsModule::PD_Min(Object&argv){
  /*Attributes: Listable 
    ArgvN: 1
    对实数四舍五入取整
   */
  bool find = false;
  double v;
  for (auto iter = argv.Begin(); iter != argv.End(); iter++){
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


int ArithmaticsModule::PD_RealQ(Object&argv){
  CheckShouldEqual(1);
  ReturnBoolean( argv[1].RealQ() );
}

int ArithmaticsModule::PD_IntegerQ(Object&argv){
  CheckShouldEqual(1);
  ReturnBoolean( argv[1].IntegerQ() );
}

int ArithmaticsModule::PD_EvenQ(Object&argv){
  CheckShouldEqual(1);
  double x = argv[1].Number();
  if ( x != round(x) ) ReturnBoolean(false);
  if ( ((long)x)%2 == 0 ) ReturnBoolean( true );
  ReturnBoolean(false);
}

int ArithmaticsModule::PD_OddQ(Object&argv){
  CheckShouldEqual(1);
  double x = argv[1].Number();
  if ( x != round(x) ) ReturnBoolean(false);
  if ( ((long)x)%2 == 0 ) ReturnBoolean( false );
  ReturnBoolean( true );
}
