/****************************/

#include"pd_headers.h"
#include<QtGui/QtGui>


#include"GuisModule.h" 

using namespace std;
using namespace pond;
/****************************/
__DEBUG_MAIN__("input.ev");

GuisModule::GuisModule():Module(MODULE_NAME)
{

}

GuisModule::~GuisModule()
{

}

#include"notepad.h"
int GuisModule::PD_Notepad(Object&argv){
  // Launch a Notepad Qt application.
  QApplication app( EvaKernel->argc, EvaKernel->argv);
  Notepad notepad;
  notepad.show();
  app.exec();
  ReturnNull;
}

#include"runmod.h"
int GuisModule::PD_Runmod(Object&argv){
  // Launch a Runmod Qt application
  QApplication app( EvaKernel->argc, EvaKernel->argv);
  Runmod runmod;
  runmod.show();
  app.exec();
  ReturnNull;
}




