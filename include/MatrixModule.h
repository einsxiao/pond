#ifndef SuanZi_MatrixModule_H
#define SuanZi_MatrixModule_H

namespace pond{
  class MatrixModule:public Module{
  public:
    MatrixModule(); ~MatrixModule();
    ///////////////////////////////
    const u_int Len_Of_New_Matrix_Name = 8;
    ///////////////////////////////
    Matrix          *GetMatrix(std::string matrixName);
    ComplexMatrix   *GetComplexMatrix(std::string matrixName);
    Matrix          *GetOrNewMatrix(std::string matrixName);
    ComplexMatrix   *GetOrNewComplexMatrix(std::string matrixName);
    int              RemoveMatrix(std::string matrixName);
    int              RemoveComplexMatrix(std::string matrixName);
    ///////////////////////////////
    Matrix&operator()(std::string id);
    Matrix&operator[](std::string id);
    ///////////////////////////////
    static int Array2List(Object&Argv,int*arr,int*dim);
    static int Array2List(Object&Argv,long long*arr,int*dim);
    static int Array2List(Object&Argv,float*arr,int*dim);
    static int Array2List(Object&Argv,double*arr,int*dim);
    static int Array2List(Object&Argv,complex*arr,int*dim);
    static int MatrixGet(Matrix&,Object&);
    static int MatrixGet(ComplexMatrix&,Object&);
    int MatrixGet(std::string,Object&); //get matrix from table
    static int List2Array(Object&Argv,double*arr);
    static int List2Array(Object&Argv,complex*arr);
    static int MatrixSet(Matrix&,Object&);
    static int MatrixSet(ComplexMatrix&,Object&);
    static int MatrixSet(FloatMatrix&,Object&);
    static int MatrixSet(FloatComplexMatrix&,Object&);
    static int MatrixSet(IntMatrix&,Object&);
    int MatrixSet(std::string,Object&); //store Matrix to table
    ///////////////////////////////
    int PD_Matrix(Object&Argv);
    ///////////////////////////////
    static bool MatrixQ(Object&obj,Object&dim);
    static bool MatrixQ(Object&obj);
    bool MatrixExist(std::string name);
    int PD_MatrixExist(Object&);
    ///////////////////////////////
    int MatrixPosition(Object&);
    int MatrixSetPosition(Object&);
    ///////////////////////////////
  };
  
  inline int operator>>(Object Argv,Matrix &matrix)
  {
    return MatrixModule::MatrixSet(matrix,Argv);
  }

  inline int operator>>(Matrix &matrix,Object Argv )
  {
    return MatrixModule::MatrixGet(matrix,Argv);
  }

  
#if defined(__CUDACC__)
#include<curand.h>
#endif
  template<class type>
  class RawRandomMatrix: public Matrix_T<type>{
  public:
    double               Mean = 0.0,Variance = 1.0;
    int                  Seed = 0;
#if defined(__CUDACC__)
    curandGenerator_t    Gen_dev;
#endif

    RawRandomMatrix()
    {
      Mean=0.0f;
      Variance=1.0f;
      Seed=0;
#if defined(__CUDACC__)
      Gen_dev  = NULL;
#endif
    };

    RawRandomMatrix(int seed)
    {
      Mean=0.0f;
      Variance=1.0f;
      Seed=seed;
      srand(seed);
#if defined(__CUDACC__)
      Gen_dev  = NULL;
#endif
    };

    ~RawRandomMatrix()
    {
#if defined(__CUDACC__)
      if (Gen_dev) curandDestroyGenerator(Gen_dev);
#endif
    };

    int InitRandomMatrix(int *dimArr,double mean=0.0,double variance=1.0 ,int seed=0.0)
    {
      srand(seed);
#if defined(__CUDACC__)
      curandCreateGenerator(&Gen_dev,CURAND_RNG_PSEUDO_DEFAULT);
      curandSetPseudoRandomGeneratorSeed(Gen_dev,seed);
#endif
      Matrix_T<type>::Init(dimArr,MatrixHostDevice);
      Mean=mean;
      Variance=variance;
      if ( seed != 0 )
        Seed=seed;
      return 0;
    };
    
    int InitRandomMatrix(int n,...)
    {
      va_list args;
      va_start(args,n);
      int *arr	=	new int [ n+ 2 ];
      arr[0]=n;
      for (int i=1; i<=n; i++)
        arr[i]	=	va_arg(args,int);
      va_end(args);
      InitRandomMatrix(arr,Mean,Variance,Seed);
      delete[]arr;
      return 0;
    }; 

    int SetParas(double mean=0.0,double variance =1.0,int seed = 0.0)
    {
      Mean=mean;
      Variance=variance;
      Seed=seed;
      srand(Seed);
#if defined(__CUDACC__)
      curandSetPseudoRandomGeneratorSeed(Gen_dev,Seed);
#endif
      return 0;
    };

    Matrix_T<type>     &NewNormalDevice()
    {
#if defined(__CUDACC__)
      if ( EvaSettings::RunningMode() == RunningModeGpu ){
        if ( typeid(type) == typeid(double) ){
          curandGenerateNormalDouble(Gen_dev,
                                     (double*)Matrix_T<type>::DataDevice,
                                     Matrix_T<type>::Size(),
                                     Mean,Variance
                                     );
        }else if( typeid(type) == typeid(float) ){
          curandGenerateNormal(Gen_dev,
                               (float*)Matrix_T<type>::DataDevice,
                               Matrix_T<type>::Size(),
                               Mean,Variance
                               );
        }else{
          ThrowError("RawRandomMatrix","NewNormalDevice","Only double and float are supported types.");
        }
      }else
        NewNormal();
#endif
      return *(Matrix_T<type>*)this;
    };

    Matrix_T<type>     &NewNormal()
    {
      if ( EvaSettings::MatrixPosition() == MatrixDevice ){
        return NewNormalDevice();
      }else{
        for (int i=0; i < Matrix_T<type>::Size(); i++)
          Matrix_T<type>::Data[i]=GaussRand(Mean,Variance);
        return *(Matrix_T<type>*)this;
      }
    };
  };

  typedef RawRandomMatrix<double>    RandomMatrix;
  typedef RawRandomMatrix<float>     FloatRandomMatrix;
 

  //reload the operator + - * / for  Matrix and numbers







  
};
#endif
