#define ModuleName Arithmatics
#include<pond>
#include<math.h>
#include"ArithmaticsModule.h"
using namespace std;
using namespace pond;

static INIT_OPER_SYMID_OF(且,                    170,169,   3,1);
static INIT_OPER_SYMID_OF(或,                    175,174,   3,1);
static INIT_OPER_SYMID_OF(非,                      0,168,   1,1);
static INIT_OPER_SYMID_OF(大于,                  130,129,   3,1);
static INIT_OPER_SYMID_OF(大于等于,              130,129,   3,1);
static INIT_OPER_SYMID_OF(小于,                  130,129,   3,1);
static INIT_OPER_SYMID_OF(小于等于,              130,129,   3,1);
static INIT_OPER_SYMID_OF(等于,                  130,129,   3,1);
static INIT_OPER_SYMID_OF(不等于,                130,129,   3,1); 


ModuleConstruction{
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

ModuleDestruction{};

int ArithmaticsModule::Plus(Object &Argv){
  //dout<<"  step into Plus"<<endl;
  // ReturnIfLessThan(1);
  Argv.FlattenListAndSequence();
  // if ( Argv.Size() == 1 ){
  //   Argv = Argv[1];
  //   ReturnHold;
  // }
  if ( Argv.Size() > 0 && Argv[1].StringQ() ){
    INIT_SYMID_OF( ToString );
    Argv[0].SetSymbol(SYMID_OF_ToString);
    return EvaKernel->Evaluate(Argv);

  }
  double sum = 0;
  u_int p = Argv.Size();
  while ( p >=1 ) {
    if ( Argv[p].RealQ() ){
      sum+=(double)Argv[p];
      Argv.Delete(p);
    }
    p--;
  }
  //dout<<"  treat all elements"<<endl;
  // cout<<Argv<<endl;
  if ( Argv.Size() == 0 ){
    ReturnNumber( sum );
  }
  if ( sum != 0.0 ){ //!=0
    Argv.InsertNumber(1,sum);
  }
  if ( Argv.Size() >= 2 and ( Argv[1].StringQ() or Argv[2].StringQ() ) ){
    string res;
    if ( Argv[1].StringQ() ) res += Argv[1].Key();
    else res += Argv[1].ToString();

    if ( Argv.Size() == 2 ){
      Argv.SetString( res );
      ReturnHold;
    }else{
      Argv[1].SetString( res );
      Argv.Delete(2);
      return EvaKernel->Evaluate( Argv );
    }
    ReturnHold;
  }
  Argv.ExpressionSort();
  ReturnHold;
}

int ArithmaticsModule::Times(Object &Argv){
  CheckShouldNoLessThan(1);
  Argv.FlattenListAndSequence();
  // if ( Argv.Size() == 1 ){
  //   Argv = Argv[1];
  //   ReturnHold;
  // }
  //dprintf("Calculate %s",Argv.ToString().c_str() );
  double res=1;
  u_int p = Argv.Size();
  while ( p >=1 ) {
    //EvaKernel->Evaluate(Argv[p]);
    if ( Argv[p].RealQ() ){
      res*=(double)Argv[p];
      Argv.Delete(p);
    }
    p--;
  }
  // cout<<Argv<<endl;
  if ( res == 0 or Argv.Size() == 0 ){
    ReturnNumber( res );
  }
  if ( res != 1.0 ){ //!=0
    Argv.InsertNumber(1,res);
    ReturnHold;
  }
  ReturnHold;
}

int ArithmaticsModule::Power(Object &Argv){
  CheckShouldEqual(2);
  if ( Argv[2].RealQ() ){
    double index = (double) Argv[2];
    if ( index == 0 )
      ReturnNumber(1);
    if ( index == 1 ){
      Argv = Argv[1];
      ReturnNormal;
    }
    if ( Argv[1].RealQ() )
      ReturnNumber( pow((double)Argv[1],index ) );
  }
  ReturnHold;
}

int ArithmaticsModule::Log(Object &Argv){
  CheckShouldBeWithin(1,2);
  if ( not Argv[1].RealQ() ) ReturnNormal;
  if ( Argv.Size() == 1 ){
    double x = (double)Argv[1];
    ReturnNumber( log(x) );
  }
  if ( not Argv[2].RealQ() ) ReturnNormal;
  ReturnNumber( log((double)Argv[2])/log((double)Argv[1]) );
}

// int ArithmaticsModule::Complex(Object &Argv){
//   CheckShouldEqual(2);
//   //EvaKernel->Evaluate( Argv[1] );
//   //EvaKernel->Evaluate( Argv[2] );
//   if ( Argv[1].RealQ() && Argv[2].RealQ() ){
//     ReturnNormal;
//   }
//   ReturnHold;
// }

int ArithmaticsModule::AddTo(Object &Argv){
  CheckShouldEqual(2);
  Object &var = Argv[1];
  Object &obj = Argv[2];
  if ( var.SymbolQ() ){
    Object value = EvaKernel->GetValuePairRef( var );
    if ( not value.NullQ() ){
      //EvaKernel->Evaluate( obj );
      if ( value[2].NumberQ() && obj.NumberQ() ){
        value[2].SetNumber( (double)value[2]+(double)obj );
        Argv.CopyObject(value[2]);
        ReturnNormal;
      }
    }else{
      Erroring(Argv.Key(),Argv[1].ToString()+" is not a variable with a value.");
      ReturnError;
    }
  }
  //change to form of x = x + 1
  Object newS; newS.SetList( SYMID_OF_Set );
  Object newR; newR.SetList( SYMID_OF_Plus );
  newR.PushBackCopy( var ); newR.PushBackRef( obj );
  newS.PushBackRef( var ); newS.PushBackRef( newR );
  Argv = newS;
  return EvaKernel->Evaluate( Argv );
}

int ArithmaticsModule::SubtractFrom(Object &Argv){
  CheckShouldEqual(2);
  Object &var = Argv[1];
  Object &obj = Argv[2];
  if ( var.SymbolQ() ){
    Object value = EvaKernel->GetValuePairRef( var );
    if ( not value.NullQ() ){
      //EvaKernel->Evaluate( obj );
      if ( value[2].NumberQ() && obj.NumberQ() ){
        value[2].SetNumber( (double)value[2]-(double)obj );
        Argv.CopyObject(value[2]);
        ReturnNormal;
      }
    }else{
      Erroring(Argv.Key(),Argv[1].ToString()+" is not a variable with a value.");
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
  Argv = newS;
  return EvaKernel->Evaluate( Argv );
}

int ArithmaticsModule::TimesBy(Object &Argv){
  CheckShouldEqual(2);
  Object &var = Argv[1];
  Object &obj = Argv[2];
  if ( var.SymbolQ() ){
    Object value = EvaKernel->GetValuePairRef( var );
    if ( not value.NullQ() ){
      //EvaKernel->Evaluate( obj );
      if ( value[2].NumberQ() && obj.NumberQ() ){
        value[2].SetNumber( (double)value[2]*(double)obj );
        Argv.CopyObject(value[2]);
        ReturnNormal;
      }
    }else{
      Erroring(Argv.Key(),Argv[1].ToString()+" is not a variable with a value.");
      ReturnError;
    }
  }
  //change to form of x = x * a
  Object newS; newS.SetList( SYMID_OF_Set );
  Object newR; newR.SetList( SYMID_OF_Times );
  newR.PushBackCopy( var ); newR.PushBackRef( obj );
  newS.PushBackRef( var ); newS.PushBackRef( newR );
  Argv = newS;
  return EvaKernel->Evaluate( Argv );
}

int ArithmaticsModule::DivideBy(Object &Argv){
  CheckShouldEqual(2);
  Object &var = Argv[1];
  Object &obj = Argv[2];
  if ( var.SymbolQ() ){
    Object value = EvaKernel->GetValuePairRef( var );
    if ( not value.NullQ() ){
      //EvaKernel->Evaluate( obj );
      if ( value[2].NumberQ() && obj.NumberQ() ){
        value[2].SetNumber( (double)value[2]/(double)obj );
        Argv.CopyObject(value[2]);
        ReturnNormal;
      }
    }else{
      Erroring(Argv.Key(),Argv[1].ToString()+" is not a variable with a value.");
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
  Argv = newS;
  return EvaKernel->Evaluate( Argv );
}

int ArithmaticsModule::PowerWith(Object &Argv){
  CheckShouldEqual(2);
  Object &var = Argv[1];
  Object &obj = Argv[2];
  if ( var.SymbolQ() ){
    Object value = EvaKernel->GetValuePairRef( var );
    if ( not value.NullQ() ){
      //EvaKernel->Evaluate( obj );
      if ( value[2].RealQ() && obj.RealQ() ){
        value[2].SetNumber( pow( (double)value[2],(double)obj ) );
        Argv.CopyObject(value[2]);
        ReturnNormal;
      }
    }else{
      Erroring(Argv.Key(),Argv[1].ToString()+" is not a variable with a value.");
      ReturnError;
    }
  }
  //change to form of x = x ^ a
  Object newS; newS.SetList( SYMID_OF_Set );
  Object newR; newR.SetList( SYMID_OF_Power );
  newR.PushBackCopy( var ); newR.PushBackRef( obj );
  newS.PushBackRef( var ); newS.PushBackRef( newR );
  Argv = newS;
  return EvaKernel->Evaluate( Argv );
}



int ArithmaticsModule::Increment(Object &Argv){
  CheckShouldEqual(1);
  Object&var = Argv[1];
  if ( var.SymbolQ() ){
    Object value = EvaKernel->GetValuePairRef( var );
    if ( not value.NullQ() ){
      if ( value[2].NumberQ() ){
        Argv.CopyObject( value[2] );
        value[2].SetNumber( (double)value[2]+1.0 );
        ReturnNormal;
      }
    }else{
      Erroring(Argv.Key(),Argv[1].ToString()+" is not a variable with a value.");
      ReturnError;
    }
  }
  //change to form of x = x + 1 
  Object newS; newS.SetList( SYMID_OF_Set );
  Object newR; newR.SetList( SYMID_OF_Plus );
  newR.PushBackRef( var ); newR.PushBackNumber( 1.0 );
  newS.PushBackCopy( var ); newS.PushBackRef( newR );
  //EvaKernel->Evaluate(var);
  Argv = Argv[ 1 ] ;
  EvaKernel->Evaluate( newS );
  ReturnNormal;
 };

int ArithmaticsModule::Decrement(Object &Argv){
  CheckShouldEqual(1);
  Object&var = Argv[1];
  if ( var.SymbolQ() ){
    Object value = EvaKernel->GetValuePairRef( var );
    if ( not value.NullQ() ){
      if ( value[2].NumberQ() ){
        Argv.CopyObject( value[2] );
        value[2].SetNumber( (double)value[2]-1.0 );
        ReturnNormal;
      }
    }else{
      Erroring(Argv.Key(),Argv[1].ToString()+" is not a variable with a value.");
      ReturnError;
    }
  }
  //change to form of x = x + 1 
  Object newS; newS.SetList( SYMID_OF_Set );
  Object newR; newR.SetList( SYMID_OF_Plus );
  newR.PushBackRef( var ); newR.PushBackNumber( -1.0 );
  newS.PushBackCopy( var ); newS.PushBackRef( newR );
  //EvaKernel->Evaluate(var);
  Argv = Argv[ 1 ] ;
  EvaKernel->Evaluate( newS );
  ReturnNormal;
 };

int ArithmaticsModule::PreIncrement(Object &Argv){
  CheckShouldEqual(1);
  Object &var = Argv[1];
  if ( var.SymbolQ() ){
    Object value = EvaKernel->GetValuePairRef( var );
    if ( not value.NullQ() ){
      if ( value[2].NumberQ() ){
        value[2].SetNumber( (double)value[2]+1.0 );
        Argv.CopyObject( value[2] );
        ReturnNormal;
      }
    }else{
      Erroring(Argv.Key(),Argv[1].ToString()+" is not a variable with a value.");
      ReturnError;
    }
  }
  //change to form of x = x ^ a
  Object newS; newS.SetList( SYMID_OF_Set );
  Object newR; newR.SetList( SYMID_OF_Plus );
  newR.PushBackRef( var ); newR.PushBackNumber( 1.0 );
  newS.PushBackCopy( var ); newS.PushBackRef( newR );
  Argv = newS;
  return EvaKernel->Evaluate( Argv );
};


int ArithmaticsModule::PreDecrement(Object &Argv){
  CheckShouldEqual(1);
  Object &var = Argv[1];
  if ( var.SymbolQ() ){
    Object value = EvaKernel->GetValuePairRef( var );
    if ( not value.NullQ() ){
      if ( value[2].NumberQ() ){
        value[2].SetNumber( (double)value[2]-1.0 );
        Argv.CopyObject( value[2] );
        ReturnNormal;
      }
    }else{
      Erroring(Argv.Key(),Argv[1].ToString()+" is not a variable with a value.");
      ReturnError;
    }
  }
  //change to form of x = x ^ a
  Object newS; newS.SetList( SYMID_OF_Set );
  Object newR; newR.SetList( SYMID_OF_Plus );
  newR.PushBackRef( var ); newR.PushBackNumber( -1.0 );
  newS.PushBackCopy( var ); newS.PushBackRef( newR );
  Argv = newS;
  return EvaKernel->Evaluate( Argv );
};

// is to deal with form like (a < b) < c or (a < b) >c
// will be transformed to    And[a<b,b<c]  And[a<b,b>c]
#define __pre_treatment                                       \
  if ( ( left.ListQ(SYMID_OF_Greater ) )                      \
       ||left.ListQ(SYMID_OF_大于 )                           \
       ||left.ListQ(SYMID_OF_GreaterEqual )                   \
       ||left.ListQ(SYMID_OF_大于等于 )                       \
       ||left.ListQ(SYMID_OF_Less )                           \
       ||left.ListQ(SYMID_OF_小于 )                           \
       ||left.ListQ(SYMID_OF_LessEqual )                      \
       ||left.ListQ(SYMID_OF_小于等于)                        \
       ||left.ListQ(SYMID_OF_Equal )                          \
       ||left.ListQ(SYMID_OF_等于 )                           \
       ||left.ListQ(SYMID_OF_UnEqual )                        \
       ||left.ListQ(SYMID_OF_不等于 ) ){                      \
    CheckArgsShouldEqual(left,2);                             \
    Object newleft;  newleft = left; Argv.Delete(1);          \
    Object newright; newright.SetList( Argv[0].ids() );       \
    newright.PushBackCopy( newleft[2] );                      \
    newright.PushBackRef( Argv[1] );                          \
    Argv.SetList( pond::SYMID_OF_And );                       \
    Argv.PushBackRef( newleft ); Argv.PushBackRef(newright);  \
    EvaKernel->Evaluate(Argv);                                \
    ReturnNormal;                                             \
  }                                                           \

int ArithmaticsModule::Greater(Object &Argv){
  CheckShouldEqual(2);
  Object&left  = Argv(1);
  Object&right = Argv(2);
  __pre_treatment;
  //EvaKernel->Evaluate( left );
  //EvaKernel->Evaluate( right );
  if ( left.RealQ() && right.RealQ() ){
    ReturnBoolean( (double)left> (double)right );
    ReturnNormal;
  }
  ReturnHold;
}

int ArithmaticsModule::GreaterEqual(Object &Argv){
  CheckShouldEqual(2);
  Object&left = Argv(1);
  Object&right= Argv(2);
  __pre_treatment;
  //EvaKernel->Evaluate( left );
  //EvaKernel->Evaluate( right );
  if ( left.RealQ() && right.RealQ() ){
    ReturnBoolean(  (double)left>=double(right) );
  }
  ReturnHold;
}

int ArithmaticsModule::Less(Object &Argv){
  CheckShouldEqual(2);
  Object&left = Argv(1);
  Object&right= Argv(2);
  __pre_treatment;
  //EvaKernel->Evaluate( left );
  //EvaKernel->Evaluate( right );
  if ( left.RealQ() && right.RealQ() ){
    ReturnBoolean( double(left)<double(right) );
  }
  ReturnHold;
}

int ArithmaticsModule::LessEqual(Object &Argv){
  CheckShouldEqual(2);
  Object&left = Argv(1);
  Object&right= Argv(2);
  __pre_treatment;
  // EvaKernel->Evaluate( left );
  // EvaKernel->Evaluate( right );
  if ( left.RealQ() && right.RealQ() ){
    ReturnBoolean( double(left) <= double(right) );
  }
  ReturnHold;
}

int ArithmaticsModule::Equal(Object &Argv){
  CheckShouldEqual(2);
  Object&left = Argv(1);
  Object&right= Argv(2);
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
int ArithmaticsModule::UnEqual(Object &Argv){
  CheckShouldEqual(2);
  Object&left = Argv(1);
  Object&right= Argv(2);
  __pre_treatment;
  // EvaKernel->Evaluate( left );
  // EvaKernel->Evaluate( right );
  if ( left.NumberQ() && right.NumberQ() ){
    Argv.SetBoolean( double(left) != double(right) );
    ReturnNormal;
  }
  ReturnHold;
}

#undef __pre_treatment

int ArithmaticsModule::And(Object &Argv){
  CheckShouldNoLessThan(2);
  bool unkonwn = false;
  for ( u_int i = 1; i<=Argv.Size(); i++ ){
    EvaKernel->Evaluate( Argv[i] );
    if ( Argv[i].Boolean() == 0 ){ //false
      ReturnBoolean(false);
    }else if ( Argv[i].Boolean() < 0 ){
      unkonwn = true;
    }
  }
  if ( unkonwn )
    ReturnHold;
  ReturnBoolean(true);
}

int ArithmaticsModule::Or(Object &Argv){
  CheckShouldNoLessThan(2);
  int unknownExpr= false;
  for ( u_int i = 1; i<=Argv.Size(); i++ ){
    EvaKernel->Evaluate( Argv[i] );
    if ( Argv[i].Boolean() > 0 ){
      ReturnBoolean(true);
    }else if ( Argv[i].Boolean()< 0 ){
      unknownExpr = true;
    }
  }
  if ( unknownExpr )
    ReturnHold;
  ReturnBoolean(false);
}

int ArithmaticsModule::Not(Object &Argv){
  CheckShouldEqual(1);
  // EvaKernel->Evaluate( Argv[1] );
  if ( Argv[1].Boolean() > 0 ){
    ReturnBoolean( false );
  }else if ( Argv[1].Boolean() == 0 ){
    ReturnBoolean(true);
  }
  ReturnHold;
}

// int ArithmaticsModule::Factorial(Object &Argv){
//   CheckShouldEqual(1);
//   if ( not Argv[1].RealQ() ) ReturnHold ;
//   long res=1, num = Argv[1].Number();
//   for (int i = 2 ;i <=num; i++)
//     res*=i;
//   Argv.SetNumber((double)res);
//   ReturnNormal ;
// }

int ArithmaticsModule::Abs(Object &Argv){
  CheckShouldEqual(1);
  if ( Argv[1].RealQ() ){
    ReturnNumber( abs((double)Argv[1]) );
  }
  ReturnHold;
}

int ArithmaticsModule::SetRandomSeed(Object &Argv){
  CheckShouldEqual(1);
  ReturnIfNotNumber(1);
  srand( (int)Argv[1] );
  ReturnNumber(  (double)rand()/RAND_MAX );
}

int ArithmaticsModule::Random(Object &Argv){
  ReturnNumber((double)rand()/RAND_MAX);
}

DefinePDFunction(Sin,Argv){
  CheckShouldEqual(1);
  if ( Argv[1].RealQ() ){
    ReturnNumber( sin((double)Argv[1]) );
  }
  ReturnHold;
}

DefinePDFunction(Cos,Argv){
  CheckShouldEqual(1);
  if ( Argv[1].RealQ() ){
    ReturnNumber( cos((double)Argv[1]) );
  }
  ReturnNormal;
}

DefinePDFunction(Tan,Argv){
  CheckShouldEqual(1);
  if ( Argv[1].RealQ() ){
    ReturnNumber( tan((double)Argv[1]) );
  }
  ReturnHold;
}

DefinePDFunction(Ln,Argv){
  CheckShouldEqual(1);
  if ( not Argv[1].RealQ() ) ReturnHold;
  double x = (double)Argv[1];
  ReturnNumber( log(x) );
}

DefinePDFunction(Mod,Argv){
  /*
    ArgvN: 2
    Attributes: Listable 
    求x/y的余数
   */
  CheckShouldEqual(2);
  if ( not Argv[1].RealQ() or not Argv[2].RealQ() ) ReturnHold;
  double x = Argv[1].Number(); 
  double y = Argv[2].Number();
  ReturnNumber( x - long(x/y)*y );
}

DefinePDFunction(Floor,Argv){
  /*Attributes: Listable 
    ArgvN: 1
    对实数下取整
   */
  if ( not Argv[1].RealQ() ) ReturnHold;
  double x = (double)Argv[1]; 
  ReturnNumber( floor(x) );
}

DefinePDFunction(Ceil,Argv){
  /*Attributes: Listable 
    ArgvN: 1
    对实数上取整
   */
  if ( not Argv[1].RealQ() ) ReturnHold;
  double x = (double)Argv[1]; 
  ReturnNumber( ceil(x) );
}

DefinePDFunction(Round,Argv){
  /*Attributes: Listable 
    ArgvN: 1
    对实数四舍五入取整
   */
  if ( not Argv[1].RealQ() ) ReturnHold;
  double x = (double)Argv[1]; 
  ReturnNumber( round(x) );
}

DefinePDFunction(Sqrt,Argv){
  /*Attributes: Listable 
    ArgvN: 1
    对实数四舍五入取整
   */
  if ( not Argv[1].RealQ() ) ReturnHold;
  ReturnNumber( sqrt((double)Argv[1]) );
}

DefinePDFunction(Max,Argv){
  /*Attributes: Listable 
    ArgvN: 1
    对实数四舍五入取整
   */
  bool find = false;
  double v;
  for (auto iter = Argv.Begin(); iter != Argv.End(); iter++){
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

DefinePDFunction(Min,Argv){
  /*Attributes: Listable 
    ArgvN: 1
    对实数四舍五入取整
   */
  bool find = false;
  double v;
  for (auto iter = Argv.Begin(); iter != Argv.End(); iter++){
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


DefinePDFunction(RealQ,Argv){
  CheckShouldEqual(1);
  ReturnBoolean( Argv[1].RealQ() );
}

DefinePDFunction(IntegerQ,Argv){
  CheckShouldEqual(1);
  ReturnBoolean( Argv[1].IntegerQ() );
}

DefinePDFunction(EvenQ,Argv){
  CheckShouldEqual(1);
  double x = Argv[1].Number();
  if ( x != round(x) ) ReturnBoolean(false);
  if ( ((long)x)%2 == 0 ) ReturnBoolean( true );
  ReturnBoolean(false);
}

DefinePDFunction(OddQ,Argv){
  CheckShouldEqual(1);
  double x = Argv[1].Number();
  if ( x != round(x) ) ReturnBoolean(false);
  if ( ((long)x)%2 == 0 ) ReturnBoolean( false );
  ReturnBoolean( true );
}
