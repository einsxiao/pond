//////////////////////////////////////////////
#include"pond.h"
#include"SystemModule.h"
#include"ListModule.h"
#include<sys/time.h>
//////////////////////////////////////////////
using namespace std;
using namespace pond;

static INIT_SYMID_OF( SimpleSimplify );
static INIT_SYMID_OF( Do );
static INIT_SYMID_OF( Attributes );
static INIT_SYMID_OF( FUNCTION$DEFINED$ );
// static INIT_OPER_SYMID_OF( 如果,        0,220,   1,2    );
// static INIT_OPER_SYMID_OF( 又如,      218,219,   3,2    );
// static INIT_OPER_SYMID_OF( 否则,      218,219,   3,1    );
// static INIT_OPER_SYMID_OF( 只要,        0,220,   1,2    );
// static INIT_OPER_SYMID_OF( 罗列,        0,220,   1,2    );
// static INIT_OPER_SYMID_OF( 于  ,      120,115,   3,1    );

static INIT_OPER_SYMID_OF( def ,        0,220,   1,2    );
static INIT_OPER_SYMID_OF( function ,   0,220,   1,2    );
// static INIT_OPER_SYMID_OF( 函数,        0,220,   1,2    );
// static INIT_OPER_SYMID_OF( 存于,      120,115,   3,1    );
static INIT_OPER_SYMID_OF( in  ,      120,115,   3,1    );
// static INIT_OPER_SYMID_OF( 为  ,      120,115,   3,1    );
static INIT_OPER_SYMID_OF( is  ,      120,115,   3,1    );
static INIT_OPER_SYMID_OF( do  ,        0,220,   1,2    );

static INIT_OPER_SYMID_OF( context,     0,220,   1,1    );
static INIT_OPER_SYMID_OF( ctx,         0,220,   1,1    );
    
__DEBUG_MAIN__("input.pd");

SystemModule::SystemModule():Module(MODULE_NAME){
  //dout<<"system module initialized"<<endl;

  ///////////////////////////////////////////////////////////
  // special form
#define AP(sym) EvaKernel->SetAttribute(#sym, AttributeType::Protected )
  AP(Null);
  AP(Flat);
  AP(Listable);
  AP(OneIdentity);
  AP(HoldFirst);
  AP(HoldRest);
  AP(HoldAll);
  AP(HoldAllCompleted);
  AP(SequenceHold);
  AP(Orderless);
  AP(GrammarUnit);
  AP(Constant);
  AP(DelayFunction);
  AP(DelayHold);
  AP(DelaySetable);

  AP(Conjunctable);
  AP(Setable);
  AP(Partable);
  AP(Destroyable);
  AP(Protected);
  AP(Locked);
  AP(MAXENUM);
#undef AP
  Object left,right;
  left.SetSymbol("I"); right.SetSymbol("I");
  EvaKernel->InsertOrUpdateBottomPairValue(left,right);
  AddAttribute("I",AttributeType::Protected );

  // systematic functions
  // {
  //   RegisterFunction("Evaluate",Evaluate,this);
  // }
  {
    RegisterFunction("Set",Set,this);
    AddAttribute("Set",AttributeType::HoldFirst);
  }
  {
    RegisterFunction("SetDelayed",          SetDelayed,this);
    AddAttribute("SetDelayed",              AttributeType::HoldAll);
  }
  {
    AddAttribute("context",  AttributeType::HoldAll );
    RegisterFunction( "ctx",MemberFunction(&SystemModule::PD_context), this);
    AddAttribute("ctx",  AttributeType::HoldAll );
  }
  {
    RegisterFunction("Parenthesis",         Parenthesis,this);
    AddAttribute("Parenthesis",  AttributeType::HoldAll );
    AddAttribute("Parenthesis",  AttributeType::OneIdentity );
  }
  {
    RegisterFunction("ExpressionList",      ExpressionList,this);
    AddAttribute("ExpressionList",  AttributeType::HoldAll );
  }
  {
    RegisterFunction("CompoundExpression",  CompoundExpression,this);
    AddAttribute("CompoundExpression",  AttributeType::HoldAll );
  }
  {
    RegisterFunction("Replace",             Replace,this);
    AddAttribute("Replace",  AttributeType::HoldAll );
  }
  {
    RegisterFunction("ReplaceAll",          ReplaceAll,this);
    AddAttribute("ReplaceAll",  AttributeType::HoldAll );
  }
  {
    RegisterFunction("ReplaceAllRepeated",  ReplaceAllRepeated,this);
    AddAttribute("ReplaceAllRepeated",  AttributeType::HoldAll );
  }
  {
    RegisterFunction("ShowModuleFunctions",ShowModuleFunctions,this);
    AddAttribute("ShowModuleFunctions",AttributeType::Listable);
  }
  {
    RegisterFunction("ShowFunctionsOfModule",ShowModuleFunctions,this);
    AddAttribute("ShowFunctionsOfModule",AttributeType::Listable);
  }
  {
    RegisterFunction("ShowModules",ShowModules,this);
  }
  ////////////////////////////////////////////////////////////////
  {
    // RegisterFunction("Options",Options,this);
  }
  {
    // RegisterFunction("Attributes",Attributes,this); 
    AddAttribute("Attributes",AttributeType::Listable);
  }
  {
    RegisterFunction("SetAttributes",SetAttribute_Eva,this);
    AddAttribute("SetAttributes",AttributeType::HoldAll);
    AddAttribute("Attributes",AttributeType::Setable);
  }
  {
    RegisterFunction("AddAttribute",AddAttribute_Eva,this);
    AddAttribute("AddAttribute",AttributeType::HoldAll);
  }
  ///////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////
  // pattern system
  {
    RegisterFunction("MatchQ",MatchQ,this);
  }
  {
    RegisterFunction("UnifyRule",UnifyRule,this);
  }
  {
    RegisterFunction("UnifiedMatchQ",UnifiedMatchQ,this);
  }
  ////////////////////////////////////////////////////////////
  {
    AddAttribute("Pattern",AttributeType::HoldFirst);
    AddAttribute("Pattern",AttributeType::Protected);
  }
  {
    AddAttribute("Rule",AttributeType::Protected);
    AddAttribute("RuleDelayed",AttributeType::HoldRest);
    AddAttribute("RuleDelayed",AttributeType::Protected);
    AddAttribute("KeyValuePair",AttributeType::HoldFirst);
    AddAttribute("KeyValuePair",AttributeType::Protected);
  }
  ////////////////////////////////////////////////////////////
  {
    // RegisterFunction("Hold",Hold,this);
    AddAttribute("Hold",AttributeType::HoldAll);
    AddAttribute("Hold",AttributeType::OneIdentity);
    AddAttribute("Hold",AttributeType::OneIdentity);

  }
  {
    RegisterFunction("HoldOnce",HoldOnce,this);
    AddAttribute("HoldOnce",AttributeType::HoldAll);
    AddAttribute("HoldOnce",AttributeType::OneIdentity);
  }
  {
    // RegisterFunction("HoldPattern",HoldPattern,this);
    AddAttribute("HoldPattern",AttributeType::HoldAll);
    AddAttribute("HoldPattern",AttributeType::OneIdentity);
  }
  ///////////////////////////////////////////////////////////
  {
    RegisterFunction("Module",GrammarModule,this);
    AddAttribute("Module",AttributeType::HoldAll);
  }
  {
    RegisterFunction("Function",Function,this);
    AddAttribute("Function",AttributeType::DelayFunction);
    AddAttribute("Function",AttributeType::HoldAll);

    RegisterFunction("PureFunction",PureFunction,this);
    AddAttribute("PureFunction",AttributeType::DelayFunction);
    AddAttribute("PureFunction",AttributeType::HoldAll);

    // AddAttribute("ArrowFunction",AttributeType::DelayFunction);
    AddAttribute("ArrowFunction",AttributeType::HoldAll); // working similar to def and function

    RegisterFunction( "function",MemberFunction(&SystemModule::PD_def), this);
    AddAttribute("def",AttributeType::HoldAll);
    AddAttribute("function",AttributeType::HoldAll);
    
    RegisterFunction( "函数",MemberFunction(&SystemModule::PD_def), this);
    AddAttribute("函数",AttributeType::HoldAll);
    AddAttribute("FUNCTION$DEFINED$",AttributeType::DelayFunction);
    AddAttribute("FUNCTION$DEFINED$",AttributeType::HoldAll);

  }
  {
    RegisterFunction("Conjunct",Conjunct,this);
    AddAttribute("Conjunct",AttributeType::HoldAll );
  }
  ///////////////////////////////////////////////////////////
  {
    RegisterFunction("If",If,this);
    AddAttribute("If",AttributeType::HoldAll);
  }
  {
    RegisterFunction("While",While,this);
    AddAttribute("While",AttributeType::HoldAll);
  }
  {
    RegisterFunction("For",For,this);
    AddAttribute("For",AttributeType::HoldAll);
  }
  {
    RegisterFunction("Foreach",Foreach,this);
    AddAttribute("Foreach",AttributeType::HoldAll);
  }
  {
    RegisterFunction("Do",Do,this);
    AddAttribute("Do",AttributeType::HoldAll);
    AddAttribute("do",AttributeType::HoldAll);
  }
  {
     
    
    RegisterFunction("如果", (MemberFunction)&SystemModule::PD_if, this );
    AddAttribute("如果",  AttributeType::HoldAll );
    AddAttribute("又如",AttributeType::Protected  );
    AddAttribute("否则",AttributeType::Protected );
    AddAttribute("if",  AttributeType::HoldAll );
    AddAttribute("elif",AttributeType::Protected );
    AddAttribute("else",AttributeType::Protected );
    RegisterFunction("只要", (MemberFunction)&SystemModule::PD_while, this );
    AddAttribute("只要",AttributeType::HoldAll);
    AddAttribute("while",AttributeType::HoldAll);
    RegisterFunction("罗列", (MemberFunction)&SystemModule::PD_for, this );
    AddAttribute("罗列",AttributeType::HoldAll);
    AddAttribute("for",AttributeType::HoldAll);
    RegisterFunction("于", (MemberFunction)&SystemModule::PD_in, this );
    AddAttribute("于",AttributeType::Protected );
    AddAttribute("in",AttributeType::Protected );
    AddAttribute("do",AttributeType::HoldAll);
    AddAttribute("switch",AttributeType::HoldAll);
    AddAttribute("case",AttributeType::Protected );
    AddAttribute("default",AttributeType::Protected );
    AddAttribute("in",AttributeType::HoldAll);
    AddAttribute("var",AttributeType::HoldAll);
  }
  // {
  //   RegisterFunction("System",System,this);
  //   RegisterFunction("SystemWithoutOutput",SystemWithoutOutput,this);
  // }
  ///////////////////////////////////////////////////////////
  RegisterFunction("SetOutputFormat",SetOutputFormat,this,"Set output format for numbers.\nSetOutputFormat[precison,is_scientific]"); 
  RegisterFunction("SystemArgv",SystemArgv,this,"Get system input argument variables.");
  ///////////////////////////////////////////////////////////
  RegisterFunction("Help",ShowHelpInfo,this,"Show help information of module functions.");
  RegisterFunction("Clear",Clear,this,"Clear defination relating to symbol(s)."); 
  AddAttribute("Clear",AttributeType::HoldAll);

  RegisterFunction("clear",Clear,this,"Clear defination relating to symbol(s)."); 
  AddAttribute("clear",AttributeType::HoldAll);

  //RegisterFunction("Protect",Protect_Eva,this,"Add Protect Attribute to a Symbol.");
  AddAttribute("Protect",AttributeType::HoldAll);
  //RegisterFunction("UnProtect",UnProtect_Eva,this,"Remove Protect Attribute of a Symbol if exist.");
  AddAttribute("UnProtect",AttributeType::HoldAll);
  ///////////////////////////////////////////////////////////
  RegisterFunction("AbsoluteTime",AbsoluteTime,this,"Get Absolute time from 1970.");
  RegisterFunction("DateList",DateList,this,"Give out the Date Object .");
  {
    RegisterFunction("Timing",Timing,this,"Calculate the time a command consumed."); 
    AddAttribute("Timing",AttributeType::HoldAll);
  }
  {
    RegisterFunction("CpuTiming",CpuTiming,this,"Calculate the Cpu time a command consumed."); 
    AddAttribute("CpuTiming",AttributeType::HoldAll);
  }
  RegisterFunction("Sleep",Sleep,this,"Sleep for some seconds. Then continue to run.");
  RegisterFunction("sleep",Sleep,this,"Sleep for some seconds. Then continue to run.");

  RegisterFunction("ToString",ToString_Eva,this);
  RegisterFunction("string",ToString_Eva,this);
  RegisterFunction("ToExpression",ToExpression,this);

  {
    RegisterFunction("SimpleSimplify", SimpleSimplify, this );
  }
  AddAttribute("Profile",AttributeType::HoldAll);

  //dout<<"System register funtion list = "<< __module_function_list <<" id="<<__module_function_list.objid<<endl;
  AddAttribute("type",   AttributeType::HoldAll);
  AddAttribute("String", AttributeType::Protected);
  AddAttribute("Number", AttributeType::Protected);
  AddAttribute("Symbol", AttributeType::Protected);
  AddAttribute("List",   AttributeType::Protected);

};

SystemModule::~SystemModule(){
};

