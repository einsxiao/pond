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

}

TemplateModule :: ~TemplateModule(){

}

/****************************/
int TemplateModule::PD_hello(Object&Argv){
  // this is line comment explanation for hello
  dout << "Welcome to the land of creativity!!!"  << endl;
  // ReturnString("Greate!!!!");
  ReturnNull;
}

/****************************/
int TemplateModule :: PD_hello2(Object&Argv)
{
  /*
    this comment content will be doc string for hello2
    and it can be multilines I'm "good"
    "bad"
   */
  dout << "Welcome to the land of creativity!!!"  << endl;
  // ReturnString("Greate!!!!");
  ReturnNull;
}




