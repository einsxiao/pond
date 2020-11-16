/**********************************
Author          : AUTHOR
Version         : Template.VERSION  
R-Copyleft(r-ɔ) : DATE
**********************************/
#include <pond>
#include "TemplateModule.h" 
/****************************/
using namespace std;
using namespace pond;

/****************************/
__DEBUG_MAIN__("input.pd");
/****************************/

TemplateModule::TemplateModule():Module(MODULE_NAME){
  RegisterFunction("加一", MemberFunction(&TemplateModule::PD_incr), this);

}

TemplateModule :: ~TemplateModule(){

}

/****************************/
int TemplateModule::PD_hello(Object&Argv){
  /* comment lines following will be doc for this function */
  cout << "Welcome to the land of creativity!!!"  << endl;
  ReturnNull;
}

int TemplateModule::PD_incr(Object&argv){
  /* add 1 to the input number */
  CheckShouldEqual(1);    // the argument number should be one
  CheckShouldBeNumber(1); // the first argument should be number
  double num = (double)argv[1];
  ReturnNumber( num+1 );
}

int TemplateModule::PD_j$j$(Object&argv){
  ReturnString("j$j$");
}