int SystemModule::PD_Evaluate(Object &ARGV){
  /*zh:
    Evaluate(expr)
        对处于Hold状态中的表达式expr进行求值
    |||
    Evaluate(expr)
        Evaluate expr in 'Hold' state
   */
  CheckShouldEqual(1);
  EvaKernel->Evaluate(ARGV[1],false);
  ARGV = ARGV[1];
  ReturnNormal;
}

int SystemModule::PD_EvaluateString(Object &ARGV){
  /*zh:
    EvaluateString(str)
        解析str中的表达式并求值
    |||
    EvaluateString(str)
        parse expression in str and evaluate the parsed result
   */
  CheckShouldEqual(1);
  CheckShouldBeString(1);
  string code = ARGV[1].Key(); 
  pond::PondInnerStringRestoreNormal( code );
  Object result;
  EvaKernel->EvaluateString(code, result, 0, true);
  ARGV = result;
  ReturnObject(result);
}

int SystemModule::PD_EvaluateFile(Object &ARGV){
  /*zh:
    EvaluateFile(filename)
        解析文件中的内容并求值
    |||
    EvaluateFile(filename)
        parse content in file and evaluate the parsed result
   */
  CheckShouldEqual(1);
  CheckShouldBeString(1);
  Object result;
  EvaKernel->EvaluateFile(ARGV[1].Key(), result, 0, false);
  ReturnNull;
}

int SystemModule::PD_EvaluateFileWithReturn(Object &ARGV){
  /*zh:
    EvaluateFile(filename)
        解析文件中的内容并求值
    |||
    EvaluateFile(filename)
        parse content in file and evaluate the parsed result
   */
  CheckShouldEqual(1);
  CheckShouldBeString(1);
  Object result;
  EvaKernel->EvaluateFile(ARGV[1].Key(), result, 0, false);
  ReturnObject(result);
}

int SystemModule::PD_Options(Object & ARGV){
  ReturnHold;
}

int SystemModule::SetOptions(Object & ARGV){
  ReturnHold;
}

int SystemModule::SetAttribute(Object &left,Object&attris){
  if ( not attris.ListQ(SYMID_OF_List) ) {
    zhErroring("设置特性", "特性参数应当以列表的形式传入.") ||
    _Erroring("SetAttribute", "Attributes should be in a List.");
    ReturnError;
  }
  for ( u_int i =1 ;i<= attris.Size();i++){
    if (not(attris)[i].StringQ())
    {
      zhErroring( "设置属性", "属性应该用字符串表示") ||
        _Erroring("SetAttibute", "Attibutes should be Strings.");
      ReturnError;
    }
  }
  bool *attri;
  if ( left.ListQ( SYMID_OF_Attributes) ){
    if ((left).Size() != 1 ) {
      zhErroring("SetAttributes","Attributes 要求以Object作为参数.")||
        _Erroring("SetAttributes","Attributes requires an Object as argument.");
      ReturnError; }
    if ( not left[1].SymbolQ() ) {
      zhErroring("SetAttribute","只有符号变量才能被赋予属性.")||
        _Erroring("SetAttribute","Only Symbol can assign attributes to.");
      ReturnError;
    }
    attri = EvaKernel->GetAttributes( (left)[1].Key() );
  }else{
    if ( not left.SymbolQ() ) {
      zhErroring("SetAttribute","只有符号变量才能被赋予属性.")||
        _Erroring("SetAttribute","Only Symbol can assign attributes to.");
      ReturnError;
    }
    attri = EvaKernel->GetAttributes( left );
  }
  if ( attri == NULL ) {
    zhErroring("SetAttributes","不能创建新的属性.")||
      _Erroring("SetAttributes","Can not create new attributes.");
    ReturnError;
  }
  
  AttributeType type;
  for (u_int i=1; i<=attris.Size(); i++ ){
    if ( not attris[i].StringQ() ) {
      zhErroring("SetAttributes","要求一个属性字符串.")||
        _Erroring("SetAttributes","An attribute string is required.");
      ReturnError;
    }
    type = String2AttributeType( attris[i].Key() );
    if ( type == AttributeType::Null ) {
      zhErroring("SetAttribute",(string)"未知属性 \""+attris[i].Key()+"\".")||
        _Erroring("SetAttribute",(string)"Unkonwn Attribute \""+attris[i].Key()+"\".");
      ReturnError;
    }
    pond::SetAttribute(attri,type);
  }
  return 0;
};

int SystemModule::SetAttribute_Eva(Object &ARGV){
  CheckArgsShouldEqual(ARGV,2);
  SetAttribute( ARGV[1], ARGV[2]);
  ARGV = ARGV[2];
  ReturnNormal;
};

int SystemModule::AddAttribute_Eva(Object &ARGV){
  CheckShouldEqual(2);
  CheckShouldBeSymbol(1);
  EvaRecord *rec = EvaKernel->GetOrNewEvaRecord( ARGV[1] );
  if ( rec == NULL ) {
    zhErroring("AddAttribute","未能获取符号变量 '"+ARGV[1].ToString()+"' 的参数对象.")||
      _Erroring("AddAttribute","Can not get Attributes Object for Symbol '"+ARGV[1].ToString()+"'.");
    ReturnError;
  }
  if ( AttributeQ(rec->attributes,AttributeType::Protected) ) {
    zhErroring("AddAttribute",(string)"符号变量'"+ARGV[1].Key()+"' 处于保护状态中.")||
      _Erroring("AddAttribute",(string)"Symbol '"+ARGV[1].Key()+"' is protected.");
    ReturnError;
  }
  if ( ARGV[2].SymbolQ() || ARGV[2].StringQ() ){
    AttributeType attriType = String2AttributeType( ARGV[2].Key() );
    if ( attriType == AttributeType::Null ) {
      zhErroring("AddAttribute",(string)"给出的属性类型'"+ARGV[2].Key()+"' 不正确.")||
        _Erroring("AddAttribute",(string)"Attribute type '"+ARGV[2].Key()+"' specified is not right.");
      ReturnError;
    }
    pond::SetAttribute(rec->attributes, attriType );
    ReturnNull;
  } else if ( ARGV[2].ListQ(SYMID_OF_List) ){
    AttributeType attriType;
    for ( u_int i =1; i<=ARGV[2].Size() ; i++){
      attriType = String2AttributeType( ARGV[2][i].Key() );
      if ( attriType == AttributeType::Null ) {
        zhErroring("AddAttribute",(string)"给出的属性类型'"+ARGV[2][i].Key()+"' 不正确.")||
          _Erroring("AddAttribute",(string)"Attribute type '"+ARGV[2][i].Key()+"' specified is not right.");
        ReturnError;
      }
      pond::SetAttribute( rec->attributes, attriType );
    }
    ReturnNull;
  }
  zhErroring("AddAttribute","给出的属性值形式不正确.")||
    _Erroring("AddAttribute","Attributes specified is not in the right form.");
  ReturnError;
};

int SystemModule::RemoveAttributes_Eva(Object &ARGV){
  CheckShouldEqual(2);
  CheckShouldBeSymbol(1);
  EvaRecord *rec = EvaKernel->GetOrNewEvaRecord( ARGV[1] );
  if ( rec == NULL ) {
    zhErroring("RemoveAttribute","未能获取符号变量 '"+ARGV[1].ToString()+"' 的参数对象.")||
      _Erroring("RemoveAttribute","Can not get Attributes Object for Symbol '"+ARGV[1].ToString()+"'.");
    ReturnError;
  }
  if ( AttributeQ(rec->attributes,AttributeType::Protected) ) {
    zhErroring("RemoveAttribute",(string)"符号变量'"+ARGV[1].Key()+"' 处于保护状态中.")||
      _Erroring("RemoveAttribute",(string)"Symbol '"+ARGV[1].Key()+"' is protected.");
    ReturnError;
  }
  if ( ARGV[2].SymbolQ() || ARGV[2].StringQ() ){
    AttributeType attriType = String2AttributeType( ARGV[2].Key() );
    if ( attriType == AttributeType::Null ) {
      zhErroring("RemoveAttribute",(string)"给出的属性类型'"+ARGV[2].Key()+"' 不正确.")||
        _Erroring("RemoveAttribute",(string)"Attribute type '"+ARGV[2].Key()+"' specified is not right.");
      ReturnError;
    }
    pond::SetAttribute( rec->attributes, attriType,false);
    ReturnNull;
  }else if ( ARGV[2].ListQ(SYMID_OF_List) ){
    AttributeType attriType;
    for ( u_int i =1; i<=ARGV[2].Size() ; i++){
      attriType = String2AttributeType( ARGV[2][i].Key() );
      if ( attriType == AttributeType::Null ) {
        zhErroring("RemoveAttribute",(string)"给出的属性类型'"+ARGV[2][i].Key()+"' 不正确.")||
          _Erroring("RemoveAttribute",(string)"Attribute type '"+ARGV[2][i].Key()+"' specified is not right.");
        ReturnError;
      }
      pond::SetAttribute( rec->attributes, attriType,false );
    }
    ReturnNull;
  }
  zhErroring("RemoveAttribute","给出的属性值形式不正确.")||
    _Erroring("RemoveAttribute","Attributes specified is not in the right form.");
  ReturnError;
};

int SystemModule::PD_Protect(Object &ARGV){
  /*zh:
    Protect( [sym] )
        为[sym]添加保护属性(Protected)，禁止对[sym]进行修改操作
    |||
    Protect( [sym] )
        Add a 'Protected' attribute to symbol [sym], prevent any change to [sym]
  */
     
  CheckShouldEqual(1);
  CheckShouldBeSymbol(1);
  EvaRecord *rec = EvaKernel->GetOrNewEvaRecord( ARGV[1] );
  if ( rec == NULL ) {
    zhErroring("Protect","不能获得符号'"+ARGV[1].ToString()+"'的属性对象")||
      _Erroring("Protect","Can not get Attributes Object for Symbol '"+ARGV[1].ToString()+"'");
    ReturnError;
  }
  pond::SetAttribute( rec->attributes, AttributeType::Protected, true);
  ReturnNull;
};

int SystemModule::PD_UnProtect(Object &ARGV){
  /*zh:
    UnProtect( [sym] )
        移除[sym]的保护属性(Protected)，恢复对[sym]进行修改操作的权限
    |||
    UnProtect( [sym] )
        Remove 'Protected' attribute of symbol [sym]
  */
  CheckShouldEqual(1);
  CheckShouldBeSymbol(1);
  EvaRecord *rec = EvaKernel->GetOrNewEvaRecord( ARGV[1] );
  if ( rec == NULL ) {
    zhErroring("移除属性","不能获得符号"+ARGV[1].ToString()+"的属性对象") ||
      _Erroring("RemoveAttribute","Can not get Attributes Object for Symbol"+ARGV[1].ToString() );
    ReturnError;
  }
  pond::SetAttribute( rec->attributes, AttributeType::Protected,false);
  ReturnNull;
};

int SystemModule::PD_Attributes(Object & ARGV){
  Set_Context(Attributes){
    return SetAttribute_Eva(ARGV);
  }
  CheckArgsShouldEqual(ARGV,1);
  CheckShouldBeSymbol(1);
  Object arg = ARGV(1);
  bool *attri = EvaKernel->GetAttributes( arg.Key() );
  ARGV.SetList();
  if ( attri ){
    for ( int i = (int)AttributeType::Null+1; i< (int)AttributeType::MAXENUM; i++ ){ 
      if ( attri[i] )
        ARGV.PushBackString( AttributeType2String((AttributeType)i) );
    }
  }
  ReturnNormal;
}

int SystemModule::Parenthesis(Object & ARGV){
  //dout<<"comes to ()"<<ARGV<<endl;
  for (int i = 1;  i <= ARGV.Size() ; i++){
    EvaKernel->Evaluate( ARGV[i] );
    //dout<<"one () run end to "<<ARGV<<endl;
    if ( EvaKernel->statusCode == 88 ){
      ARGV = EvaKernel->statusObject;
      ReturnNormal;
    }else if( EvaKernel->statusCode == 66 or EvaKernel->statusCode == 44 ){
      ReturnNull;
    }
  }
  //dout<<"() evaluated to "<<ARGV<<endl;
  if ( ARGV.Size() == 0 ){
    ReturnNull;
  }
  if ( ARGV.Size() == 1 ){
    ARGV = ARGV[ 1 ];
    ReturnNormal;
  }
  // ARGV[0].SetSymbol( SYMID_OF_Tuple );
  ReturnNormal;
}

int SystemModule::ExpressionList(Object & ARGV){
  int stackPtr = EvaKernel->GetStackPtr();
  for (int i = 1;  i <= ARGV.Size() ; i++){
    EvaKernel->Evaluate( ARGV[i] );
    //dout<<"Expression evaluate "<<i<<" to "<<ARGV<<endl;
    if ( EvaKernel->statusCode == 88 ){
      ARGV = EvaKernel->statusObject;
      EvaKernel->SetStackToPtr( stackPtr );
      ReturnNormal;
    }else if( EvaKernel->statusCode == 66 or
              EvaKernel->statusCode == 44 ){
      EvaKernel->SetStackToPtr( stackPtr );
      ReturnNull;
    }
  }
  EvaKernel->SetStackToPtr( stackPtr );
  //dout<<"Expression evaluate end to "<<ARGV<<endl;
  if ( ARGV.Size() == 0 ){
    ReturnNull;
  }
  if ( ARGV.Size() == 1 ){
    ARGV = ARGV[ 1 ];
  }
  //dout<<"Expression evaluate final end to "<<ARGV<<endl;
  ReturnNormal;
}

