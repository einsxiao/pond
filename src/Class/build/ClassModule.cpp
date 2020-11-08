#define ModuleName Class
#include<pond.h>
#include"ClassModule.h"
using namespace std;
using namespace pond;


static INIT_SYMID_OF( FUNCTION$DEFINED$ );

static INIT_SYMID_OF( CLASS$DEFINED$ );
static INIT_SYMID_OF( CLASS$DEFINED$CONTENT$ );

static INIT_SYMID_OF( CLASS$INSTANCE$ );

static INIT_OPER_SYMID_OF(class,                  0,220,   1,2);
static INIT_OPER_SYMID_OF(new,                    0,160,   1,1);
static INIT_OPER_SYMID_OF(del,                    0,160,   1,1);

class __EvaClassTable:public __EvaTable< pair<ValueTable,int> >{
public:
  __EvaClassTable():__EvaTable(1024){
    tableName = "ClassTable";
  };
};

static __EvaClassTable ClassPool;

ModuleConstruction{
  AddAttribute("class",AttributeType::HoldAll); // the function to define class tempalte

  AddAttribute("CLASS$DEFINED$",AttributeType::HoldAll);  // For Member ref or call Member function
  AddAttribute("CLASS$DEFINED$",AttributeType::Conjunctable); 
  AddAttribute("CLASS$DEFINED$",AttributeType::DelayFunction); 
  AddAttribute("CLASS$DEFINED$",AttributeType::Setable); 
  AddAttribute("CLASS$DEFINED$CONTENT$",AttributeType::HoldAll);
  //For Member ref or call Member function
  AddAttribute("CLASS$INSTANCE$",AttributeType::HoldAll);  // For Member ref or call Member function
  AddAttribute("CLASS$INSTANCE$",AttributeType::Conjunctable); 
  AddAttribute("CLASS$INSTANCE$",AttributeType::DelayFunction); 
  AddAttribute("CLASS$INSTANCE$",AttributeType::Setable); 

  AddAttribute("test",AttributeType::HoldAll);
}

ModuleDestruction{
};

DefinePDFunction(class,argv){Object&Argv=argv;
  CheckShouldEqual( 2 );
  CheckShouldBeSymbol( 1 );
  CheckShouldBeListWithHead(2, SYMID_OF_ExpressionList );
  Object &name = argv[1];
  Object &body = argv[2];
  //dout<<"class def with "<<argv<<endl;
  Object pairTable(__List__, SYMID_OF_Dict );
  Object patternTable(__List__,SYMID_OF_List );
  Object varList(__List__,SYMID_OF_List );
  //dout<<"create new context"<<endl;
  EvaKernel->newContext( pairTable, patternTable);
  //dout<<"try analyze defination body"<<endl;
  for (int i=1; i <= body.Size(); i++ ){
    //dout<<"deal arg "<<i<<endl;
    while ( body[i].ListQ( SYMID_OF_CompoundExpression ) ){
      body.InsertRef( body.Begin()+i, body[i].Begin(), body[i].End() );
      body.Delete( i );
    }
    Object pair; 
    if ( body[i].SymbolQ() ){
      pair.SetList(SYMID_OF_KeyValuePair )
        .PushBackRef( body[i] )
        .PushBackNull();
    }else if ( body[i].PairQ(SYMID_OF_Set) ){
      pair = body[i];
    }else if ( body[i].PairQ(SYMID_OF_SetDelayed ) ){
      if ( Pattern::PatternListQ( body[i][1] ) ){
        EvaKernel->currentValueTable->UpdateOrInsert(body[i][1], body[i][2], false, true );
        continue;
      }
      pair = body[i];
    }else {
      EvaKernel->Evaluate( body[i] );
      continue;
    }
    //   else{//unexpected expression
    //   _Erroring( zh,"类定义","不合法的类定义表达式 "+body[i].ToString() ) ||
    //     _Erroring(_,"class","unexpected expression in class defination "+body[i].ToString() );
    //   EvaKernel->deleteContext();
    //   ReturnError;
    // }
    if ( not pair.NullQ() ){
      int res = pairTable.DictInsertPairRef( pair );
      if ( not res ){
        _Erroring( zh,"类定义","重复定义类变量 "+body[i].ToString() ) ||
          _Erroring(_,"class","Redefination of class variable "+body[i].ToString() );
        EvaKernel->deleteContext();
        ReturnError;
      }
    }
    // check if pair is a variable, if it is add it to variable array
    if ( not pair[2].ListQ( SYMID_OF_FUNCTION$DEFINED$ ) ){ // pure symbol, 
      varList.PushBackRef( pair ); 
    }
  }
  EvaKernel->deleteContext();
  //dout<<"class table = "<< pairTable<<" \npattern table = "<< patternTable <<endl;
  Argv[2] = pairTable;
  Argv.PushBackRef( patternTable );
  Argv.PushBackRef( varList );
  Argv[0].SetSymbol( SYMID_OF_CLASS$DEFINED$CONTENT$ );
  // define content pair
  Object defname = Object(__Symbol__, string(name)+"$DEFINATION$");
  Object defpair = EvaKernel->RefInstantInsertOrUpdatePairValue(defname, Argv);
  //dout<< " pair stored in table is "<< defpair<<endl;

  Object cladef(__List__,SYMID_OF_CLASS$DEFINED$ );
  cladef.PushBackSymbol( SYMID_OF_SerialCode );
  cladef.PushBackRef( name );
  cladef[1].set_idx( defpair.objid );
  //dout<<defpair<<" objid ="<< defpair.objid<<endl;
  // define for defination
  Object pair = EvaKernel->RefInstantInsertOrUpdatePairValue( name, cladef );
  Module::AddAttribute( name.Key(), AttributeType::Protected );
  Argv.SetVoid();
  // after well formated defination:
  // A$DEFINATION$  ->
  //        CLASS$DEFINATION$( name,    pairTable,      patternTable )
  // A ->
  //        CLASS$DEFINED$($_x_y, A)
  return 1;
}

