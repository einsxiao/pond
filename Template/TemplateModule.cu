/**********************************
Author          : MOD_AUTHOR
Version         : Template.MOD_VERSION  
R-Copyleft(r-ɔ) : MOD_DATE
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
}

TemplateModule :: ~TemplateModule(){
}

int TemplateModule::PD_welcome(Object&argv){
  /*
    comment lines following will be doc for this function
  */
  cout << "Welcome to PHPP!!!"  << endl;
  argv.SetNull();
  return 0;
}

int TemplateModule::PD_vector_times(Object&argv){
  /*
    example function which calculate vector times number parallel on cpu or gpu.
  */
  Matrix mat; MatrixModule::Object2Matrix(argv[1], mat);
  double factor = double(argv[2]);
  cout<<"initial mat = "<< mat <<endl;
  mat.HostToDevice();
#pragma launch_kernel<<<i: mat.Size()>>>(Matrix mat : mat, double factor : factor)
  {
    mat[i] *= factor;
  }
  MatrixModule::Matrix2Object(mat, argv);
  return 0;
}