int SystemModule::CompoundExpression(Object & ARGV){
  for (auto iter = ARGV.Begin(); iter!= ARGV.End() ; iter++){
    EvaKernel->Evaluate(*iter);
    if ( EvaKernel->statusCode == 88 ){
      ARGV = EvaKernel->statusObject;
      ReturnNormal;
    }else if( EvaKernel->statusCode == 66 or EvaKernel->statusCode == 44 ){
      ReturnNull;
    }
  }
  //dout<<" ce = "<<ARGV<<endl;
  if ( ARGV.Size() > 0 ) 
    ARGV = ARGV[ ARGV.Size() ];
  else
    ARGV.SetNull();
  ReturnNormal;
}

int SystemModule::ToString_Eva(Object& ARGV)
{
  string content;
  for (auto i=1; i<= ARGV.Size(); i++ ){
    if ( ARGV[i].StringQ() ){
      content += ARGV[i].Key();
    }else{
      content += ARGV[i].ToString();
    }
  }
  ARGV.SetString( content );
  ReturnNormal;
}

/*
int SystemModule::ToExpression_Eva(Object& ARGV, Evaluation *Eva )
{
  CheckShouldEqual(1);
  CheckShouldBeString(1);
  Object t;
  t = ImportList::ToExpression( ARGV[1].Key() );
  ARGV = t;
  ReturnNormal;
}
*/

int SystemModule::MatchQ(Object & ARGV){
  CheckArgsShouldEqual(ARGV,2);
  EvaKernel->Evaluate(ARGV[1]);
  //cerr<<"ARGV = "<<ARGV.ToFullFormString()<<endl;
  //cerr<<"ARGV[1] = "<<ARGV[1].ToFullFormString()<<endl;
  Object&l=ARGV[1],&pattern = ARGV[2];
  Pattern::UnifyRuleLeft( pattern );
  ReturnBoolean( Pattern::UnifiedMatchQ( l, pattern ) );
}

int SystemModule::UnifiedMatchQ(Object & ARGV){
  CheckArgsShouldEqual(ARGV,2);
  Object&l=ARGV(1),&pattern = ARGV(2);
  ReturnBoolean( Pattern::UnifiedMatchQ(l,pattern));
}

int SystemModule::UnifyRule(Object & ARGV){
  CheckArgsShouldEqual(ARGV,1);
  Object&pattern = ARGV(1);
  if ( (pattern).Size() < 2 ){
    { _Erroring("UnifyRule","Rule form is right."); ReturnError; }
  }
  Pattern::UnifyRule( (pattern)[1], (pattern)[2] );
  ARGV = ARGV[1];
  ReturnNormal;
}

// int SystemModule::Hold(Object & ARGV){
//   for ( auto iter = ARGV.Begin(); iter!= ARGV.End(); iter++){
//     EvaKernel->Evaluate( *iter, true );
//   }
//   ReturnHold;
// }

// int SystemModule::HoldPattern(Object & ARGV){
//   CheckShouldEqual( 1 );
//   EvaKernel->Evaluate( ARGV[1], true );
//   ReturnHold;
// }

int SystemModule::HoldOnce(Object & ARGV){
  CheckShouldEqual(1);
  EvaKernel->Evaluate( ARGV[1], true );
  ARGV = ARGV[1];
  ReturnNormal;
}


int SystemModule::Replace(Object & ARGV){
  CheckArgsShouldEqual(ARGV,2);
  Object&l = ARGV(1);
  Object&rule = ARGV(2);
  Pattern::UnifyRule( rule );
  bool replaced = Pattern::UnifiedReplaceAll(l,rule,false);
  if ( replaced ) EvaKernel->Evaluate(l);
  ARGV = ARGV[1];
  ReturnNormal;
}

int SystemModule::ReplaceAll(Object & ARGV){
  CheckShouldEqual( 2);
  CheckShouldBeList( 2 );
  Object&l     = ARGV(1); 
  Object&rules = ARGV(2);
  bool replaced = true;
  if ( rules.PairQ( SYMID_OF_Rule ) or
       rules.PairQ( SYMID_OF_RuleDelayed ) or
       rules.PairQ( SYMID_OF_KeyValuePair ) ){
    Pattern::UnifyRule( rules );
    replaced = Pattern::UnifiedReplaceAll(l,rules,true);
    if ( replaced ) EvaKernel->Evaluate(l);
    ARGV = ARGV[1];
    ReturnNormal;
  }else{
    for (u_int i = 1; i<= rules.Size() ; i++){
      if ( not rules[i].PairQ() ) continue;
      Pattern::UnifyRule( rules[i] );
      if ( Pattern::UnifiedReplaceAll( l, rules[i] ,true) )
        replaced = true;
    }
    if ( replaced ) EvaKernel->Evaluate(l);
    ARGV = ARGV[1];
    ReturnNormal;
  }
  // _Erroring("ReplaceAll","Second parameter should be a Rule or a ARGV of rules.");
  // ReturnError; 
}

int SystemModule::ReplaceAllRepeated(Object & ARGV){
  CheckArgsShouldEqual(ARGV,2);
  Object&l = ARGV(1); 
  Object&rules = ARGV(2);
  bool replaced = true;
  if ( rules.PairQ( SYMID_OF_Rule ) or
       rules.PairQ( SYMID_OF_RuleDelayed ) or
       rules.PairQ( SYMID_OF_KeyValuePair ) ){
    Pattern::UnifyRule( rules );
    while ( replaced ){
      replaced = false;
      if ( Pattern::UnifiedReplaceAll(l, rules,true) ){
        replaced = true;
      }
      if ( replaced ) EvaKernel->Evaluate(l);
    }
  }else{
    for (u_int i = 1; i<= rules.Size() ; i++){
      if ( not rules[i].PairQ() ) continue;
      Pattern::UnifyRule( rules[i] );
    }
    while ( replaced ){
      replaced = false;
      for (u_int i = 1; i<= rules.Size() ; i++){
        if ( not rules[i].PairQ() ) continue;
        if ( Pattern::UnifiedReplaceAll( l, (rules)[i] ,true) )
          replaced = true;
      }
      if ( replaced ) EvaKernel->Evaluate(l);
    }
  }
  ARGV = ARGV[1];
  ReturnNormal;
}

int SystemModule::SimpleSimplify(Object&ARGV){
  if ( not ARGV.ListQ() )
    return 0;
  if ( ARGV.ListQ( SYMID_OF_SimpleSimplify ) ){
    CheckArgsShouldEqual(ARGV,1);
    ARGV = ARGV[1];
  }
  // return 0;
  //dout<<"try simplify "<<ARGV<<endl;
  static Object SSRules(ObjectType::List,SYMID_OF_List);
  // if ( false and SSRules.Size() == 0 ){
  if ( SSRules.Size() == 0 ){
#define addrule(rule_str) {                       \
      Object obj;                                 \
      obj =  ImportList::ToExpression(rule_str);  \
      Pattern::UnifyRule( obj );                  \
      SSRules.PushBackRef( obj );                 \
    }
    /////////////
    addrule("HoldPattern( Plus(a~))                                                 :> a");
    addrule("HoldPattern( Plus(a~~~,Null,b~~~))                                     :> Plus(a,b)");
    addrule("HoldPattern( Plus(a~~~,x~,x~,b~~~))                                    :> Plus(a,Times(2,x),b)");
    addrule("HoldPattern( Plus(a~~~,x~,Times(i~Number,x~),b~~~))                    :> Plus(a,Times(Plus(1,i),x),b)");
    addrule("HoldPattern( Plus(a~~~,Times(x~~),Times(i~Number,x~~),b~~~))           :> Plus(a,Times(Plus(1,i),x),b)");
    addrule("HoldPattern( Plus(a~~~,Times(i~Number,x~~),Times(j~Number,x~~),b~~~))  :> Plus(a,Times(Plus(i,j),x),b)");
    //// Times Power
    addrule("HoldPattern( Times(a~))                                                :> a");
    addrule("HoldPattern( Times(a~~~,,b~~~))                                        :> Times(a,b)");
    addrule("HoldPattern( Times(x~,x~))                                             :> Power(x,2)");
    addrule("HoldPattern( Times(a~~~,x~,x~,b~~~))                                   :> Times(a,Power(x,2),b)");
    addrule("HoldPattern( Times(a~~~,x~,Power(x~,i~Number),b~~~))                   :> Times(a,Power(x,Plus(1,i)),b)");
    addrule("HoldPattern( Times(a~~~,Power(x~,i~Number),Power(x~,j~Number),b~~~))   :> Times(a,Power(x,Plus(i,j)),b)");
    ////////////////////////
    //////////////////////// normal rules
    addrule("HoldPattern( Power(x~,0)                 :> 1 )");
    addrule("HoldPattern( Power(x~,1)                 :> x )");
    addrule("HoldPattern( Power(Power(x~,a~),b~)      :> Power(x,Times(a,b)) )");
    addrule("HoldPattern( Log(Power(x~,m~))           :> Times(m,Log(a)) )");
#undef addrule
  }
  //dout<<"simplify with rules:"<<SSRules<<endl;
  bool replaced = true;
  while (replaced){
    replaced = false;
    for ( u_int i= 1 ;i <= SSRules.Size() ; i++){
      if ( Pattern::UnifiedReplaceAll( ARGV , SSRules[i] ,true) )
        replaced = true;
    }
    if ( replaced ){
      EvaKernel->Evaluate( ARGV );
    }
  }
  return 0;
}

int SystemModule::GrammarModule(Object &ARGV){
  CheckArgsShouldNoLessThan(ARGV,2);
  // create the local variables;
  EvaKernel->newContext();
  Object variables = ARGV(1);
  if ( not (variables).ListQ( SYMID_OF_List ) ) {
    _Erroring("Module","Local variable should be in a List.");
    EvaKernel->deleteContext();
    ReturnError;
  }
  // dealing new variables in new context,
  // Part can be directly evaluated to the sybmol in the list
  for (Object::iterator iter = variables.Begin(); iter!=variables.End(); iter++){
    if ( (*iter).ListQ( SYMID_OF_Part ) ){
      Object&obj = *iter; 
      //cerr<<"use part as local var spec:"<<obj.ToFullFormString()<<endl;
      if ( obj.Size() < 2 ){
        _Erroring("Module","Local variable specification "+obj.ToString()+ " wrong.");
        EvaKernel->deleteContext();
        ReturnError;
      }
      Object result;
      //cerr<<"try get part result"<<endl;
      ListModule::GetPartList( obj[1], obj, 2, result );
      //cerr<<"result = "<< result << endl;
      if ( not result.SymbolQ() ){
        _Erroring("Module","Local variable specification "+result.ToString()+"("+obj.ToString()+ ") should be a symbol.");
        EvaKernel->deleteContext();
        ReturnError;
      }
      Object rvalue; rvalue.CopyObject(result);
      EvaKernel->InstantInsertOrUpdatePairValue(result,rvalue);
      continue;
    }
    if ( (*iter).SymbolQ() || (*iter).ListQ( SYMID_OF_Part ) ){
      Object rvalue; rvalue.CopyObject(*iter);
      //rvalue.KeyPrepend( "$" );
      EvaKernel->InstantInsertOrUpdatePairValue(*iter,rvalue);
      continue;
    }
    if ( (*iter).ListQ( SYMID_OF_Set ) ){
      EvaKernel->InstantInsertOrUpdatePairValue( (*iter)[1],(*iter)[2] );
      continue;
    }
    if ( (*iter).ListQ( SYMID_OF_SetDelayed ) ){
      //EvaKernel->Evaluate( (*iter)[2]); 
      EvaKernel->InstantInsertOrUpdatePairValue( (*iter)[1],(*iter)[2]);
      continue;
    }
    _Erroring("Module","Local variable specification "+variables.ToString()+" contains "+(*iter).ToString()+" which is not a atom symbol or an assignment to a symbol."); 
    EvaKernel->deleteContext();
    ReturnError;
  }
  //evaluate the returned ARGV
  for (Object::iterator iter = ARGV.Begin()+1; iter!=ARGV.End(); iter++){
    EvaKernel->Evaluate( *iter);
  }
  EvaKernel->deleteContext();
  ARGV = ARGV[ ARGV.Size() ];
  ReturnNormal;
}

int PureFunctionApply(Object&p_expr,Object&ARGV){
  if ( p_expr.NullQ() ) return 0;
  switch (p_expr.type() ){
  case ObjectType::Number: return 0;
  case ObjectType::String: return 0;
  case ObjectType::Symbol:{
    if ( p_expr.ids() == SYMID_OF_FunctionVariable ){
      u_int id = p_expr.re();
      if ( id == 0 or id > ARGV.Size() )
        { _Erroring("PureFunction","PureFunction variable id exceed the number of arguments."); ReturnError; }
      p_expr = ARGV[id];
    }
    return 0;
  }
  case ObjectType::List:{
    for (auto iter = p_expr.ElementsBegin(); iter != p_expr.ElementsEnd(); iter++){
      PureFunctionApply(*iter,ARGV);
    }
  }
  }
  return 0;
}

