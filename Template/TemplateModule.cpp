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
  RegisterFunction("打招呼", MemberFunction(&TemplateModule::PD_hello), this);
}

TemplateModule :: ~TemplateModule(){

}

/****************************/
int TemplateModule::PD_hello(Object&Argv){
  /* comment lines following will be doc for this function */
  cout << "Welcome to DocRun!!!"  << endl;
  ReturnNull;
}

