#include"pond.h"
#include"StringModule.h"
// #include<emscripten.h>
using namespace std;
using namespace pond;

__DEBUG_MAIN__("input.pd");

StringModule::StringModule():Module(MODULE_NAME){

}

StringModule::~StringModule(){

}

int StringModule::PD_StringSplit(Object&Argv){
  /* Split string into pieces by splitor specified. */
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

int StringModule::PD_RandomString(Object&Argv)
{
  /*
    Return a random string of given length.
  */
  CheckShouldEqual(1);
  CheckShouldBeNumber(1);
  int len = Argv[1].Number();
  ReturnString( pond::RandomString(len) );
}

int StringModule::PD_StringJoin(Object &Argv)
{
  /*
    Join Strings or String of Expressions into one Single String.
  */
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


// int StringModule::PD_RunJS(Object & argv){
//   CheckShouldBe(1);
//   CheckShouldBeString(1);
//   string script(argv[1].Key() );
//   //cout<<"script is "<< script <<endl;
//   emscripten_run_script(script.c_str() );
//   Return(Null);
// }

// int StringModule::PD_RunJSInt(Object & argv){
//   CheckShouldBe(1);
//   CheckShouldBeString(1);
//   string script(argv[1].Key() );
//   //cout<<"script is "<< script <<endl;
//   int res = emscripten_run_script_int(script.c_str() );
//   ReturnNumber(res);
// }

// int StringModule::PD_RunJSString(Object & argv){
//   CheckShouldBe(1);
//   CheckShouldBeString(1);
//   string script(argv[1].Key() );
//   //cout<<"script is "<< script <<endl;
//   string res = emscripten_run_script_string(script.c_str() );
//   ReturnString(res);
// }