// pure function has no
int SystemModule::PureFunction(Object &ARGV){
  CheckShouldNoLessThan(1);
  CheckArgsShouldEqual(ARGV[0],1);
  Object expr = ARGV[0][1];
  PureFunctionApply(expr,ARGV);
  ARGV = expr;
  EvaKernel->Evaluate( ARGV );
  ReturnNormal;
}


int SystemModule::Function(Object &ARGV){
  // CheckShouldNoLessThan(1);
  Object&func = ARGV[0];
  CheckArgsShouldBeWithin(func,1,2);
  if ( func.Size() == 1 ){
    return PureFunction(ARGV);
  }
  Object &varlist=func[1];
  //dout<<"into function with varlist = "<<varlist <<endl;
  if ( varlist.SymbolQ() ){
    if ( ARGV.Size() != 1 ) {
      zhErroring("函数","参数列表和调用提供参数不一致.") ||
        _Erroring("Function","Function variable list is different from arguments applied to.");
      ReturnError;
    }
    EvaKernel->newContext();
    Object pairobj = EvaKernel->StackPushCopy( varlist, varlist);
    pairobj[2] = ARGV[1];
    Object res = func[2];
    EvaKernel->Evaluate( res );
    ARGV = res;
    EvaKernel->deleteContext();
    ReturnNormal;
  }else if ( varlist.ListQ() ){
    if ( ARGV.Size() != varlist.Size() ) {
      zhErroring("函数","参数列表和调用提供参数不一致.") ||
        _Erroring("Function","Function variable list is different from arguments applied to.");
      ReturnError;
    }
    EvaKernel->newContext();
    for(u_int i = 1; i <= varlist.Size() ; i++){
      if ( not varlist[i].SymbolQ() ) {
        zhErroring("函数","函数参数列表应该是一个符号列表") ||
          _Erroring("Function","Argument list elements are required to be Symbols.") ; 
        EvaKernel->deleteContext();
        ReturnError; 
      }
      Object pairobj = EvaKernel->StackPushCopy(varlist[i], varlist[i] );
      pairobj[2] = ARGV[i];
    }
    Object res = func[2];
    EvaKernel->Evaluate(res);
    ARGV = res;
    EvaKernel->deleteContext();
    ReturnNormal;
  }
  zhErroring("函数","函数参数列表应该是符号，或者符号列表") ||
    _Erroring("Function","Arguments should be symbols or list of symbols.") ; 
  ReturnError; 
}

int SystemModule::Conjunct(Object &ARGV){
  CheckShouldEqual(2);
  //Dealing 1st argument evaluate until to a conjunctable expr
  //cout<<"get into Conjunct with ARGV = "<<ARGV<<endl;
  Object &fobj = ARGV[1];
  //EvaKernel->Evaluate( fobj ); // should be reference evaluate
  EvaKernel->Evaluate( fobj, false, true );
  // and those obj with Conjunctable should take care of how to update the ref value
  //cout<<"fobj after eval= "<<fobj<<endl;
  if ( not fobj.SimpleListQ() ){
    ReturnHold;
  }
  if ( not fobj[0].ValuedSymbolQ() ){
    ReturnHold;
  }
  EvaRecord *rec = EvaKernel->GetEvaRecord( fobj[0] );
  if ( rec ){
    if ( AttributeQ(rec->attributes, AttributeType::Conjunctable ) ){
      // the conjunct pass through has form
      // Conjuct(  f(...), xxx )    xxx can be any form,  f is referenced value if is from value pair
      //dout<<"pass "<<ARGV << " to "<< fobj[0] <<endl;
      return EvaKernel->Call(rec, ARGV);
    }
    ReturnHold;
  }
  ReturnHold;
}

int SystemModule::If(Object &ARGV){
  CheckShouldBeWithin(2,3);
  EvaKernel->Evaluate(ARGV[1]);
  char res = ARGV[1].Boolean();
  if (  res > 0 ){
    EvaKernel->Evaluate(ARGV[2]);
    ARGV = ARGV[2];
  }else if (  res == 0 ){
    if ( ARGV.Size() == 3){
      EvaKernel->Evaluate(ARGV[3]);
      ARGV = ARGV[3];
    }else{// return Null
      ARGV.SetNull();
    }
  }
  // condition result not known Boolean() == -1
  //dprintf("If returned with hold");
  ReturnHold;
}
int SystemModule::While(Object &ARGV){
  CheckShouldEqual(2);
  Object&condition = ARGV(1);
  Object&expression = ARGV(2);
  Object cond,expr;
  for(;;){
    cond = NullObject;
    cond.CopyObject(condition);
    EvaKernel->Evaluate(cond);
    if ( cond.Boolean() > 0 ){ //evaluate expr
      expr = NullObject;
      expr.CopyObject(expression);
      EvaKernel->Evaluate( expr);
      continue;
    }
    break;
  }
  ReturnNull;
}

int SystemModule::For(Object &ARGV){
  CheckShouldEqual(4);
  Object&start= ARGV(1);
  EvaKernel->Evaluate( start);
  Object&test= ARGV(2);
  Object&incr= ARGV(3);
  Object&body= ARGV(4);
  Object ltest,lincr,lbody;
  for(;;){
    ltest = NullObject;
    ltest.CopyObject(test);
    EvaKernel->Evaluate( ltest);
    if ( ltest.Boolean() > 0 ){ //evaluate expr
      lbody = NullObject;
      lbody.CopyObject(body);
      EvaKernel->Evaluate( lbody);
      lincr = NullObject;
      lincr.CopyObject(incr);
      EvaKernel->Evaluate(lincr);
      continue;
    }
    break;
  }
  ReturnNull;
}

int SystemModule::Foreach(Object&ARGV){
  CheckShouldEqual(3);
  Object&vars  = ARGV[1];
  EvaKernel->Evaluate( ARGV[2] );
  Object&lists = ARGV[2];
  Object&expr  = ARGV[3];
  if ( not lists.ListQ() )
    { _Erroring("Foreach","Second argument is required to be a List."); ReturnError; }
  if ( vars.SymbolQ() ){
    int ptr = EvaKernel->GetStackPtr();
    Object pairobj = EvaKernel->StackPushCopy(vars,vars.Copy() );
    Object&vobj = pairobj[2];
    Object newexpr;
    // EvaKernel->PreEvaluate( expr );
    for (auto iter= lists.Begin(); iter != lists.End(); iter++){
      vobj.CopyObject( *iter );
      newexpr.CopyObject( expr );
      EvaKernel->Evaluate( newexpr );
    }
    EvaKernel->SetStackToPtr( ptr );
    ReturnNull;
  }else if ( vars.ListQ() ){
    int ptr = EvaKernel->GetStackPtr();
    Object vobj_list; vobj_list.SetList();
    for ( auto iter = vars.Begin(); iter != vars.End(); iter ++ ){
      Object pairobj = EvaKernel->StackPushCopy(*iter,*iter);
      vobj_list.PushBackRef( pairobj[2] );
    }
    Object newexpr;
    for ( auto iter = lists.Begin(); iter != lists.End(); iter++ ){
      if ( iter->Size() != vobj_list.Size() ) { 
        _Erroring("Foreach::shape","List shape is not consistent with variable list."); 
        EvaKernel->SetStackToPtr( ptr );
        ReturnError; 
      }
      for ( auto viter=vobj_list.Begin(),liter=(*iter).Begin();viter!=vobj_list.End();viter ++,liter++){
        (*viter).CopyObject( *liter );
      }
      newexpr = NullObject;
      newexpr.CopyObject( expr );
      EvaKernel->Evaluate( newexpr );
    }
    EvaKernel->SetStackToPtr( ptr );
    ReturnNull;
  }
  _Erroring("Foreach","Iterator should be a Symbol or a List of Symbols.");
  ReturnError; 
}

int SystemModule::Do(Object &ARGV){
  //dout<<"try deal with function Do"<<endl;
  CheckShouldNoLessThan(2);
  while ( ARGV.Size() >2 ){
    Object newtable; newtable.SetList( SYMID_OF_Do ); 
    newtable.PushBackRef( ARGV(1) );
    newtable.PushBackRef( ARGV(2) );
    ARGV.Delete(2);
    ARGV(1) = newtable;
  }
  Object&expr = ARGV(1);
  Object&iter = ARGV(2);
  if ( not (iter).ListQ() || iter.Size()<1 || iter.Size()>4) {
    _Erroring("Do", (iter).ToString()+" is not a valid iterator ARGV form."); 
    ReturnError; 
  }
  //iteratorToList(iter,newlist);
  if ( iter.Size() == 1 ){ // form: {100}
    Object&num = (iter)(1);
    EvaKernel->Evaluate(num);
    if ( num.NumberQ() ){
      int N = num.Number() ;
      Object newexpr; 
      // EvaKernel->PreEvaluate( expr );
      for ( int i = 1; i<=N; i++ ){
        newexpr = NullObject;
        newexpr.CopyObject(expr );
        EvaKernel->Evaluate(newexpr);
      }
      ARGV.SetNull();
      ReturnNormal ;
    }
    _Erroring(ARGV.Key(),"Iterator "+iter.ToString()+" does not have appropriate bounds."); 
    ReturnError; 
  }
  if ( iter.Size() == 2 ){ // form: {i, 100}
    Object&var = (iter)(1);
    if ( !var.SymbolQ() ) { _Erroring("Do",var.ToString()+" cannot be used as an iterator."); ReturnError; }
    Object&num = (iter)(2);
    EvaKernel->Evaluate(num);
    //dout<<"try push stack var:"<< var<<" to table:"<<(EvaKernel->currentValueTable->stackTable)<<endl;
    Object&vobj = EvaKernel->StackPushCopy(var,var)[2];
    if ( num.NumberQ() ){
      int N =  num.Number();
      Object newexpr;
      // EvaKernel->PreEvaluate( expr );
      for (auto i = 1; i<=N; i++ ){
        newexpr = NullObject;
        newexpr.CopyObject( expr );
        vobj.SetNumber(i);
        EvaKernel->Evaluate(newexpr);
      }
      EvaKernel->StackPop();
      ReturnNull;
    }else if ( (num).ListQ() ){
      Object newexpr;
      // EvaKernel->PreEvaluate( expr );
      for (auto i = 1; i<=num.Size(); i++ ){
        newexpr = NullObject;
        newexpr.CopyObject( expr );
        vobj =  num[i];
        EvaKernel->Evaluate(newexpr);
      }
      EvaKernel->StackPop();
      ReturnNull;
    }
    _Erroring(ARGV.Key(),"Iterator "+iter.ToString()+" does not have appropriate bounds."); 
    ReturnError;
  }
  if ( iter.Size() == 3 ){ // for {i, 1 , 100}
    Object&var = (iter)(1);
    if ( !var.SymbolQ() ) { _Erroring("Do",var.ToString()+" cannot be used as an iterator."); ReturnError; }
    Object&num1 = (iter)(2);
    Object&num2 = (iter)(3);
    EvaKernel->Evaluate(num1);
    EvaKernel->Evaluate(num2);
    if ( num1.NumberQ() && num2.NumberQ() ){
      int Nbegin =  num1.Number();
      int Nend =  num2.Number();
      Object newexpr;
      Object pairobj = EvaKernel->StackPushCopy(var,var);
      Object&vobj = pairobj[2];
      // EvaKernel->PreEvaluate( expr );
      for ( int i = Nbegin; i<=Nend; i++ ){
        newexpr = NullObject;
        newexpr.CopyObject(expr);
        vobj.SetNumber(i);
        EvaKernel->Evaluate(newexpr);
      }
      EvaKernel->StackPop();
      ReturnNull;
    }
    { _Erroring(ARGV.Key(),"Iterator "+iter.ToString()+" does not have appropriate bounds."); ReturnError; }
  }
  if ( iter.Size() == 4 ){ // form :  {i,1,100,3}
    Object var = (iter)(1);
    if ( !var.SymbolQ() ) { 
      _Erroring(ARGV.Key(),var.ToString()+" cannot be used as an iterator."); 
      ReturnError; 
    }
    Object num1 = (iter)(2);
    Object num2 = (iter)(3);
    Object incr = (iter)(4);
    EvaKernel->Evaluate(num1);
    EvaKernel->Evaluate(num2);
    EvaKernel->Evaluate(incr);
    if ( num1.NumberQ() && num2.NumberQ() && incr.NumberQ() ){
      double Nbegin = num1.Number( );
      double Nend = num2.Number( );
      double Nincr = incr.Number( );
      if ( Nincr == 0 ) { _Erroring("Do","Increment should not be a zero."); ReturnError; }
      int steps = (Nend-Nbegin)/Nincr;
      Object newexpr;
      Object pairobj = EvaKernel->StackPushCopy(var,var);
      Object&vobj = pairobj[2];
      // EvaKernel->PreEvaluate( expr );
      for ( int i = 0 ;i<=steps;i++){
        newexpr = NullObject;
        newexpr.CopyObject(expr);
        vobj.SetNumber(Nbegin+i*Nincr);
        EvaKernel->Evaluate(newexpr);
      }
      EvaKernel->StackPop();
      ReturnNull;
    }
    _Erroring(ARGV.Key(),"Iterator "+iter.ToString()+" does not have appropriate bounds."); 
    ReturnError;
  }
  ReturnHold;
}