DefinePDFunction(CLASS$DEFINED$,Argv){ // A($$_i_j,A)
 
  Set_Context( Conjunct ){ // A.a = 3, where A is 
    //dout<<"into Conjunct Set Context with Argv = "<<Argv<<endl;
    Object&cla  =  Argv[1][1];
    Object clapair( cla[1].idx() );
    Object &def = clapair[2];

    Object&var  =  Argv[1][2];
    if ( not var.SymbolQ() ){
      _Erroring( zh,"类","只有类成员才能被赋值 "+var.ToString()+" 不是一个符号")||
        _Erroring(_,"class", "Only class member can be assigned a value" +var.ToString()+" is not a Symbol.");
    }
    Object &pairTable = def[2];
    //dout<<"pairTable = "<<pairTable<<endl;
    Object pair = pairTable.DictGetOrNewPair( var );
    //dout<< " get pair "<< pair <<endl;
    pair[2] = Argv[2];
    ReturnNull;
  }
  // since is registered as Oper symbol, it is protected, normal set will not run to here
  // Set_Context( CLASS$DEFINED$ ){ 
  //   //dout<<"into normal Set Context with Argv = "<<Argv<<endl;
  //   ReturnNormal;
  // }
  // use as direct A.a
  Conjunct_Context( CLASS$DEFINED$ ){ // A.a  or A.f()
    Object &cla= Argv[1];
    Object clapair( cla[1].idx() );
    Object &def = clapair[2];
    Object &name = def[1];
    Object &pairTable = def[2];
    Object &patternTable = def[3];
    Object &oper = Argv[2];
    //dout<<"conjunct context def"<<def<<endl;
    //dout<<" get into defined context, access static members "<<endl;

    if ( oper.SymbolQ() ){
      Object pair = pairTable.DictGetPair( oper );
      if ( pair.NullQ() ){
        _Erroring( zh,"类","类 "+name.ToString()+" 没有名为"+oper.ToString()+" 的变量或者方法")||
          _Erroring(_,"class", "class "+name.ToString()+" does not have member named "+oper.ToString() );
        ReturnError;
      }
      //dout<< "as a normal symbol member"<<endl;
      Argv.CopyObject( pair[2] );
      ReturnNormal;
    }else if ( oper.SimpleListQ() ){ // form like    a.init(...)
      Object pair = pairTable.DictGetPair( oper[0] );
      if ( pair[2].ListQ( SYMID_OF_FUNCTION$DEFINED$ ) ){
        //dout<< "is a defined function, try init context and evaluate"<<endl;
        oper =  Argv[2];
        oper[0].CopyObject( pair[2] );
        //dout<<" try eval "<< oper <<" under ctx"<<endl;
        EvaKernel->newContext( pairTable, patternTable );
        EvaKernel->Evaluate( oper );
        EvaKernel->deleteContext();
        Argv = oper;
        ReturnNormal;
      }
      // try test pattern table
      _Erroring( zh,"类","类 "+name.ToString()+" 名为"+oper.ToString()+" 成员不是函数")||
        _Erroring(_,"class", "class "+name.ToString()+" member named "+oper.ToString()+" is not a function." );
      ReturnError;
    }
    ReturnNormal;
  }
  DelayFunction_Context( CLASS$DEFINED$ ){ // form for  A()
    //dout<<"get into delay function of defined class to do instance production"<<endl;
    //dout<<"current argv = "<<Argv<<endl;
    Object &cla = Argv[0];
    //dout<<" cla = "<<cla<<endl;
    Object clapair( cla[1].idx() );
    Object &def = clapair[2];
    Object &pairTable = def[2];
    Object &patternTable = def[3];
    EvaKernel->newContext( pairTable, patternTable );
    Object ins(__List__,     SYMID_OF_CLASS$INSTANCE$ );
    ins.PushBackSymbol( SYMID_OF_SerialCode );
    ins[1].set_idx( def.objid );
    ins.PushBackCopy( def[4] );
    // Argv = ins;
    static INIT_SYMID_OF( constructor );
    static Object constructor(__Symbol__, SYMID_OF_constructor );
    Object funcpair = pairTable.DictGetPair( constructor );
    if ( funcpair[2].ListQ( SYMID_OF_FUNCTION$DEFINED$ ) ){
      //dout<<"before constructor vt = "<<ins[2]<<endl;
      EvaKernel->newContext( ins[2] );
      Argv[0].SetVoid();
      Argv[0].CopyObject( funcpair[2] );
      EvaKernel->Evaluate( Argv );
      // class_function( Argv );
      //dout<<"after constuctor called vt = "<< ins[2]<<endl;
      EvaKernel->deleteContext();
    }
    EvaKernel->deleteContext();
    Argv = ins;
    ReturnNormal;
  }
  //dout<<"normal one"<<endl;

  ReturnNormal;
}

