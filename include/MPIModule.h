#ifndef POND_MPI_H
#define POND_MPI_H

namespace pond{

  class MPIModule:public Module{
  public: // all are in MPI_COMM_WORLD
    MPIModule();   ~MPIModule();
    bool            initialized = false;
    int             rankID = 0;
    int             rankSize = 1;
    std::string     processorName;
    void            Init(); 
    void            Finalize();

    void            Distribute(Matrix&global_matrix,Matrix&local_matrix,int ith=1);
    void            Distribute(FloatMatrix&global_matrix,FloatMatrix&local_matrix,int ith=1);
    void            Distribute(ComplexMatrix&global_matrix,ComplexMatrix&local_matrix,int ith=1);
    void            Distribute(FloatComplexMatrix&global_matrix,FloatComplexMatrix&local_matrix,int ith=1);
    void            Distribute(IntMatrix&global_matrix,IntMatrix&local_matrix,int ith=1);

    void            Collect(Matrix&local_matrix,Matrix&global_matrix,int ith=1);
    void            Collect(FloatMatrix&local_matrix,FloatMatrix&global_matrix,int ith=1);
    void            Collect(ComplexMatrix&local_matrix,ComplexMatrix&global_matrix,int ith=1);
    void            Collect(FloatComplexMatrix&local_matrix,FloatComplexMatrix&global_matrix,int ith=1);
    void            Collect(IntMatrix&local_matrix,IntMatrix&global_matrix,int ith=1);

    void            Barrier();
    bool            IsRootRank();
    int             PD_MPITest(Object&);
  };

};
#endif
