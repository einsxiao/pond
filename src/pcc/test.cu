#include<pond.h>
#include<iostream>
using namespace std;
using namespace pond;
int main(){

#pragma launch_kernel<<<i: 1333>>>(int a:344,   \
                                   int c: 4,    \
                                   int d: 4,    \
                                   int b: 444,)
  {
    a = a*3;
  }

  return 0; 
  
}
