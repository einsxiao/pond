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
    /* static int       List2Array(Object&Argv,double*arr); */
    /* static int       List2Array(Object&Argv,complex*arr); */

    static int       Object2Matrix(Object&,    Matrix&);
    static int       Object2Matrix(Object&,    ComplexMatrix&);
    //store Matrix to table
    int              Object2Matrix(Object&,    std::string matname); 
    ///////////////////////////////
    static int       Array2List(Object&Argv,  int       *arr,   int  *dim);
    static int       Array2List(Object&Argv,  long long *arr,   int  *dim);
    static int       Array2List(Object&Argv,  float     *arr,   int  *dim);
    static int       Array2List(Object&Argv,  double    *arr,   int  *dim);
    static int       Array2List(Object&Argv,  complex   *arr,   int  *dim);

    static int       Matrix2Object(Matrix&,              Object&);
    static int       Matrix2Object(ComplexMatrix&,       Object&);
    static int       Matrix2Object(FloatMatrix&,         Object&);
    static int       Matrix2Object(FloatComplexMatrix&,  Object&);
    static int       Matrix2Object(IntMatrix&,           Object&);

    //get matrix from mat table
    int              Matrix2Object(std::string,          Object&); 
    ///////////////////////////////
    int              PD_Matrix(Object&Argv);
    ///////////////////////////////
    static bool      MatrixQ(Object&obj,Object&dim);
    static bool      MatrixQ(Object&obj);
    bool             MatrixExist(std::string name);
    int              PD_MatrixExist(Object&);
    ///////////////////////////////
    int              PD_GetMatrixPosition(Object&);
    int              PD_SetMatrixPosition(Object&);
    ///////////////////////////////
    int              PD_test(Object&);
  };
  
  inline int operator>>(Object Argv,Matrix &matrix)
  {
    return MatrixModule::Matrix2Object(matrix,Argv);
  }

  inline int operator>>(Matrix &matrix,Object Argv )
  {
    return MatrixModule::Object2Matrix(Argv, matrix);
  }
  
};
#endif
