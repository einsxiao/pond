/**********************************
Author          : AUTHOR
Version         : Template.VERSION  
R-Copyleft(r-ɔ) : DATE
**********************************/
#include "pond.h"
#include "MatrixModule.h"
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
int TemplateModule::PD_hello(Object&argv){
  /* comment lines following will be doc for this function */
  cout << "Welcome to PHPP!!!"  << endl;
  complex a(3,4);
  cout << 2*a << endl;
  argv.SetNull();
  return 0;
}

int TemplateModule::PD_matX2(Object&argv){
  Matrix mat;
  MatrixModule::Object2Matrix(argv[1],mat);
  mat.HostToDevice();
#pragma launch_kernel<<<i:mat.Size()>>>(Matrix mat: mat)
  {
    mat(i) *= 2;
  }
  mat.DeviceToHost();
  MatrixModule::Matrix2Object( mat, argv );
  return 0;
}