int SystemModule::ToExpression(Object & ARGV){
  string s;
  if ( not ARGV.StringQ() ){
    CheckArgsShouldEqual(ARGV,1);
    EvaKernel->Evaluate(ARGV[1]);
    CheckArgsShouldBeString(ARGV,1);
    s = ARGV[1].Key();
  }else{
    s = ARGV.Key();
  }
  Object tobj = ImportList::ToExpression(s);
  if ( tobj.voidQ() )
    { _Erroring("ToExpression","Ivalid string to transform to POND expression."); ReturnError; }
  ARGV = tobj;
  EvaKernel->Evaluate(ARGV);
  ReturnNormal;
}

int system_with_print(string cmd, vector<string>&result){
  result.clear();
  FILE *fp;
  char line[1024];
  fp = popen(cmd.c_str(),"r");
  if ( fp == NULL ){
    _Erroring("System",cmd+" cannot be excuted");
    return -1;
  }
  while ( fgets(line,sizeof(line),fp) != NULL ){
    if ( line[ strlen(line)-1 ] == '\n' ){
      line[ strlen(line)-1 ] = '\0';
    }
    result.push_back(line);
  }
  pclose(fp);
  return result.size();
}

int SystemModule::PD_System(Object &ARGV){
  CheckArgsShouldEqual(ARGV,1);
  EvaKernel->Evaluate( ARGV[1] );
  if ( !ARGV[1].StringQ() ) { _Erroring(ARGV.Key(),"First argument should be a string."); ReturnError; }
  vector<string> strs;
  if ( system_with_print( ARGV[1].Key(),strs ) <0 )
    { _Erroring(ARGV.Key(),"cmd "+ARGV[1].ToString()+" execute error."); ReturnError; }
  ARGV.SetList();
  for ( int i=0; i<(int)strs.size(); i++){
    ARGV.PushBackString( strs[i].c_str() );
  }
  ReturnNormal;
}

int SystemModule::SystemWithoutOutput(Object &ARGV)
{
  CheckArgsShouldEqual(ARGV,1);
  EvaKernel->Evaluate( ARGV[1] );
  if ( !ARGV[1].StringQ() ) { _Erroring(ARGV.Key(),"First argument should be a string."); ReturnError; }
  if ( system( ARGV[1].Key() ) <0 )
    { _Erroring(ARGV.Key(),"cmd "+ARGV[1].ToString()+" execute error."); ReturnError; }
  ReturnNull;
}

// const u_char Set_Temp_State = 1;
int SystemModule::Set( Object&ARGV){
  //if not symbol then evaluate its head such as a[x],,, a == Dict or Matrix or List or something
  CheckShouldEqual(2);
  Object&left = ARGV[1];
  Object&right = ARGV[2];
  // if ( ARGV.state() != Set_Temp_State ){
  EvaKernel->Evaluate( right );
  // }
  // not only left need to be evaluated, but also symbols
  //dout<<"get into set with ARGV = "<<ARGV<<endl;
  if ( left.ListQ() ){ // evaluate  in reference style
    EvaKernel->Evaluate(left[0],false,true);
  }
  if ( left.ListQ(SYMID_OF_List) ){
    if ( right.ListQ(SYMID_OF_List) ){
      if ( left.Size() != right.Size() ) {
        zhErroring("赋值","列表大小不匹配.") ||
          _Erroring("Set","Left List size should match size of right List.");
        ReturnError;
      }
      for ( u_int i = 1; i <= left.Size(); i++ ){
        Object tSet; tSet.SetList( SYMID_OF_Set );
        tSet.set_state( 1 );
        tSet.PushBackRef( left[i] ); tSet.PushBackRef( right[i] );
        Set( tSet );
      }
    }else{
      for ( u_int i = 1; i <= left.Size(); i++ ){
        Object tSet; tSet.SetList( SYMID_OF_Set );
        tSet.PushBackRef( left[i] ); tSet.PushBackRef( right );
        Set( tSet );
      }
    }
    // if ( ARGV.state() != Set_Temp_State ){
    //   ARGV.SetVoid();
    //   // ARGV = ARGV[2];
    // }
    ReturnObject( ARGV[2] );
  }
  //normal
  if ( left.NumberQ() or left.StringQ()  ){
    zhErroring("赋值","不能对数字或者字符串赋值.") ||
      _Erroring("Set","Left value of set is not allowed to be a Number or String.");
    ReturnError; 
  }
  //check if the left is a Setable or DelaySetable List
  if ( left.ListQ() ){
    //Conjunct
    // if left is a conjuntable object then it has already get its valuetable ref for
    // previous evaluation in reference style 
    if ( left[0].SymbolQ( SYMID_OF_Conjunct ) ){ // Conjunct
      //  case of:   A.a = b
      //dout<<"unprocessed conjunct ARGV = "<<ARGV<<endl;
      Object&conLeft = left[1];
      Object&conRight = left[2];
      EvaKernel->Evaluate( conLeft, false, true );
      //dout<<"unprocessed conjunct ARGV after eval conLeft= "<<ARGV<<endl;
      if ( conLeft.SimpleListQ() and conLeft[0].ValuedSymbolQ() ){
        EvaRecord *rec = EvaKernel->GetEvaRecord( conLeft[0] );
        if ( rec ){
          if ( AttributeQ( rec->attributes , AttributeType::Setable ) ){
            return rec->Call( ARGV );
          }else{
            zhErroring("赋值",conLeft.ToString()+" 不可赋值，因为 "+conLeft[0].ToString()+" 不具备可赋值属性") ||
              _Erroring("Set",conLeft.ToString()+" can not be assigned because "+conLeft[0].ToString()+" does not have Setable Attribute.");
            ReturnHold;
          }
        }
      }
      // left.CopyObject( right );
      // if ( ARGV.state() != Set_Temp_State ) 
      //   ARGV = ARGV[2];
      ReturnNormal;
    }else{
      // Setable
      // EvaKernel->Evaluate( left, false, true ); // ref evaluate without hold
      // so everyfunction that 
      if ( left[0].SymbolQ() ) { //and left[0].idx() != Index0 ){ // a vlaued symbol
        EvaRecord * rec = EvaKernel->GetEvaRecord( left[0] );
        if ( rec ){
          if ( AttributeQ( rec->attributes , AttributeType::Setable ) ){
            // the conjunct pass through has form
            // Set(  f(...), xxx )    xxx can be any form,  f is referenced value if is from value pair
            //dout<<" pass "<<ARGV<< " to "<<left[0]<<endl;
            return rec->Call( ARGV );
          }
          // else{
          //   zhErroring("赋值",left.ToString()+" 不可赋值，因为 "+left[0].ToString()+" 不具备可赋值属性") ||
          //     _Erroring("Set",left.ToString()+" can not be assigned because "+left[0].ToString()+" does not have Setable Attribute.");
          //   ReturnHold;
          // }
        }
      }
      //DelaySetable
      if ( left[0].SimpleListQ() ){ // and left[0][0].idx() != Index0 ){
        EvaRecord * rec = EvaKernel->GetEvaRecord( left[0][0] );
        if ( rec ){
          if ( AttributeQ(rec->attributes ,AttributeType::DelaySetable) ){
            // the conjunct pass through has form
            // Set(  f(...)(...), xxx )    xxx can be any form,  f is referenced value if is from value pair
            //dout<<" pass "<<ARGV<< " to "<<left[0]<<endl;
            return rec->Call( ARGV );
          }
          // }else{
          //   zhErroring("赋值",left[0][0].ToString()+" 不具备可延迟赋值属性") ||
          //     _Erroring("Set",left[0][0].ToString()+" does not have DelaySetable Attribute.");
          //   ReturnHold;
          // }
        }
      }
    }
  }
  //get left
  //check whether pattern
  // if left is  symbol it has not been evaluate yet, previous ref eval is only for list
  Object res = EvaKernel->GetOrNewValuePairRef(left); // the List pair
  if ( not res.NullQ() ){//is not a pattern
    if ( res.ids() != 0 ){//check whether protected
      if (  EvaKernel->AttributeQ(res.ids() ,AttributeType::Protected) ){
        zhErroring("赋值",(left).ToString() +" 处于被保护状态.")||
          _Erroring("Set",(left).ToString() +" is Protected.");
        ReturnError;
      }
    }
    res[2].CopyObject(right);
  }else{//is a pattern
    EvaKernel->RefInsertOrUpdatePairValue(left,right);
  }
  // if ( ARGV.state() != Set_Temp_State )
  //   ARGV = ARGV[2];
  ReturnObject( ARGV[2] );
  // ARGV.SetVoid();
  // return 1;
}

int SystemModule::SetDelayed(Object & ARGV){
  CheckShouldEqual(2);
  Object&left = ARGV(1);  
  Object&right = ARGV(2);
  if ( left.ValuedSymbolQ() ){
    Object( left.idx() )[2] =  right;
    ReturnNull;
  }
  CheckShouldNotBeNumber(1);
  CheckShouldNotBeString(1);
  //get left
  //check whether pattern
  if ( not Pattern::PatternListQ(left ) ){
    Object res = EvaKernel->GetOrNewValuePairRef(left,false,true);
    if ( res.ids() != 0 ){//check whether protected
      if (  EvaKernel->AttributeQ( res.ids(),AttributeType::Protected) ){
        zhErroring("Set",(left).ToString() +(string)" 处于被保护状态, 不能被赋值.") ||
          _Erroring("Set",(left).ToString() +(string)" is Protected."); 
        ReturnError; 
      }
    }
    res[2] = right;
    ReturnNull;
  }else{//is a pattern
    EvaKernel->InsertOrUpdatePairValue(left,right,false,true);
    ReturnNull;
  }
}

int SystemModule::ShowModuleFunctions(Object &ARGV){
  CheckArgsShouldEqual(ARGV,1);
  CheckShouldBeString(1);
  ReturnNormal;
}

int SystemModule::ShowModules(Object &ARGV){
  CheckArgsShouldEqual(ARGV,0);
  ReturnNormal;
}

int SystemModule::SetOutputFormat(Object &ARGV){
  CheckShouldBeWithin(1,2);
  CheckShouldBeNumber(1);
  if ( ARGV.Size() == 2){
    CheckShouldBeNumber(2);
    pond::SetPrintFormat((double)ARGV[1],(double)ARGV[2]);
  } else {
    pond::SetPrintFormat((double)ARGV[1]);
  }
  ReturnNull;
}

int SystemModule::SystemArgv(Object &ARGV)
{
  CheckShouldNoMoreThan(1);
  if ( ARGV.Size() == 0 ){
    //EvaKernel->argc;
    ARGV.SetList();
    for (int i=0; i<EvaKernel->argc; i++ ){
      ARGV.PushBackString( EvaKernel->argv[i] );
    }
    ReturnNormal;
  }
  CheckShouldBeNumber(1);
  int p = (int)ARGV[1];
  if ( p < 0 ) { _Erroring("SystemArgv","Input should be a non-negative Number."); ReturnError; }
  if ( p >= EvaKernel->argc ) ReturnString("");
  ReturnString( EvaKernel->argv[p] );
}

int SystemModule::ShowHelpInfo(Object &ARGV){
  CheckShouldEqual(1);
  EvaRecord*rec = EvaKernel->GetEvaRecord(ARGV(1));
  if ( rec == NULL ){
    cout<<"No help info found for "<<ARGV[1].ToString()<<endl;
  }else{
    cout<<rec->description<<endl;
  }
  ReturnNull;
}

int SystemModule::Clear(Object&ARGV){
  if ( ARGV.Size() == 0 ) ReturnNull;
  for ( u_int i=1;i<=ARGV.Size();i++){
    if ( not ARGV[i].SymbolQ() ) {
      zhErroring("Clear","Clear的参数应该是一个符号.") ||
        _Erroring("Clear","Arguments of Clear is required to be Symbol."); 
      ReturnError; 
    }
    if ( EvaKernel->Clear(ARGV[i]) < 0 ) {
      zhErroring("清除",ARGV[i].ToString()+" 处于被保护中.")||
        _Erroring("Clear",(string)ARGV[i].Key()+" is Protected.");
      ReturnError;
    }
  }
  ReturnNull;
}

int SystemModule::AbsoluteTime(Object &ARGV){
  struct timeval tvs;
  gettimeofday(&tvs,NULL);
  ARGV.SetNumber( tvs.tv_sec+tvs.tv_usec/1000000.0);
  ReturnNormal;
}

int SystemModule::CpuTiming(Object &ARGV){
  CheckShouldEqual(1);
  clock_t start,finish;
  start = clock();
  EvaKernel->Evaluate(ARGV[1]);
  finish = clock();
  ARGV[0].SetSymbol( SYMID_OF_List );
  ARGV.InsertNumber( 1,( (1.0*finish-start)/CLOCKS_PER_SEC ) );
  ReturnNormal;
}

int SystemModule::Timing(Object &ARGV){
  CheckShouldEqual(1);
  struct timeval start,finish;
  gettimeofday( &start,NULL );
  EvaKernel->Evaluate(ARGV[1]);
  gettimeofday( &finish,NULL );
  ARGV[0].SetSymbol( SYMID_OF_List );
  ARGV.InsertNumber( 1,finish.tv_sec - start.tv_sec + (finish.tv_usec - start.tv_usec)/1000000.0 );
  ReturnNormal;
}

int SystemModule::DateList(Object &ARGV){
  ARGV.SetList();
  ReturnNormal;
}

