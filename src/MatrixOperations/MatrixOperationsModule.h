#ifndef POND_MatrixOperations_H
#define POND_MatrixOperations_H

namespace pond{

  enum MatrixOperationsType{ MatrixOperations_FFT_FORWARD, MatrixOperations_FFT_BACKWARD};
  class MatrixOperationsModule:public Module{
    static MPIModule* mpiModule;
  public:
    MatrixOperationsModule(); ~MatrixOperationsModule();
    void            Init(); 
    void            Inverse33(Matrix &in_mat,Matrix &out_mat);
    void            Inverse(Matrix &in_mat,Matrix &out_mat);
    int             PD_MatrixInverse(Object&);
    static void     FFT(int rank,int n[], ComplexMatrix &in_mat, ComplexMatrix &out_mat, MatrixOperationsType direction = MatrixOperations_FFT_FORWARD);
    static void     FFT(int rank,int n[], FloatComplexMatrix &in_mat, FloatComplexMatrix &out_mat, MatrixOperationsType direction = MatrixOperations_FFT_FORWARD);
    static void     FFT3D(int n1, int n2, int n3, ComplexMatrix &in_mat, ComplexMatrix &out_mat, MatrixOperationsType direction = MatrixOperations_FFT_FORWARD);
    static void     FFT3D(int n1, int n2, int n3, Matrix &in_mat, ComplexMatrix &out_mat);
    static void     FFT3D(int n1, int n2, int n3, ComplexMatrix &in_mat, Matrix &out_mat);
    static void     FFT3D(int n1, int n2, int n3, FloatComplexMatrix &in_mat, FloatComplexMatrix &out_mat, MatrixOperationsType direction = MatrixOperations_FFT_FORWARD);
    static void     FFT3D(int n1, int n2, int n3, FloatMatrix &in_mat, FloatComplexMatrix &out_mat);
    static void     FFT3D(int n1, int n2, int n3, FloatComplexMatrix &in_mat, FloatMatrix &out_mat);

    static void     MPI_FFT3D(int n1, int n2, int n3, ComplexMatrix &in_mat, ComplexMatrix &out_mat, MatrixOperationsType direction=MatrixOperations_FFT_FORWARD);
    static void     MPI_FFT3D(int n1, int n2, int n3, Matrix &in_mat, ComplexMatrix &out_mat);
    static void     MPI_FFT3D(int n1, int n2, int n3, ComplexMatrix &in_mat, Matrix &out_mat);
    int             PD_MatrixMPI_FFT3DC2C(Object&);
    int             PD_fftTest(Object&);
  };
};
#endif
