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
  //RegisterFunction("测试", MemberFunction(&TemplateModule::PD_test), this);
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
    a vector times a number
  */
  Matrix mat; MatrixModule::Object2Matrix(argv[1], mat);

#pragma launch_kernel<<<i: mat.Size() >>>(Matrix mat : mat,           \
                                          double x : double(argv[2]))
  {
    mat[i]*=x;
  }
  MatrixModule::Matrix2Object(mat, argv);
  return 0;
}


int TemplateModule::PD_mat_oper_test(Object&argv){
  /*
    test matrix operations
  */
  cout<<"################\n###test matrix operation\n##############"<<endl;
  pond::SetDataPosition(MatrixDevice);
  Matrix mat; 
  mat.Init(2,3,3,MatrixHostDevice);
  for(int i=0; i<3; i++)
    for (int j=0; j<3; j++)
      mat(i,j) = i+j*3;
  cout<<"mat = "<<mat<<endl;
  mat.HostToDevice();
#pragma launch_kernel<<<i:3, j:3>>>(Matrix mat : mat)
  {
    mat(i,j) *= -3;
  }
  mat.DeviceToHost();
  cout<<"mat = "<<mat<<endl;
  mat *= -3;
  mat.DeviceToHost();
  cout<<"mat = "<<mat<<endl;
  ReturnNull;
}