int SystemModule::Sleep(Object&ARGV){
  // if ( ARGV.NumberQ() ){
  //   pond::sleep_ms( ARGV.Number()*1000 );
  // }
  CheckShouldEqual(1);
  CheckShouldBeNumber(1);
  pond::sleep_micro( ARGV[1].Number()*1000 );
  ReturnNull;
}


int func_else(Object&ARGV,bool condi){ // if ( a , else(b,c) )
  //dout<<"into func_else "<<ARGV<<", condi =" <<condi<<endl;
  if ( ARGV.Size() != 2 ) return -1;
  if ( condi > 0 ){ // if ( a , else(b,c) )  a is true
    EvaKernel->Evaluate( ARGV[1] );
    ARGV = ARGV[1];
    ReturnNormal;
  }else{ // if (a, else(b,c) )   a is false
    EvaKernel->Evaluate( ARGV[2] );
    ARGV = ARGV[2];
    ReturnNormal;
  }
}

int func_elif(Object&ARGV,bool condi){ // if (a, elif(b,(c),d) )
  //dout<<"into func_elif "<<ARGV<<", condi =" <<condi<<endl;
  if ( ARGV.Size() != 3 ) return -1;
  if ( condi > 0 ){ // if (a , elif(b,c,d) )   a is true, return evaluated b
    EvaKernel->Evaluate( ARGV[1] );
    ARGV = ARGV[ 1 ];
    ReturnNormal;
  }else{ // need to check if else or elif
    if ( not ARGV[2].ListQ( SYMID_OF_Parenthesis ) ){
      zhErroring("elif","elif 的条件项应该放在 () 中" ) ||
        _Erroring("elif","elif condition should enclosed with ()" );
      ReturnError;
    }
    EvaKernel->Evaluate( ARGV[2] ); // evaluate c
    char res = ARGV[2].Boolean();
    //dout<<" cal own condi to "<<int(res)<<endl;
    if ( res < 0 ){ // cannot evaluate, rewrite to if(c,d)
      ARGV.Delete(1);
      ARGV[0].SetSymbol( SYMID_OF_if );
      return res;
    }

    if ( ARGV[3].ListQ( SYMID_OF_else ) ){ // d is else
      res = func_else( ARGV[3], res );
      ARGV = ARGV[3];
      return res;
    }else if ( ARGV[3].ListQ( SYMID_OF_elif ) ){ // d is elif
      res = func_elif( ARGV[3], res );
      ARGV = ARGV[3];
      return res;
    }else{ //  d is normal expression
      if ( res > 0 ){
        EvaKernel->Evaluate(ARGV[3]);
        ARGV = ARGV[3];
        ReturnNormal;
      }
      ReturnNull;
    }
  }
}

int SystemModule::PD_if(Object&ARGV){
  /*zh:
    ----------------------
    if ( condition ){
      expr_true;
    }
    ----------------------
    if ( condition ){
      expr_true;
    } else {
      expr_false;
    }
    ----------------------
   */
  CheckShouldEqual(2);
  if ( not ARGV[1].ListQ( SYMID_OF_Parenthesis ) ){
    zhErroring("if","if 条件项应该放在 () 中" ) ||
      _Erroring("if","if condition should enclosed with ()" );
    ReturnError;
  }
  EvaKernel->Evaluate(ARGV[1]);
  char res = ARGV[1].Boolean();
  if ( res < 0  ) ReturnHold; 
  if ( ARGV[2].ListQ( SYMID_OF_else ) ){ // if (a, else(b,c) )
    res = func_else( ARGV[2], res );
    ARGV = ARGV[2];
    return res;
  }else if ( ARGV[2].ListQ( SYMID_OF_elif ) ){ // if(a elsif(b,c,d) )
    res = func_elif( ARGV[2], res );
    ARGV = ARGV[2];
    return res;
  }else{ // if(a,b),  b is simple expression
    if ( res > 0 ){
      EvaKernel->Evaluate( ARGV[2] );
      ARGV = ARGV[2];
      ReturnNormal;
    }else if ( res == 0 ){
      ReturnNull;
    }
  }
  ReturnHold;
}

int SystemModule::PD_while(Object&ARGV) {
  CheckShouldEqual(2);
  if ( not ARGV[1].ListQ( SYMID_OF_Parenthesis ) ){
    zhErroring("while","while 后面应该紧跟括号.") ||
      _Erroring("while","while should be followed with ().") ;
  }
  Object&condition = ARGV(1);
  Object&body = ARGV(2);
  Object cond, expr, bodyelse;
  if ( body.ListQ(SYMID_OF_else) and body.Size() == 2 ){
    bodyelse = body[2];
    body     = body[1];
  }
  for(;;){
    cond.CopyObject( condition );
    EvaKernel->Evaluate( cond );
    if ( cond.Boolean() > 0 ){ //evaluate expr
      expr.CopyObject( body );
      EvaKernel->Evaluate( expr );
      // return
      if ( EvaKernel->statusCode == 88 ){ARGV.SetVoid(); ReturnNormal;}
      if ( EvaKernel->statusCode == 66 ){EvaKernel->statusCode = 0; continue;}
      if ( EvaKernel->statusCode == 44 ){
        bodyelse = NullObject;
        EvaKernel->statusCode = 0;
        break;
      }
      continue;
    }
    break;
  }
  if ( not bodyelse.NullQ() ) EvaKernel->Evaluate( bodyelse );
  ReturnNull;
}

static INIT_SYMID_OF(range);
int SystemModule::PD_for(Object&ARGV){
  //dout<< "dealing for "<< ARGV <<endl;
  CheckShouldEqual(2);
  if ( not ARGV[1].ListQ( SYMID_OF_Parenthesis ) ){
    zhErroring("for","for 后面应该紧跟括号.") ||
      _Erroring("for","for should be followed with ().");
  }
  if ( ARGV[1].Size() < 1 ){
    zhErroring("for","for 括号中参数太少") ||
      _Erroring("for","for has too few arguments in().");
  }
  //dout<<"for ARGV[1]"<<ARGV<<endl;
  if ( ( ARGV[1][1].ListQ( SYMID_OF_in ) )
         and ARGV[1][1].Size() == 2 ){ // for ( i in {1,2,3,4} ){ expr }
    Object&in = ARGV(1)(1);
    Object&var = in[1];
    Object&body= ARGV(2);
    Object bodyelse;
    if ( body.ListQ( SYMID_OF_else ) and body.Size() == 2 ){
      bodyelse = body[2];
      body = body[1];
    }

    if ( in[2].ListQ(SYMID_OF_range) ){
      // as single variable interate
      if ( not var.SymbolQ() ){
        zhErroring("for:in:range","循环变量应该是一个符号.") ||
          _Erroring("for:in:range","the iterator variable should be a symbol.");
      }
      double start, incr; long N;
      if ( ListModule::range_iter(in[2],N,start,incr) == 0 ){
        zhErroring("for:in:range","range 不合法") ||
          _Erroring("for:in:range","range is not valid.");
        ReturnError;
      }
      Object&vobj = EvaKernel->StackPushCopy(var,var)[2];
      Object newexpr;
      // EvaKernel->PreEvaluate( body );
      for(long i=0; i<N; i++ ){
        vobj.SetNumber( start+i*incr );
        newexpr.CopyObject( body , true);
        EvaKernel->Evaluate( newexpr );
        if ( EvaKernel->statusCode == 88 ){ARGV.SetVoid(); ReturnNormal;}
        if ( EvaKernel->statusCode == 66 ){EvaKernel->statusCode = 0; continue;}
        if ( EvaKernel->statusCode == 44 ){
          bodyelse = NullObject;
          EvaKernel->statusCode = 0;
          break;
        }
      }
      if ( not bodyelse.NullQ() ) EvaKernel->Evaluate( bodyelse );
      EvaKernel->StackPop();
      ReturnNull;
    }else{
      //dout<<"deal for in with in = "<<in<<endl;
      EvaKernel->Evaluate( in[2] );
      //dout<<"after deal in = "<<in<<endl;
      Object&lists = in[2];
      if ( not lists.ListQ() ){
        zhErroring("for","in 后面的参数应该是一个列表.") ||
          _Erroring("for","argument after in is required to be a List.");
        ReturnError;
      }

      if ( var.SymbolQ() ){
        Object&vobj = EvaKernel->StackPushCopy(var,var)[2];
        Object newexpr;
        for (auto iter= lists.Begin(); iter != lists.End(); iter++){
          vobj =  *iter ;
          newexpr.CopyObject( body );
          EvaKernel->Evaluate( newexpr );
          // return continue break from pond
          if ( EvaKernel->statusCode == 88 ){ARGV.SetVoid(); ReturnNormal;}
          if ( EvaKernel->statusCode == 66 ){EvaKernel->statusCode = 0; continue;}
          if ( EvaKernel->statusCode == 44 ){
            bodyelse = NullObject;
            EvaKernel->statusCode = 0;
            break;
          }
        }
        if ( not bodyelse.NullQ() ) EvaKernel->Evaluate( bodyelse );
        EvaKernel->StackPop();
        ReturnNull;
      }
      if ( var.ListQ() ){
        int ptr = EvaKernel->GetStackPtr();
        Object vobj_list; vobj_list.SetList();
        for ( auto iter = var.Begin(); iter != var.End(); iter ++ ){
          vobj_list.PushBackRef(  EvaKernel->StackPushCopy(*iter,*iter)[2] );
        }
        Object newexpr;
        for ( auto iter = lists.Begin(); iter != lists.End(); iter++ ){
          if ( iter->Size() != vobj_list.Size() ) { 
            zhErroring("for::in::shape","列表和变量列表的形状不一致.") ||
              _Erroring("for::in::shape","List shape is not consistent with variable list."); 
            ReturnError; 
          }
          for ( auto viter=vobj_list.Begin(),liter=(*iter).Begin();viter!=vobj_list.End();viter ++,liter++){
            (*viter).CopyObject( *liter );
          }
          newexpr = NullObject;
          newexpr.CopyObject( body );
          EvaKernel->Evaluate( newexpr );
          // return continue break from pond
          if ( EvaKernel->statusCode == 88 ){ARGV.SetVoid(); ReturnNormal;}
          if ( EvaKernel->statusCode == 66 ){EvaKernel->statusCode = 0; continue;}
          if ( EvaKernel->statusCode == 44 ){
            bodyelse = NullObject;
            EvaKernel->statusCode = 0;
            break;
          }
        }
        if ( not bodyelse.NullQ() ) EvaKernel->Evaluate( bodyelse );
        EvaKernel->SetStackToPtr( ptr );
      }
    }
    zhErroring("for:in","variable should be a symbol or list of symbols.")||
      _Erroring("for:in","变量应该是一个符号变量或者一个由符号变量构成的列表."); 
    ReturnHold;
  }
  zhErroring("for","for clause should be in form for ( i in [n1,n2,...nN] ) expr;")||
    _Erroring("for","for 语句应该形为 for ( i in [n1,n2,...nN] ) expr;"); 
  ReturnHold;
}

int SystemModule::PD_in(Object&ARGV){
  //"test if one obj is in list" ){ // a in {...} a in [...]    or  a in (...)
  CheckShouldEqual(2);
  //dout<<"deal in with ARGV = "<< ARGV<<endl;
  bool exist = false;
  Object &list = ARGV[2];
  EvaKernel->Evaluate( list, false, true );
  CheckShouldBeList(2);
  Object &var  = ARGV[1];
  EvaKernel->Evaluate( var, false, true );
  if ( list.ListQ( SYMID_OF_Dict ) ){
    Object res = list.DictGetPair( var );
    if ( res.NullQ() ) ReturnNumber(SYMID_OF_False);
    ReturnNumber(SYMID_OF_True);
  }
  for (auto iter = list.Begin(); iter != list.End(); iter ++ ){
    if  ( var == (*iter) ){
      ReturnSymbol(SYMID_OF_True);
    }
  }
  ReturnSymbol(SYMID_OF_False );
}

int SystemModule::PD_RunPageJS(Object&ARGV){
  //"在前端页面直接执行JS代码"
  // in form RunJS[ " console.log("this is good"); ]
  
  ReturnNormal;
}

int SystemModule::PD_RunKernelJS(Object&ARGV){ //"在运算单元中直接执行JS代码"
  // in form RunJS[ " console.log("this is good"); ]
  
  ReturnNormal;
}

int SystemModule::PD_continue(Object&ARGV){//"结束本轮循环，并开始下一轮循环" 
  EvaKernel->statusCode = 66;
  ReturnNull;
}

int SystemModule::PD_break(Object&ARGV){ //, "跳出当前循环"
  EvaKernel->statusCode = 44;
  ReturnNull;
}

int SystemModule::PD_return(Object&ARGV){ //"将当前值作为当前代码域返回值并返回" 
  /*
    Attributes: HoldAll
   */
  //dout<<"get into return ARGV = "<<ARGV<<endl;
  EvaKernel->statusCode = 88;
  if ( ARGV.Size() >= 1 ){
    ARGV = ARGV[ ARGV.Size() ];
  }else{
    ARGV.SetVoid();
  }
  EvaKernel->statusObject = ARGV;
  ReturnNormal;
}

