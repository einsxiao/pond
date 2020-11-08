/**********************************
Author          : AUTHOR
Version         : test.VERSION  
R-Copyleft(r-ɔ) : DATE
**********************************/
#include <pond>
#include "testModule.h" 
/****************************/
using namespace std;
using namespace pond;

/****************************/
__DEBUG_MAIN__("input.pd");
/****************************/
testModule::testModule():Module(MODULE_NAME){
  /*
    Module string of explanation
  */

}

testModule :: ~testModule(){

}

/****************************/
int testModule::PD_hello(Object&argv)
{
  dout << "Welcome to the land of creativity!!!"  << endl;
  // ReturnString("Greate!!!!");
  ReturnNull;
}

int testModule :: PD_hello2(Object&Argv)
{
  dout << "Welcome to the land of creativity!!!"  << endl;
  // ReturnString("Greate!!!!");
  ReturnNull;
}