DefinePDFunction(CLASS$INSTANCE$,Argv){

  // Conjunct always pass reference
  Conjunct_Context( CLASS$INSTANCE$ ){ // CLASS$INSTANCE$($$_i_j,{vartable}).x
    // a.x  or a.f()
    Object&insobj = Argv[1];
    CheckArgsShouldNoLessThan(insobj,1);
    if ( not insobj[1].SymbolQ(SYMID_OF_SerialCode ) ){
      Erroring("Class","Only an installation of Class can access their members.");
      return -1;
    }
    Object vartable = insobj[2];
    Object def( insobj[1].idx() );
    Object &name = def[1];
    Object &pairTable = def[2];
    Object &patternTable = def[3];
    Object &oper= Argv[2];

    if ( oper.SymbolQ() ){  // a = A();     a.a
      Object pair = vartable.DictGetPair( oper );
      if ( pair.NullQ() ){
        _Erroring( zh,"类::点运算",oper.ToString()+" 不是类实例成员")||
          _Erroring(_,"Class::Conjunct",oper.ToString()+" is not a member of class instance.");
        ReturnError;
      }
      Argv.CopyObject( pair[2] );
      ReturnNormal;
    }else if ( oper.SimpleListQ() ){ // a.f()
      Object pair           = pairTable.DictGetPair( oper[0] );
      if ( pair[2].ListQ( SYMID_OF_FUNCTION$DEFINED$ ) ){
        oper[0].CopyObject( pair[2] );
        EvaKernel->newContext( vartable, patternTable );
        EvaKernel->Evaluate( oper );
        EvaKernel->deleteContext();
        Argv = oper;
        ReturnNormal;
      }
      // try test pattern table
      _Erroring( zh,"类","类 "+name.ToString()+" 名为"+oper.ToString()+" 成员不是函数")||
        _Erroring(_,"class", "class "+name.ToString()+" member named "+oper.ToString()+" is not a function." );
      ReturnError;
    }
    _Erroring( zh,"类::点运算","数值或者字符串不能作为类成员.")||
      _Erroring(_,"Class::Conjunct","String or Number cannot be member of class.");
    ReturnError;
  }
  Set_Context( Conjunct ){ // a.x = 3
    //dout<<"into normal Conjunct Set Context with Argv = "<<Argv<<endl;
    Object&insobj         = Argv[1][1];
    Object def              ( insobj[1].idx() );
    Object vartable       = insobj[2];
    Object &name          = def[1];
    Object &pairTable     = def[2];
    Object &patternTable  = def[3];
    Object &oper          = Argv[1][2];
    Object &rightValue    = Argv[2];
    if ( oper.SymbolQ() ){
      //Object pair = vartable.DictGetPair( oper );
      Object pair = vartable.DictGetOrNewPair( oper );
      pair[2] = rightValue;
      ReturnNull;
    }
    _Erroring( zh,"类","类 "+name.ToString()+" 成员指定 "+oper.ToString()+" 不合法")||
      _Erroring(_,"class", "class "+name.ToString()+" member specification "+oper.ToString()+" is not valid." );
    ReturnNormal;
  }

  Set_Context( CLASS$INSTANCE$ ){ // a = 3 
    //dout<<"into normal Set Context with Argv = "<<Argv<<endl;
    ReturnNormal;
  }

  ReturnHold;
}