int function_def_process(Object&ARGV){ //, "通用函数定义" 
  // used in form:   function f(a,b,x,y=1,z=3){...}       inner form ...(fname, (a,b...), {...} )
  // return or trandi way to return value
  // *args, **kwargs
  CheckShouldEqual(2);
  if ( not ARGV[1].ListQ() or not ARGV[1][0].SymbolQ() ){
    zhErroring(ARGV[0].ToString(),"函数声明格式错误: "+ARGV[1].ToString() )||
      _Erroring(ARGV[0].ToString(),"function declaration is in the wrong form: "+ARGV[1].ToString() );
    ReturnError;
  }
  CheckShouldBeListWithHead(2, SYMID_OF_ExpressionList );
  //dout<<"into def with ARGV ="<<ARGV<<endl;
  // deal those with default values
  Object & args = ARGV[1];
  Object dict(__List__,SYMID_OF_Dict);
  Object packVar;
  Object dictPackVar;
  int i = 1;
  while ( i<= args.Size() ){
    if ( args[i].SymbolQ() ){
      if ( not packVar.NullQ() ){
        zhErroring(ARGV[0].ToString(),"位置参数出现在参数包后") ||
          _Erroring(ARGV[0].ToString(),"Positional argument follows arguments pack");
        ReturnError;
      }
      if ( not dictPackVar.NullQ() ){
        zhErroring(ARGV[0].ToString(),"位置参数出现在字典参数包后") ||
          _Erroring(ARGV[0].ToString(),"Positional argument follows keyword arguments pack");
        ReturnError;
      }
    }else if ( args[i].PairQ( SYMID_OF_Set ) ){
      EvaKernel->Evaluate( args[i][2] );
      // dict.DictInsertPairRef( args[i] );
      Object::iterator iter;
      bool res = dict.DictGetPosition( args[i][1], iter ); 
      if ( res == 0 ){
        zhErroring(ARGV[0].ToString(),"字典参数 "+args[i][1].ToString()+" 被重复指定") ||
          _Erroring(ARGV[0].ToString(),"Multiple Keyword argument "+args[i][1].ToString()+" specified.");
        ReturnError;
      }
      dict.InsertRef(iter, args[i] );
      i++; //args.Delete(i);
      continue;
    }else if ( args[i].PairQ( SYMID_OF_SetDelayed ) ){
      zhErroring(ARGV[0].ToString(),"延迟赋值不能用来给参数提供默认值") ||
        _Erroring(ARGV[0].ToString(),"SetDelayed can not be used to provide default value");
      ReturnError;
    }else if ( args[i].ListQ( SYMID_OF_Unpack )  ){
      if ( not packVar.NullQ() ){
        zhErroring(ARGV[0].ToString(),"参数包只能又一个") ||
          _Erroring(ARGV[0].ToString(),"Multi argument pack");
        ReturnError;
      }
      if ( args[i].Size() != 1 and not args[i][1].SymbolQ() ){
        zhErroring(ARGV[0].ToString(),"参数包应该具有符号名字") ||
          _Erroring(ARGV[0].ToString(),"Arguments pack should has a symbol name");
        ReturnError;
      }
      packVar = args[i][1];
      //dout<<"try delete pack var at "<<i <<" args ="<<args<<endl;
      args.Delete(i);
      continue;
    }else if ( args[i].ListQ( SYMID_OF_UnpackDict )  ){
      if ( not dictPackVar.NullQ() ){
        zhErroring(ARGV[0].ToString(),"字典参数包只能有一个") ||
          _Erroring(ARGV[0].ToString(),"Multi keyword argument pack");
        ReturnError;
      }
      if ( args[i].Size() != 1 and not args[i][1].SymbolQ() ){
        zhErroring(ARGV[0].ToString(),"字典参数包应该具有符号名字") ||
          _Erroring(ARGV[0].ToString(),"Dict arguments pack should has a symbol name");
        ReturnError;
      }
      dictPackVar = args[i][1];
      //dout<<"try delete dict pack var at "<<i <<" args ="<<args<<endl;
      args.Delete(i);
      continue;
    }else{
      zhErroring(ARGV[0].ToString(),args[i].ToString() + "不能用来作为参数") ||
        _Erroring(ARGV[0].ToString(),args[i].ToString() + " can not be used to as argument");
      ReturnError;
    }
    i++;
  }
  ARGV.PushBackRef(dict);
  ARGV.PushBackRef(packVar);
  ARGV.PushBackRef(dictPackVar);
  // save processed value to valueTable
  ARGV[0].SetSymbol( SYMID_OF_FUNCTION$DEFINED$ );
  return 1;
}

int SystemModule::PD_def(Object&ARGV){ //, "通用函数定义" 
  function_def_process( ARGV );
  // // used in form:   function f(a,b,x,y=1,z=3){...}       inner form ...(fname, (a,b...), {...} )
  // // return or trandi way to return value
  // // *args, **kwargs
  // CheckShouldEqual(2);
  // if ( not ARGV[1].ListQ() or not ARGV[1][0].SymbolQ() ){
  //   zhErroring(ARGV[0].ToString(),"函数声明格式错误: "+ARGV[1].ToString() )||
  //     _Erroring(ARGV[0].ToString(),"function declaration is in the wrong form: "+ARGV[1].ToString() );
  //   ReturnError;
  // }
  // CheckShouldBeListWithHead(2, SYMID_OF_ExpressionList );
  // //dout<<"into def with ARGV ="<<ARGV<<endl;
  // // deal those with default values
  // Object & args = ARGV[1];
  // Object dict(__List__,SYMID_OF_Dict);
  // Object packVar;
  // Object dictPackVar;
  // int i = 1;
  // while ( i<= args.Size() ){
  //   if ( args[i].SymbolQ() ){
  //     if ( not packVar.NullQ() ){
  //       zhErroring(ARGV[0].ToString(),"位置参数出现在参数包后") ||
  //         _Erroring(ARGV[0].ToString(),"Positional argument follows arguments pack");
  //       ReturnError;
  //     }
  //     if ( not dictPackVar.NullQ() ){
  //       zhErroring(ARGV[0].ToString(),"位置参数出现在字典参数包后") ||
  //         _Erroring(ARGV[0].ToString(),"Positional argument follows keyword arguments pack");
  //       ReturnError;
  //     }
  //   }else if ( args[i].PairQ( SYMID_OF_Set ) ){
  //     EvaKernel->Evaluate( args[i][2] );
  //     // dict.DictInsertPairRef( args[i] );
  //     Object::iterator iter;
  //     bool res = dict.DictGetPosition( args[i][1], iter ); 
  //     if ( res == 0 ){
  //       zhErroring(ARGV[0].ToString(),"字典参数 "+args[i][1].ToString()+" 被重复指定") ||
  //         _Erroring(ARGV[0].ToString(),"Multiple Keyword argument "+args[i][1].ToString()+" specified.");
  //       ReturnError;
  //     }
  //     dict.InsertRef(iter, args[i] );
  //     i++; //args.Delete(i);
  //     continue;
  //   }else if ( args[i].PairQ( SYMID_OF_SetDelayed ) ){
  //     zhErroring(ARGV[0].ToString(),"延迟赋值不能用来给参数提供默认值") ||
  //       _Erroring(ARGV[0].ToString(),"SetDelayed can not be used to provide default value");
  //     ReturnError;
  //   }else if ( args[i].ListQ( SYMID_OF_Unpack )  ){
  //     if ( not packVar.NullQ() ){
  //       zhErroring(ARGV[0].ToString(),"参数包只能又一个") ||
  //         _Erroring(ARGV[0].ToString(),"Multi argument pack");
  //       ReturnError;
  //     }
  //     if ( args[i].Size() != 1 and not args[i][1].SymbolQ() ){
  //       zhErroring(ARGV[0].ToString(),"参数包应该具有符号名字") ||
  //         _Erroring(ARGV[0].ToString(),"Arguments pack should has a symbol name");
  //       ReturnError;
  //     }
  //     packVar = args[i][1];
  //     //dout<<"try delete pack var at "<<i <<" args ="<<args<<endl;
  //     args.Delete(i);
  //     continue;
  //   }else if ( args[i].ListQ( SYMID_OF_UnpackDict )  ){
  //     if ( not dictPackVar.NullQ() ){
  //       zhErroring(ARGV[0].ToString(),"字典参数包只能有一个") ||
  //         _Erroring(ARGV[0].ToString(),"Multi keyword argument pack");
  //       ReturnError;
  //     }
  //     if ( args[i].Size() != 1 and not args[i][1].SymbolQ() ){
  //       zhErroring(ARGV[0].ToString(),"字典参数包应该具有符号名字") ||
  //         _Erroring(ARGV[0].ToString(),"Dict arguments pack should has a symbol name");
  //       ReturnError;
  //     }
  //     dictPackVar = args[i][1];
  //     //dout<<"try delete dict pack var at "<<i <<" args ="<<args<<endl;
  //     args.Delete(i);
  //     continue;
  //   }else{
  //     zhErroring(ARGV[0].ToString(),args[i].ToString() + "不能用来作为参数") ||
  //       _Erroring(ARGV[0].ToString(),args[i].ToString() + " can not be used to as argument");
  //     ReturnError;
  //   }
  //   i++;
  // }
  // ARGV.PushBackRef(dict);
  // ARGV.PushBackRef(packVar);
  // ARGV.PushBackRef(dictPackVar);
  // // save processed value to valueTable
  // ARGV[0].SetSymbol( SYMID_OF_FUNCTION$DEFINED$ );
  EvaKernel->InstantInsertOrUpdatePairValue(ARGV[1][0], ARGV);
  ReturnNull;
}

