#include<pond.h>
#include<iostream>
using namespace std;
using namespace pond;
#define x(name) ({#name})
int main(){
  GlobalPool.Eva = NULL;
  cout<< GlobalPool.Eva <<endl;

  cout<<"Hello POND."<<endl;
  pond::SetParallelMode(pond::ParallelModeCpu);
  cout<< "DataPosition:" << pond::GetDataPosition <<endl;
  cout<< "threads number:" << EvaSettings.threadNumberPerKernel <<endl;
  cout<<pond::GPUDeviceQ()<<endl;
  Matrix arr;
  int Ni=3,Nj=2,Nk=1,Nl=2;
  arr.Init(4,Ni,Nj,Nk,Nl,MatrixHostDevice);
  arr = arr.Size();
  cout <<"original arr = "<<arr<<endl;

  arr.HostToDevice();
// #pragma launch_kernel<<<i:Ni,j:Nj,k:Nk,l:Nl>>>(Matrix arr:arr,int a:6)
//   {
//     printf("%d %d %d %d\n",i,j,k,l);
//     arr(i,j,k,l)=(i+j+k+l)*a;
//   }
  arr.DeviceToHost();

  cout<<"arr altered = "<<arr<<endl;
//   cout<<"test for omp things"<<endl;
#pragma launch_kernel<<<i: 1333>>>(int a:344)
  {
    a = a*3;
  }
  arr.DumpFile("arr.dat");

  return 0; 
  
}
