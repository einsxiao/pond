#ifndef EvaDefinition_POND_H
#define EvaDefinition_POND_H

namespace pond{


  ///////////////////////////////////////////
  const int   MaxRecursionDepth		          = 256;
  const int   NormalQuitCode		            = 999;
  const int   ErrorQuitCode			            = -999;
  const int   ErrorContinueCode	           	= -1;

  const int   COMPARE_LESS 			            = -1;
  const int   COMPARE_EQUAL 		            = 0;
  const int   COMPARE_BEGGER 		            = 1;

  const int 	MatrixStateNormal		          = 0;
  const int 	MatrixStateCopied		          = 1;
  const int 	MatrixStateTempFree           = 2;
  const int 	MatrixStateTempOccupied       = 3;

  const int 	MatrixNone		                = 0;
  const int 	MatrixHost		                = 1;
  const int 	MatrixDevice	                = 2;
  const int 	MatrixHostDevice              = 3;

  const int 	DataNone		                = 0;
  const int 	DataHost		                = 1;
  const int 	DataDevice	                = 2;
  const int 	DataHostDevice              = 3;

  /* const int 	CalculatePositionHost		      = 1; */
  /* const int 	CalculatePositionDevice	      = 2; */

  const int   ParallelModeNormal             = 0;
  const int   ParallelModeCpu                = 1;
  const int   ParallelModeGpu                = 0;

  const int   RunningModeNormal             = 0;
  const int   RunningModeCpu                = 1;
  const int   RunninglModeGpu               = 0;

  const int   __CudaMaxBlockNumber          = 65535;

  const int   KernelStatusImport            = 11;
  const int   KernelStatusBreak             = 44;
  const int   KernelStatusContinue          = 66;
  const int   KernelStatusReturn            = 88;
  const int   KernelStatusQuit              = 110;
  const int   KernelStatusErrorThrow        = 120;
  const int   KernelStatusError             = 119;
  
  class __EvaSettings{
  public:
    int    DataPosition;                  //default Host
    int    ParallelMode;                  //default Gpu 
    int    precision;                     //15
    bool   scientificFormat;              //false
    double epsilon;                       //0.000000000000005
    int    threadNumberPerBlock;          //should be multiple of 32
    int    threadNumberPerKernel;         //4
    ///////////////////////////////////////////////////////////////////////
    void   SetPrintFormat(int precision,bool sci=false){
      precision = precision;
      if ( precision<=0 )  precision = 1;
      if ( precision>=16 ) precision = 16;
      scientificFormat = sci;
    };
    void   SetThreadNumberPerBlock(int num){
      if ( num <= 0 and num%32 != 0 and num > 1024 ) {
        threadNumberPerBlock = 128;
        return;
      }
      threadNumberPerBlock = num;
    };
    void   SetThreadNumberPerKernel(int num){
      if ( num <= 0 ) {
        num = 1;
        return;
      }
      threadNumberPerKernel = num;
    };
  };
  template<int id> class _EvaSettings{
  public:
    static __EvaSettings settings;
  };
  template<int id> __EvaSettings _EvaSettings<id>::settings;

#define EvaSettings (_EvaSettings<0>::settings)

  __attribute__((always_inline)) inline int GetParallelMode(){
#ifdef __CUDACC__
    return EvaSettings.ParallelMode;
#else
    return pond::ParallelModeCpu;
#endif
  }
  __attribute__((always_inline)) inline  void SetParallelMode(int mode){
    EvaSettings.ParallelMode = mode;
  }
  const auto GetRunningMode = GetParallelMode;
  const auto SetRunningMode = SetParallelMode;

  __attribute__((always_inline)) inline int GetDataPosition(){
#ifdef __CUDACC__
    if ( EvaSettings.ParallelMode == ParallelModeGpu ){
      return EvaSettings.DataPosition;
    } else {
      return MatrixHost;
    }
#else
    return MatrixHost;
#endif
  }
  __attribute__((always_inline)) inline void SetDataPosition(int pos){
    EvaSettings.DataPosition = pos;
  }
  const auto GetMatrixPosition = GetDataPosition;
  const auto SetMatrixPosition = SetDataPosition;

  __attribute__((always_inline)) inline bool IsDataOnHost(){
#ifdef __CUDACC__
    if ( EvaSettings.ParallelMode == ParallelModeGpu ){
      return ( EvaSettings.DataPosition == DataHost || EvaSettings.DataPosition == DataHostDevice );
    } else {
      return true;
    }
#else
    return MatrixHost;
#endif
  }
  __attribute__((always_inline)) inline bool IsDataOnDevice(){
#ifdef __CUDACC__
    if ( EvaSettings.ParallelMode == ParallelModeGpu ){
      return ( EvaSettings.DataPosition == DataDevice || EvaSettings.DataPosition == DataHostDevice );
    } else {
      return true;
    }
#else
    return MatrixDevice;
#endif
  }

  __attribute__((always_inline)) inline void SetPrintFormat(int precision,bool sci=false){
    EvaSettings.SetPrintFormat(precision, sci);
  }

  __attribute__((always_inline)) inline void SetThreadNumberPerBlock(int num){
    EvaSettings.SetThreadNumberPerBlock(num);
  }

  __attribute__((always_inline)) inline void SetThreadNumberPerKernel(int num){
    EvaSettings.SetThreadNumberPerKernel(num);
  }

}

#endif
