#define ModuleName String
#include"pond.h"
#include"StringModule.h"
#include<emscripten.h>
using namespace std;
using namespace pond;

ModuleConstruction{

}

ModuleDestruction{

}

DefineFunction(StringSplit,"Split string into pieces by splitor specified."){
  CheckShouldBeWithin(1,2);
  CheckShouldBeString(1);
  string str = Argv[1].Key();
  string split;
  if (Argv.Size()>1){
    CheckShouldBeString(2);
    split = Argv[2].Key();
  }else
    split = " ";
  vector<string>result;
  pond::StringSplit(str,result,split);
  Argv.SetList();
  for (u_int i = 0; i< result.size(); i++)
    Argv.PushBackString( result[i].c_str() );
  return 0;
}

DefineFunction(RandomString,"Return a random string of given length.")
{
  CheckShouldEqual(1);
  CheckShouldBeNumber(1);
  int len = Argv[1].Number();
  ReturnString( pond::RandomString(len) );
}

DefineFunction(StringJoin,"Join Strings or String of Expressions into one Single String.")
{
  CheckShouldNoLessThan(1);
  string content;
  for (size_t i=1; i<= Argv.Size(); i++ ){
    if ( Argv[i].StringQ() )
      content += Argv[i].Key();
    else
      content+=Argv[i].ToString();
  }
  Argv.SetString(content);
  ReturnNormal;
}


DefinePDFunction(RunJS,argv){Object&Argv=argv;
  CheckShouldBe(1);
  CheckShouldBeString(1);
  string script(argv[1].Key() );
  //cout<<"script is "<< script <<endl;
  emscripten_run_script(script.c_str() );
  Return(Null);
}

DefinePDFunction(RunJSInt,argv){Object&Argv=argv;
  CheckShouldBe(1);
  CheckShouldBeString(1);
  string script(argv[1].Key() );
  //cout<<"script is "<< script <<endl;
  int res = emscripten_run_script_int(script.c_str() );
  ReturnNumber(res);
}

DefinePDFunction(RunJSString,argv){Object&Argv=argv;
  CheckShouldBe(1);
  CheckShouldBeString(1);
  string script(argv[1].Key() );
  //cout<<"script is "<< script <<endl;
  string res = emscripten_run_script_string(script.c_str() );
  ReturnString(res);
}