// DefinePDFunction(test,Argv){ //, "通用函数定义"
//   // used in form:   function f(a,b,x,y=1,z=3){...}
//   // return or trandi way to return value
//   // *args, **kwargs
//   //dout<<"into test of class"<<endl;
//   CheckShouldEqual(2);
//   if ( not Argv[1].ListQ() or not Argv[1][0].SymbolQ() ){
//     _Erroring(zh,Argv[0].ToString(),"函数声明格式错误: "+Argv[1].ToString() )||
//       _Erroring(_,Argv[0].ToString(),"function declaration is in the wrong form: "+Argv[1].ToString() );
//     ReturnError;
//   }
//   CheckShouldBeListWithHead(2, SYMID_OF_ExpressionList );
//   //dout<<"into def with Argv ="<<Argv<<endl;
//   // deal those with default values
//   Object & args = Argv[1];
//   Object dict(__List__,SYMID_OF_Dict);
//   Object packVar;
//   Object dictPackVar;
//   int i = 1;
//   while ( i<= args.Size() ){
//     if ( args[i].SymbolQ() ){
//       if ( not packVar.NullQ() ){
//         _Erroring(zh,Argv[0].ToString(),"位置参数出现在参数包后") ||
//           _Erroring(_,Argv[0].ToString(),"Positional argument follows arguments pack");
//         ReturnError;
//       }
//       if ( not dictPackVar.NullQ() ){
//         _Erroring(zh,Argv[0].ToString(),"位置参数出现在字典参数包后") ||
//           _Erroring(_,Argv[0].ToString(),"Positional argument follows keyword arguments pack");
//         ReturnError;
//       }
//     }else if ( args[i].PairQ( SYMID_OF_Set ) ){
//       EvaKernel->Evaluate( args[i][2] );
//       // dict.DictInsertPairRef( args[i] );
//       Object::iterator iter;
//       bool res = dict.DictGetPosition( args[i][1], iter ); 
//       if ( res == 0 ){
//         _Erroring(zh,Argv[0].ToString(),"字典参数 "+args[i][1].ToString()+" 被重复指定") ||
//           _Erroring(_,Argv[0].ToString(),"Multiple Keyword argument "+args[i][1].ToString()+" specified.");
//         ReturnError;
//       }
//       dict.InsertRef(iter, args[i] );
//       i++; //args.Delete(i);
//       continue;
//     }else if ( args[i].PairQ( SYMID_OF_SetDelayed ) ){
//       _Erroring(zh,Argv[0].ToString(),"延迟赋值不能用来给参数提供默认值") ||
//         _Erroring(_,Argv[0].ToString(),"SetDelayed can not be used to provide default value");
//       ReturnError;
//     }else if ( args[i].ListQ( SYMID_OF_Unpack )  ){
//       if ( not packVar.NullQ() ){
//         _Erroring(zh,Argv[0].ToString(),"参数包只能又一个") ||
//           _Erroring(_,Argv[0].ToString(),"Multi argument pack");
//         ReturnError;
//       }
//       if ( args[i].Size() != 1 and not args[i][1].SymbolQ() ){
//         _Erroring(zh,Argv[0].ToString(),"参数包应该具有符号名字") ||
//           _Erroring(_,Argv[0].ToString(),"Arguments pack should has a symbol name");
//         ReturnError;
//       }
//       packVar = args[i][1];
//       //dout<<"try delete pack var at "<<i <<" args ="<<args<<endl;
//       args.Delete(i);
//       continue;
//     }else if ( args[i].ListQ( SYMID_OF_UnpackDict )  ){
//       if ( not dictPackVar.NullQ() ){
//         _Erroring(zh,Argv[0].ToString(),"字典参数包只能有一个") ||
//           _Erroring(_,Argv[0].ToString(),"Multi keyword argument pack");
//         ReturnError;
//       }
//       if ( args[i].Size() != 1 and not args[i][1].SymbolQ() ){
//         _Erroring(zh,Argv[0].ToString(),"字典参数包应该具有符号名字") ||
//           _Erroring(_,Argv[0].ToString(),"Dict arguments pack should has a symbol name");
//         ReturnError;
//       }
//       dictPackVar = args[i][1];
//       //dout<<"try delete dict pack var at "<<i <<" args ="<<args<<endl;
//       args.Delete(i);
//       continue;
//     }else{
//       _Erroring(zh,Argv[0].ToString(),args[i].ToString() + "不能用来作为参数") ||
//         _Erroring(_,Argv[0].ToString(),args[i].ToString() + " can not be used to as argument");
//       ReturnError;
//     }
//     i++;
//   }
//   Argv.PushBackRef(dict);
//   Argv.PushBackRef(packVar);
//   Argv.PushBackRef(dictPackVar);
//   // save processed value to valueTable
//   nbout<<Argv[1]<<" defined"<<endl;
//   Argv[0].SetSymbol( SYMID_OF_FUNCTION$DEFINED$ );
//   EvaKernel->InstantInsertOrUpdatePairValue(Argv[1][0], Argv );
//   ReturnNull;
// }

// DefinePDFunction(test2,arg){Object&Argv=arg;
//   Object l1(__Symbol__,"test");
//   Object l2(__Symbol__,"testcontent");
//   Object l3(__List__,SYMID_OF_List);
//   Object l4(__List__,SYMID_OF_List);
//   l4.PushBackSymbol("abcde");
//   // l2.CopyObject( l1 );
//   // l3.CopyObject( l1 );
//   // l4.CopyObject( l1 );
//   l1.ToList();
//   l1.PushBackCopy( l4 );
//   l1.CopyObject( l4 );
//   // EvaKernel->PD_GetMemInfo(l3);
//   // l1.CopyObject( l4 );
//   // l1.DeleteElements();
//   // EvaKernel->PD_GetMemInfo(l3);
//   // l1.PushBackCopy( l4 );
//   // l1.DeleteElements();
//   // EvaKernel->PD_GetMemInfo(l3);
//   EvaKernel->InstantInsertOrUpdatePairValue( l1,l4);
//   ReturnNull;
// }