#define _err_return ({ ARGV[0] = ARGV[0][1][0]; ReturnError;})
int SystemModule::PD_FUNCTION$DEFINED$(Object&ARGV){
  // 函数调用 DelayFunction
  //dout<<"FUNCTION$DEFINED$ called with ARGV="<<ARGV<<endl;
  //EvaKernel->GetMemInfo();
  // check function form
  // argv should be evaluated before newContext


  Object pairTable(__List__,SYMID_OF_Dict);
  Object &func = ARGV[0];
  if ( not func.ListQ() or func.Size() != 5 or
       not func[3].ListQ() or  // dict, {} will read into expression list
       not ( func[4].NullQ() or func[4].SymbolQ() ) or
       not ( func[5].NullQ() or func[5].SymbolQ()  )
  ){
    zhErroring("函数调用","非法的函数调用")||
      _Erroring("DefinedFunction","Invalid function call.");
    _err_return;
  }

  //dout<<" try create new Context with func="<<func<<endl;
  Object &funcargs = func[1];
  Object &funcbody = func[2];
  Object &dict = func[3];
  //dout<<"current dict is "<<dict<<endl;
  //dout<<"current pairTable "<<pairTable<<endl;
  Object &packVar     = func[4];
  Object &dictPackVar = func[5];
  // match and create vars 
  //check if positional argument follows keyword argument
  bool isDict = false;
  Object pack;
  if ( not packVar.NullQ() ){
    Object pair(__List__,SYMID_OF_KeyValuePair);
    pack.SetList( SYMID_OF_List);
    pair.SetPairRef( packVar, pack ); 
    pairTable.DictInsertOrUpdatePairRef( pair );
  }
  Object packDict;
  if ( not func[5].NullQ() ){
    Object pair(__List__,SYMID_OF_KeyValuePair);
    packDict.SetList( SYMID_OF_Dict );
    pair.SetPairRef( dictPackVar, packDict ); 
    pairTable.DictInsertOrUpdatePairRef( pair );
  }

  //dout<<"argv process prepare done"<<endl;
  //EvaKernel->GetMemInfo();
  //dout<<"try deal real argument one by one"<<endl;
  //int pos = 1;
  for(int i = 1; i <= ARGV.Size(); i++ ){
    //dout<<"deal argv[ "<<i<<"] = "<<ARGV[i]<<endl;
    if ( ARGV[i].ListQ( SYMID_OF_Unpack ) ){
      //dout<<"deal unpack"<<ARGV[i]<<endl;
      EvaKernel->Evaluate( ARGV[i] );
      ARGV.InsertRef(ARGV.Begin()+i, ARGV[i].Begin(), ARGV[i].End() );
      ARGV.Delete(i);
      if ( i>ARGV.Size() ) break;
    }else if ( ARGV[i].ListQ( SYMID_OF_UnpackDict ) ){ // deal one by one key maybe string
      //dout<<"deal unpackdict "<<ARGV[i]<<endl;
      EvaKernel->Evaluate( ARGV[i] );
      if ( not ARGV[i].ListQ( SYMID_OF_Sequence ) ){
        zhErroring("函数调用",funcargs[0].ToString()+"() 试图对非字典数据进行字典解包")||
          _Erroring("Function call",funcargs[0].ToString()+"() try to dict-unpack none dict type.");
        _err_return;
      }
      ARGV.InsertRef(ARGV.Begin()+i, ARGV[i].Begin(), ARGV[i].End() );
      ARGV.Delete(i);
      if ( i>ARGV.Size() ) break;
    }// unpack will insert new arguments inpalce which will processed by following logic
    
    // keyword argument
    if ( ARGV[i].PairQ( SYMID_OF_Set ) or ARGV[i].PairQ( SYMID_OF_KeyValuePair ) ){
      //dout<<"deal keyword arg "<<i<<" = "<<ARGV[i]<<endl;
      isDict = true;
      Object::iterator iter;
      //dout<<"try insert to dict"<<endl;
      if ( ARGV[i][1].StringQ() ) ARGV[i][1].SetSymbol( ARGV[i][1].Key() );

      bool res = pairTable.DictGetPosition( ARGV[i][1], iter ); 
      //dout<<"find result ="<<res<<endl;
      if ( res == 0 ){ // find position
        zhErroring("函数调用",funcargs[0].ToString()+"() 重复获得字典参数 "+ARGV[i][1].ToString()+"的值")||
          _Erroring("Function call",funcargs[0].ToString()+"() got multiple values for keyword argument "+ARGV[i][1].ToString()+"");
        _err_return;
      }else{
        // find if a keyword para
        Object::iterator iterdic;
        res = dict.DictGetPosition( ARGV[i][1], iterdic );
        if ( res == 0 ){
          //dout<<"is a keyword para"<<endl;
          EvaKernel->Evaluate( ARGV[i][2] );
          pairTable.InsertRef(iter, ARGV[i] );
          dict.Delete( iterdic );
          continue;
        }
        // find if a possition para
        //dout<<"check if a positional argument"<<endl;
        for (int j = i; j <= funcargs.Size(); j++ ){
          if ( funcargs[j].SymbolQ() and ARGV[i][1] == funcargs[j] ){
            EvaKernel->Evaluate( ARGV[i][2] );
            pairTable.InsertRef(iter, ARGV[i] );
            funcargs.Delete( j );
            continue;
          }
        }
        // unexpected maybe if no packDict exists
      }
      if ( packDict.NullQ() ){
        zhErroring("函数调用",funcargs[0].ToString()+"() 遇到预期之外的参数 "+ARGV[i].ToString() )||
          _Erroring("DefinedFunction",funcargs[0].ToString()+"() called with unexpected arugment: "+ARGV[i].ToString() );
        _err_return;
      }
      EvaKernel->Evaluate( ARGV[i][2] );
      Object pair(__List__,SYMID_OF_KeyValuePair);
      pair.SetPairRef( Object(__String__,ARGV[i][1].Key() ), ARGV[i][2] );
      packDict.DictInsertPairRef( pair );
      continue;

    }else{ // positional argument
      //dout<<"deal positional argument "<<i<<" = "<<ARGV[i]<<endl;
      if ( isDict ){
        zhErroring("函数调用",funcargs[0].ToString()+"() 位置参数处于字典参数之后")||
          _Erroring("DefinedFunction",funcargs[0].ToString()+"() called with positional argument follows  kwyword argument");
        _err_return;
      }
      if ( i <= funcargs.Size() ){
        EvaKernel->Evaluate( ARGV[i] );
        if ( funcargs[i].SymbolQ() ){// one one positional corresponding
          //dout<<"try insert to dict"<<endl;
          Object pair;   pair.SetPairRef( funcargs[i] , ARGV[i] );
          pairTable.DictInsertOrUpdatePairRef( pair );
          //dout<<"insert done"<<endl;
          continue;
        }else{ // corresponding position is a  positional argument, while it cor. to a optional argument
          //dout<<" positional input "<<ARGV[i]<<" to optional arg "<<funcargs[i]<<endl;
          Object::iterator iter;
          bool res = pairTable.DictGetPosition( funcargs[i][1], iter ); 
          if ( res == 0 ){
            zhErroring("函数调用",funcargs[0].ToString()+"() 重复获得字典参数 "+ARGV[i][1].ToString()+"的值")||
              _Erroring("Function call",funcargs[0].ToString()+"() got multiple values for keyword argument "+ARGV[i][1].ToString()+"");
            _err_return;
          }
          Object pair;   pair.SetPairRef( funcargs[i][1], ARGV[i] );
          //dout<<"try insertt "<<pair<<" to pairTable "<<pairTable<<endl;
          pairTable.InsertRef( iter, pair );
          //dout<<"after insert pairTable ="<<pairTable<<endl;

          Object::iterator iterdic;
          dict.DictGetPosition( funcargs[i][1], iterdic );
          dict.Delete( iterdic );
          continue;
        }
      }
      // extra real positional arguments, push to packVar list, or report error
      if ( pack.NullQ() ){
        zhErroring("函数调用",funcargs[0].ToString()+"() 调用使用了额外的位置参数")||
          _Erroring("DefinedFunction",funcargs[0].ToString()+"() called with extra positional argument");
        _err_return;
      }
      EvaKernel->Evaluate( ARGV[i] );
      pack.PushBackRef( ARGV[i] );
      continue;
    }

  }
  //dout<<"arguments are processed"<<endl;
  //dout<< "args done with isDict="<<isDict<<" funcargs="<<funcargs<<" ARGV="<<ARGV<<endl;
  //dout<<"dict="<<dict<<endl;
  //dout<<"pairTable="<<pairTable<<endl;
  //EvaKernel->GetMemInfo();

  //dout<<"process argv left"<<endl;

  if ( ARGV.Size() < funcargs.Size() and not funcargs[ ARGV.Size()+1 ].PairQ() ){
    zhErroring("函数调用",funcargs[0].ToString()+"() 未提供值给 "+funcargs[ARGV.Size()+1].ToString() )||
      _Erroring("DefinedFunction",funcargs[0].ToString()+"() called without value for "+funcargs[ARGV.Size()+1].ToString() );
    _err_return;
  }
  //dout<<"try insert extra optional argument values into pairTable dict="<<dict<<endl;
  // insert default values to pairTable
  while ( dict.Size() > 0 ){
    pairTable.DictInsertPairRef( dict.Last() );
    dict.PopBack();
  }
  //dout<<"args in ARGV done with new ARGV to be "<< ARGV<<endl;

  //dout<<"dict="<<dict<<endl;
  //dout<<"pairTable="<<pairTable<<endl;
  //dout<<"funcbody="<<funcbody<<endl;
  //dout<<"arguments are processed try new context"<<endl;
  // EvaKernel->GetMemInfo();

  // ReturnHold;
  EvaKernel->newContext(pairTable);
  //dout<<"funcbody = "<<funcbody<<endl;
  // EvaKernel->GetMemInfo();
  EvaKernel->Evaluate( funcbody );
  //dout<<"funcbody = "<<funcbody<<endl;
  // EvaKernel->GetMemInfo();
  if ( EvaKernel->statusCode == 88 ){
    //dout<<"value is return by return clause"<<endl;
    ARGV = EvaKernel->statusObject;
  }else{ // no return just return the default result
    //dout<<"value is return by normal evaluation"<<endl;
    ARGV =  funcbody;
    //dout<<"value by func body "<<ARGV<<" objid ="<<ARGV.objid<<endl;
  }
  EvaKernel->deleteContext();
  //dout<<"after delete Context()"<<endl;
  // EvaKernel->GetMemInfo();
  EvaKernel->statusCode = 0;
  EvaKernel->statusObject = NullObject;
  //dout<<"value is return by normal evaluation "<<ARGV<<" objid ="<<ARGV.objid<<","<<EvaKernel->statusObject.objid<<endl;
  ReturnNormal;
}
#undef _err_return

int SystemModule::PD_Unpack(Object&ARGV){ //, "Unpack list to its parent list"
  CheckShouldEqual(1);
  CheckShouldBeList(1);
  //dout<<"try to unpack "<< ARGV<<endl;
  ARGV[1][0].SetSymbol( SYMID_OF_Sequence ); 
  ARGV = ARGV[1];
  ReturnNormal;
}

int SystemModule::PD_UnpackDict(Object&ARGV){ //, "Unpack list to its parent list"
  CheckShouldEqual(1);
  CheckShouldBeList(1);
  //dout<<"try to unpack "<< ARGV<<endl;
  ARGV[1][0].SetSymbol( SYMID_OF_Sequence ); 
  ARGV = ARGV[1];
  ReturnNormal;
}


int SystemModule::PD_var(Object&args){
  /*
    将变量声明为本地变量
    var x = 99,y=100
  */
  for (int i = 1; i<= args.Size(); i++ ){
    Object pair(__List__,SYMID_OF_Set);
    if ( args[i].SymbolQ() ){
      pair.SetPairRef( args[i], args[i].Copy() );
      EvaKernel->StackPushPairRef( pair );
    }else if ( args[i].ListQ( SYMID_OF_Part ) ){
      Object result;
      ListModule::GetPartList( args[i][1], args[i], 2, result );
      if ( not result.SymbolQ() ){
        zhErroring("var","本地变量定义 "+result.ToString()+" 需要是一个符号.") ||
          _Erroring("var","Local variable specification "+result.ToString()+ " should be a symbol.");
        ReturnError;
      }
      pair.SetPairRef(result, result.Copy() );
      EvaKernel->StackPushPairRef( pair );
    }else if ( args[i].PairQ(SYMID_OF_Set ) ){
      if ( args[i][1].SymbolQ() ){
        EvaKernel->Evaluate( args[i][2] );
        EvaKernel->StackPushPairRef( args[i] );
      }else if ( args[i][1].ListQ(SYMID_OF_Part) ){ //
        Object result;
        ListModule::GetPartList( args[i][1][1], args[i][1], 2, result );
        if ( not result.SymbolQ() ){
          zhErroring("var","本地变量定义 "+result.ToString()+" 需要是一个符号.") ||
            _Erroring("var","Local variable specification "+result.ToString()+ " should be a symbol.");
          ReturnError;
        }
        pair.SetPairRef( result, args[i][2] );
        EvaKernel->StackPushPairRef( pair );
      }
    }else{
      zhErroring("本地变量声明","错误的局部变量定义表达式"+args[i].ToString() ) ||
        _Erroring("var","invalid local variable defination expression "+args[i].ToString() );
      ReturnError;
    }
  }
  ReturnNull;
}

int SystemModule::PD_context(Object&args){
  /*
    将其中代码置于新的上下文中运行
   */
  if ( args.Size() == 1 and args[1].ListQ( SYMID_OF_ExpressionList ) ){
    args = args[1];
  }else{
    args[0].SetSymbol( SYMID_OF_ExpressionList );
  }
  EvaKernel->newContext();
  int res = EvaKernel->Evaluate( args );
  if ( EvaKernel->statusCode == 88 ){
    args = EvaKernel->statusObject;
  }
  EvaKernel->statusCode = 0;
  EvaKernel->statusObject = NullObject;
  EvaKernel->deleteContext();
  return res;
}

int SystemModule::PD_type(Object&args){
  CheckShouldEqual(1);
  switch( args[1].type() ){
  case ObjectType::Number : ReturnSymbol( "Number" );
  case ObjectType::String : ReturnSymbol( "String" );
  case ObjectType::Symbol : ReturnSymbol( "Symbol" );
  case ObjectType::List   : ReturnSymbol( "List"   );
  }
  zhErroring("Object::type","") ||
    _Erroring("Object::type","");
  ReturnError;
}

int SystemModule::PD_ArrowFunction(Object&ARGV){
  //dout<<"into arrow function "<<ARGV<<endl;
  ARGV[1][0].SetSymbol(SYMID_OF_ArrowFunction);
  function_def_process(ARGV);
  //dout<<"processed to"<<ARGV<<endl;
  return 1;
}

int SystemModule::PD_SetParallelMode(Object&Argv)
{
  CheckShouldEqual(1);
  CheckShouldBeString(1);
  if ( strcasecmp( Argv[1].Key(), "gpu" ) == 0 ){
    pond::SetParallelMode( ParallelModeGpu );
  }else if ( strcasecmp( Argv[1].Key(), "cpu" ) == 0 ){
    pond::SetParallelMode( ParallelModeCpu );
  }else{
    ThrowError("SetParallelMode","Running mode "+Argv[1].ToString()+" is not recongnized.");
  }
  ReturnNull;
}

int SystemModule::PD_GetParallelMode(Object&Argv)
{
  CheckShouldEqual(0);
  if ( pond::GetParallelMode() == ParallelModeGpu ){
    ReturnString("gpu");
  }
  ReturnString("cpu");
}

int SystemModule::PD_GPUDeviceQ(Object&Argv)
{
  if ( pond::Execute("nvidia-smi -L >/dev/null 2>&1") != 0 ){
    ReturnString("No nVIDIA GPU Driver found.");
  }
  vector<string> res;
  pond::System("nvidia-smi -L",res) ;
  Argv.SetList();
  for ( auto li: res){
    Argv.PushBackString( li);
  }
  ReturnNormal;
}

int SystemModule::PD_SetCudaThreadsNumberPerBlock(Object&Argv)
{
  CheckShouldEqual(1);
  CheckShouldBeNumber(1);
  int num = (int)Argv[1];
  if ( num <= 0 and num%32 != 0 and num > 1024 )
    ThrowError("SetCudaThreadsNumberPerBlock","Thread number per block is suggested to be multiple of 32 and a number with 0 and 1024.");
  pond::SetThreadNumberPerBlock( num );
  ReturnNull;
}

int SystemModule::PD_SetCpuThreadsNumber(Object&Argv)
{
  CheckShouldEqual(1);
  CheckShouldBeNumber(1);
  int num = (int)Argv[1];
  if ( num <= 0 )
    ThrowError("SetEvawizCpuKernelThreadsNumber","Thread number should be a positive number.");
  pond::SetThreadNumberPerKernel( num );
  ReturnNull;
}

int SystemModule::PD_GetCudaThreadsNumberPerBlock(Object&Argv)
{
  CheckShouldEqual(0);
  ReturnNumber( EvaSettings.threadNumberPerBlock );
}

int SystemModule::PD_GetCpuThreadsNumber(Object&Argv)
{
  CheckShouldEqual(0);
  ReturnNumber( EvaSettings.threadNumberPerKernel );
}
