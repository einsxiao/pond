/**********************************
Author          : MOD_AUTHOR
Version         : Test.MOD_VERSION  
R-Copyleft(r-ɔ) : MOD_DATE
**********************************/
#include "pond.h"
#include "MatrixModule.h"
#include "TestModule.h" 
/****************************/
using namespace std;
using namespace pond;

/****************************/
__DEBUG_MAIN__("input.pd");
/****************************/

TestModule::TestModule():Module(MODULE_NAME){}
TestModule :: ~TestModule(){ }

int TestModule::PD_vector_times(Object&argv){
  /* a vector times a number */
  Matrix mat; MatrixModule::Object2Matrix(argv[1], mat);
#pragma launch_kernel<<<i:mat.Size()>>>(Matrix mat:mat,double x:double(argv[2]))
  {
    mat[i]*=x;
  }
  MatrixModule::Matrix2Object(mat, argv);
  return 0;
}

