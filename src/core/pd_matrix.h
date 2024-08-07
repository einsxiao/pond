#ifndef Matrix_T_POND_H_Eins
#define Matrix_T_POND_H_Eins

namespace pond{

  template<class type>
  class Matrix_T{
  public:
    // state info means 0: normal 1: copied  2: temp_unuse 3: temp_in_use
	
    uint8_t                    state=0, ND=0;
    uint16_t                   D4=0,    D5=0,    D6=0,    D7=0,    D8=0;
    uint32_t                   D1=0,    D2=0,    D3=0;
    type                      *Data = NULL;
    type                      *DataDevice = NULL;
    // size of matrix 1+1+5*2+3*4+2*8 = 40 B
  public:
    __cond_host_device__       Matrix_T();
    __cond_host_device__      ~Matrix_T();
    __cond_host_device__       Matrix_T(const Matrix_T&);
    Matrix_T(int*dim, int dataPosition=MatrixHost);
    Matrix_T(int n,...);
    void                       SetDim(const int p,const int Np);
    __cond_host_device__ int   DimN()const;
    __cond_host_device__ int   Dim(const int p)const;
    u_int                      Size()const;
    int                       *NewDimArray();
    void                       Init(int*dim,int dataPosition=MatrixHost,bool veryInit=false);
    template<class otype> void Init(const Matrix_T<otype>&,int dataPosition=MatrixHost,bool veryInit=false);
    void                       Init(int n,...);
    __cond_host_device__ bool  Free();
    ////////////////////////////////////////////////////////
    __cond_host_device__ bool  SameDimensionQ(int *);
    template<class otype>
    __cond_host_device__ bool  SameDimensionQ(const Matrix_T<otype>&);
    template<class otype> int  DimensionCompare(const Matrix_T<otype>&mat);
    int                        DimensionCompare(int *);
    int                        Malloc();
    int                        MallocDevice();
    int                        HostToDevice();
    int                        DeviceToHost();

    type                       MinRevised();
    type                       MaxRevised();

    type                       TotalRevised();
    type                       TotalDevice();
    type                       TotalHost();

    int                        Count(type cut_value);
    type                       CountDevice(type cut_value);
    type                       CountHost(type cut_value);

    __cond_host_device__ type &Part(int *pArr);
    __cond_host_device__ type &Index(int *pArr);
    __cond_host_device__ type &operator()(int *pArr);
    __cond_host_device__ type &operator[](int p);
    __cond_host_device__ type &operator()(int p);
    __cond_host_device__ type &operator()(int p1,int p2);
    __cond_host_device__ type &operator()(int p1,int p2,int p3);
    __cond_host_device__ type &operator()(int p1,int p2,int p3,int p4);
    __cond_host_device__ type &operator()(int p1,int p2,int p3,int p4,int p5);
    __cond_host_device__ type &operator()(int p1,int p2,int p3,int p4,int p5,int p6);
    __cond_host_device__ type &operator()(int p1,int p2,int p3,int p4,int p5,int p6,int p7);
    __cond_host_device__ type &operator()(int p1,int p2,int p3,int p4,int p5,int p6,int p7,int p8);
    __cond_host_device__ type &at(int p);
    __cond_host_device__ type &at(int p1,int p2);
    __cond_host_device__ type &at(int p1,int p2,int p3);
    __cond_host_device__ type &at(int p1,int p2,int p3,int p4);
    __cond_host_device__ type &at(int p1,int p2,int p3,int p4,int p5);
    __cond_host_device__ type &at(int p1,int p2,int p3,int p4,int p5,int p6);
    __cond_host_device__ type &at(int p1,int p2,int p3,int p4,int p5,int p6,int p7);
    __cond_host_device__ type &at(int p1,int p2,int p3,int p4,int p5,int p6,int p7,int p8);

    //fortran index style with collomn major and index starting with 1
    __cond_host_device__ type &fPart(int *pArr);
    __cond_host_device__ type &fIndex(int *pArr);
    /* __cond_host_device__ type &operator()(char,int *pArr); */
    /* __cond_host_device__ type &operator()(char,int p); */
    /* __cond_host_device__ type &operator()(char,int p1,int p2); */
    /* __cond_host_device__ type &operator()(char,int p1,int p2,int p3); */
    /* __cond_host_device__ type &operator()(char,int p1,int p2,int p3,int p4); */
    /* __cond_host_device__ type &operator()(char,int p1,int p2,int p3,int p4,int p5); */
    /* __cond_host_device__ type &operator()(char,int p1,int p2,int p3,int p4,int p5,int p6); */
    /* __cond_host_device__ type &operator()(char,int p1,int p2,int p3,int p4,int p5,int p6,int p7); */
    /* __cond_host_device__ type &operator()(char,int p1,int p2,int p3,int p4,int p5,int p6,int p7,int p8); */
    __cond_host_device__ type &fat(int p);
    __cond_host_device__ type &fat(int p1,int p2);
    __cond_host_device__ type &fat(int p1,int p2,int p3);
    __cond_host_device__ type &fat(int p1,int p2,int p3,int p4);
    __cond_host_device__ type &fat(int p1,int p2,int p3,int p4,int p5);
    __cond_host_device__ type &fat(int p1,int p2,int p3,int p4,int p5,int p6);
    __cond_host_device__ type &fat(int p1,int p2,int p3,int p4,int p5,int p6,int p7);
    __cond_host_device__ type &fat(int p1,int p2,int p3,int p4,int p5,int p6,int p7,int p8);

    Matrix_T                   SubMatrix(int n,...); // n is the number of suffix of to draw the subMatrix out
    Matrix_T                   ReShape(int n,...); //same Size in different shape, Data kept
    Matrix_T                   ReShape(int*dim); //same Size in different shape, Data kept

    template<class otype> Matrix_T &Set(const Matrix_T<otype> &);
    template<class otype> Matrix_T &operator=(const Matrix_T<otype> &);
    template<class otype> Matrix_T &operator=(Matrix_T<otype> &);
    template<class otype> Matrix_T &operator=(const otype);

    template<class otype> Matrix_T &operator+=(const Matrix_T<otype> &);
    template<class otype> Matrix_T &operator+=(const otype);

    template<class otype> Matrix_T &operator*=(const Matrix_T<otype> &);
    template<class otype> Matrix_T &operator*=(const otype);

    template<class otype> Matrix_T &operator-=(const Matrix_T<otype> &);
    template<class otype> Matrix_T &operator-=(const otype);

    template<class otype> Matrix_T &operator/=(const Matrix_T<otype> &);
    template<class otype> Matrix_T &operator/=(const otype);
    Matrix_T                       &operator^=(const double);

    int  ReadFile(std::string filename);
    int  ReadVTKFile(std::string filename);
    int  BinaryReadFile(std::string filename);
    int  DumpFile(std::string filename);
    int  DumpVTKFile(std::string filename);
    int  BinaryDumpFile(std::string filename);
    template<class ostype,class mtype> friend ostype &operator<<(ostype &os,const Matrix_T<mtype>&data);
    template<class istype,class mtype> friend istype &operator>>(istype &is,Matrix_T<mtype>&data);
    static u_int CalculateSize(int*dim);
  };
  template<class type>
  u_int Matrix_T<type>::CalculateSize(int*dim){
    int size = 1;
    for ( int i=1; i<= dim[0]; i++ )
      size*= dim[i];
    return size;
  }

  template<class ostype,class type> ostype &operator<<(ostype &os, const Matrix_T<type>&data){
    if (data.Size() == 0 ){ // not assigned space
      _Warning("Matrix_T","No element in Matrix_T");
      return os;
    }
    for (int i=0; i<= data.ND; i++)
      os<<data.Dim(i)<<" ";
    os<<"\n";
    for ( int i = 0; i < data.Size() ; i++)
      os<<data.Data[i]<<"\n";
    return os;
  };

  template<class istype,class type> istype &operator>>(istype &is,Matrix_T<type>&data){
    data.Free();
    int n; is>>n;
    if ( n > 8 ) {
      _Erroring("Matrix::>>","Matrix only supported max to 8 dimensions.");
      return is;
    }
    data.ND	=	n;
    size_t EleSize =	1;
    for (int i = 1; i <= n ; i++){
      int num;
      is>>num;
      data.SetDim(i,num);
      EleSize*=num;
    }
    data.Malloc();
    for (u_int i=0; i< EleSize; i++ )
      is>>data.Data[i];
    return is;
  }

  template<class type> __cond_host_device__ Matrix_T<type>::Matrix_T(){};
  template<class type> __cond_host_device__ Matrix_T<type>::~Matrix_T(){
    Free();
  };
    
  template<class type> __cond_host_device__ Matrix_T<type>::Matrix_T(const Matrix_T<type>&other){
    state = MatrixStateCopied;
#define CP(item) item = other.item
    CP(ND);CP(D4);CP(D5);CP(D6);CP(D7);CP(D8);
    CP(D1);CP(D2);CP(D3);
#ifndef __CUDA_ARCH__
    CP(Data);
#endif
#ifdef __CUDACC__
    CP(DataDevice);
#endif
#undef CP
  }

  template<class type> __cond_host_device__ int Matrix_T<type>::Dim(const int p)const{
    switch (p){
    case 0: return DimN();
    case 1: return D1;
    case 2: return D2;
    case 3: return D3;
    case 4: return D4;
    case 5: return D5;
    case 6: return D6;
    case 7: return D7;
    case 8: return D8;
    default: return 0;
    }
  }

  template<class type> void Matrix_T<type>::SetDim(const int p,const int Np){
    switch (p){
    case 0: return ;
    case 1: D1 = Np; return;
    case 2: D2 = Np; return;
    case 3: D3 = Np; return;
    case 4: D4 = Np; return;
    case 5: D5 = Np; return;
    case 6: D6 = Np; return;
    case 7: D7 = Np; return;
    case 8: D8 = Np; return;
    default:
      _Erroring("Matrix","Dimension cannot be bigger than 8.");
      return ;
    }
  }

  template<class type> __cond_host_device__ int Matrix_T<type>::DimN()const{
    return ND;
  }

  template<class type> int *Matrix_T<type>::NewDimArray(){
    int *dim = new int[ ND +2 ];
    dim[0] = ND;
    for ( int i=1;i<=ND;i++)
      dim[i] = Dim(i);
    dim[ND+1] = Size();
    return dim;
  }

  template<class type> u_int Matrix_T<type>::Size()const{
    int size = D1;
    for ( int i=2; i<= ND ; i++ )
      size *= Dim(i);
    return size;
  }

  template<class type> __cond_host_device__ type&Matrix_T<type>::operator[](int p1){
#ifdef __CUDA_ARCH__
    return DataDevice[ p1 ];
#else
    return Data[ p1 ];
#endif
  };

#define _check_dimN(N) if ( ND != N ){                                  \
    _Erroring("Matrix::at","try get element of "+                        \
             ToString((int)ND)+" Dimension(s) Matrix with 1 index ");   \
  }
#define _check_dimith(N) if ( p##N >= D##N ){                   \
    _Erroring("Matrix","index of dimension "#N" out of range."); \
  }
#define _check_fdimith(N) if ( p##N > D##N ){                   \
    _Erroring("Matrix","index of dimension "#N" out of range."); \
  }
  //1
  template<class type> __cond_host_device__ type&Matrix_T<type>::operator()(int p1){
#ifdef __CUDA_ARCH__
    return DataDevice[ p1 ];
#else
    return Data[ p1 ];
#endif
  };
  template<class type> __cond_host_device__ type&Matrix_T<type>::at(int p1){
#ifdef __CUDA_ARCH__
    return DataDevice[ p1 ];
#else
    _check_dimN(1);
    _check_dimith(1);
    return Data[ p1 ];
#endif
  };
  template<class type> __cond_host_device__ type&Matrix_T<type>::fat(int p1){
#ifdef __CUDA_ARCH__
    return DataDevice[ p1-1 ];
#else
    _check_dimN(1);
    _check_fdimith(1);
    return Data[ p1-1 ];
#endif
  };


  //2
  template<class type> __cond_host_device__ type&Matrix_T<type>::operator()(int p1,int p2){
#ifdef __CUDA_ARCH__
    return DataDevice[ p1*D2+p2 ];
#else
    return Data[ p1*D2+p2 ];
#endif
  };
  template<class type> __cond_host_device__ type&Matrix_T<type>::at(int p1,int p2){
#ifdef __CUDA_ARCH__
    return DataDevice[ p1*D2+p2 ];
#else
    _check_dimN(2);
    _check_dimith(1);
    _check_dimith(2);
    return Data[ p1*D2+p2 ];
#endif
  };
  template<class type> __cond_host_device__ type&Matrix_T<type>::fat(int p1,int p2){
#ifdef __CUDA_ARCH__
    return DataDevice[ (p2-1)*D2+p1-1 ];
#else
    _check_dimN(2);
    _check_fdimith(1);
    _check_fdimith(2);
    return Data[ (p2-1)*D2+p1-1 ];
#endif
  };

  //3
  template<class type> __cond_host_device__ type&Matrix_T<type>::operator()(int p1,int p2,int p3){
#ifdef __CUDA_ARCH__
    return DataDevice[(p1*D2+p2)*D3+p3 ];
#else
    return Data[(p1*D2+p2)*D3+p3 ];
#endif
  };
  template<class type> __cond_host_device__ type&Matrix_T<type>::at(int p1,int p2,int p3){
#ifdef __CUDA_ARCH__
    return DataDevice[(p1*D2+p2)*D3+p3 ];
#else
    _check_dimN(3);
    _check_dimith(1);
    _check_dimith(2);
    _check_dimith(3);
    return Data[(p1*D2+p2)*D3+p3 ];
#endif
  };
  template<class type> __cond_host_device__ type&Matrix_T<type>::fat(int p1,int p2,int p3){
#ifdef __CUDA_ARCH__
    return DataDevice[((p3-1)*D2+p2-1)*D1+p1-1 ];
#else
    _check_dimN(3);
    _check_fdimith(1);
    _check_fdimith(2);
    _check_fdimith(3);
    return Data[((p3-1)*D2+p2-1)*D1+p1-1 ];
#endif
  };
  
  //4
  template<class type> __cond_host_device__ type&Matrix_T<type>::operator()(int p1,int p2,int p3,int p4){
#ifdef __CUDA_ARCH__
    return DataDevice[ ((p1*D2+p2)*D3+p3)*D4+p4 ];
#else
    return Data[ ((p1*D2+p2)*D3+p3)*D4+p4 ];
#endif
  };
  template<class type> __cond_host_device__ type&Matrix_T<type>::at(int p1,int p2,int p3,int p4){
#ifdef __CUDA_ARCH__
    return DataDevice[ ((p1*D2+p2)*D3+p3)*D4+p4 ];
#else
    _check_dimN(4);
    _check_dimith(1);
    _check_dimith(2);
    _check_dimith(3);
    _check_dimith(4);
    return Data[ ((p1*D2+p2)*D3+p3)*D4+p4 ];
#endif
  };
  template<class type> __cond_host_device__ type&Matrix_T<type>::fat(int p1,int p2,int p3,int p4){
#ifdef __CUDA_ARCH__
    return DataDevice[ (((p4-1)*D3+p3-1)*D2+p2-1)*D1+p1-1 ];
#else
    _check_dimN(4);
    _check_fdimith(1);
    _check_fdimith(2);
    _check_fdimith(3);
    _check_fdimith(4);
    return Data[ (((p4-1)*D3+p3-1)*D2+p2-1)*D1+p1-1 ];
#endif
  };

  //5
  template<class type> __cond_host_device__ type&Matrix_T<type>::operator()(int p1,int p2,int p3,int p4,int p5){
#ifdef __CUDA_ARCH__
    return DataDevice[(((p1*D2+p2)*D3+p3)*D4+p4)*D5+p5 ];
#else
    return Data[(((p1*D2+p2)*D3+p3)*D4+p4)*D5+p5 ];
#endif
  };
  template<class type> __cond_host_device__ type&Matrix_T<type>::at(int p1,int p2,int p3,int p4,int p5){
#ifdef __CUDA_ARCH__
    return DataDevice[(((p1*D2+p2)*D3+p3)*D4+p4)*D5+p5 ];
#else
    _check_dimN(5);
    _check_dimith(1);
    _check_dimith(2);
    _check_dimith(3);
    _check_dimith(4);
    _check_dimith(5);
    return Data[(((p1*D2+p2)*D3+p3)*D4+p4)*D5+p5 ];
#endif
  };
  template<class type> __cond_host_device__ type&Matrix_T<type>::fat(int p1,int p2,int p3,int p4,int p5){
#ifdef __CUDA_ARCH__
    return DataDevice[((((p5-1)*D4+p4-1)*D3+p3-1)*D2+p2-1)*D1+p1-1 ];
#else
    _check_dimN(5);
    _check_fdimith(1);
    _check_fdimith(2);
    _check_fdimith(3);
    _check_fdimith(4);
    _check_fdimith(5);
    return Data[((((p5-1)*D4+p4-1)*D3+p3-1)*D2+p2-1)*D1+p1-1 ];
#endif
  };

  //6
  template<class type> __cond_host_device__ type&Matrix_T<type>::operator()(int p1,int p2,int p3,int p4,int p5,int p6){
#ifdef __CUDA_ARCH__
    return DataDevice[((((p1*D2+p2)*D3+p3)*D4+p4)*D5+p5)*D6+p6];
#else
    return Data[((((p1*D2+p2)*D3+p3)*D4+p4)*D5+p5)*D6+p6];
#endif
  };
  template<class type> __cond_host_device__ type&Matrix_T<type>::at(int p1,int p2,int p3,int p4,int p5,int p6){
#ifdef __CUDA_ARCH__
    return DataDevice[((((p1*D2+p2)*D3+p3)*D4+p4)*D5+p5)*D6+p6];
#else
    _check_dimN(6);
    _check_dimith(1);
    _check_dimith(2);
    _check_dimith(3);
    _check_dimith(4);
    _check_dimith(5);
    _check_dimith(6);
    return Data[((((p1*D2+p2)*D3+p3)*D4+p4)*D5+p5)*D6+p6];
#endif
  };
  template<class type> __cond_host_device__ type&Matrix_T<type>::fat(int p1,int p2,int p3,int p4,int p5,int p6){
#ifdef __CUDA_ARCH__
    return DataDevice[(((((p6-1)*D5+p5-1)*D4+p4-1)*D3+p3-1)*D2+p2-1)*D1+p1-1];
#else
    _check_dimN(6);
    _check_fdimith(1);
    _check_fdimith(2);
    _check_fdimith(3);
    _check_fdimith(4);
    _check_fdimith(5);
    _check_fdimith(6);
    return Data[(((((p6-1)*D5+p5-1)*D4+p4-1)*D3+p3-1)*D2+p2-1)*D1+p1-1];
#endif
  };

  //7
  template<class type> __cond_host_device__ type&Matrix_T<type>::operator()(int p1,int p2,int p3,int p4,int p5,int p6,int p7){
#ifdef __CUDA_ARCH__
    return DataDevice[(((((p1*D2+p2)*D3+p3)*D4+p4)*D5+p5)*D6+p6)*D7+p7];
#else
    return Data[(((((p1*D2+p2)*D3+p3)*D4+p4)*D5+p5)*D6+p6)*D7+p7];
#endif
  };
  template<class type> __cond_host_device__ type&Matrix_T<type>::at(int p1,int p2,int p3,int p4,int p5,int p6,int p7){
#ifdef __CUDA_ARCH__
    return DataDevice[(((((p1*D2+p2)*D3+p3)*D4+p4)*D5+p5)*D6+p6)*D7+p7];
#else
    _check_dimN(7);
    _check_dimith(1);
    _check_dimith(2);
    _check_dimith(3);
    _check_dimith(4);
    _check_dimith(5);
    _check_dimith(6);
    _check_dimith(7);
    return Data[(((((p1*D2+p2)*D3+p3)*D4+p4)*D5+p5)*D6+p6)*D7+p7];
#endif
  };
  template<class type> __cond_host_device__ type&Matrix_T<type>::fat(int p1,int p2,int p3,int p4,int p5,int p6,int p7){
#ifdef __CUDA_ARCH__
    return DataDevice[((((((p7-1)*D6+p6-1)*D5+p5-1)*D4+p4-1)*D3+p3-1)*D2+p2-1)*D1+p1-1];
#else
    _check_dimN(7);
    _check_fdimith(1);
    _check_fdimith(2);
    _check_fdimith(3);
    _check_fdimith(4);
    _check_fdimith(5);
    _check_fdimith(6);
    _check_fdimith(7);
    return Data[((((((p7-1)*D6+p6-1)*D5+p5-1)*D4+p4-1)*D3+p3-1)*D2+p2-1)*D1+p1-1];
#endif
  };

  //8
  template<class type> __cond_host_device__ type&Matrix_T<type>::operator()(int p1,int p2,int p3,int p4,int p5,int p6,int p7,int p8){
#ifdef __CUDA_ARCH__
    return DataDevice[((((((p1*D2+p2)*D3+p3)*D4+p4)*D5+p5)*D6+p6)*D7+p7)*D8+p8];
#else
    return Data[((((((p1*D2+p2)*D3+p3)*D4+p4)*D5+p5)*D6+p6)*D7+p7)*D8+p8];
#endif
  };
  template<class type> __cond_host_device__ type&Matrix_T<type>::at(int p1,int p2,int p3,int p4,int p5,int p6,int p7,int p8){
#ifdef __CUDA_ARCH__
    return DataDevice[((((((p1*D2+p2)*D3+p3)*D4+p4)*D5+p5)*D6+p6)*D7+p7)*D8+p8];
#else
    _check_dimN(8);
    _check_dimith(1);
    _check_dimith(2);
    _check_dimith(3);
    _check_dimith(4);
    _check_dimith(5);
    _check_dimith(6);
    _check_dimith(7);
    _check_dimith(8);
    return Data[((((((p1*D2+p2)*D3+p3)*D4+p4)*D5+p5)*D6+p6)*D7+p7)*D8+p8];
#endif
  };
  template<class type> __cond_host_device__ type&Matrix_T<type>::fat(int p1,int p2,int p3,int p4,int p5,int p6,int p7,int p8){
#ifdef __CUDA_ARCH__
    return DataDevice[(((((((p8-1)*D7+p7-1)*D6+p6-1)*D5+p5-1)*D4+p4-1)*D3+p3-1)*D2+p2-1)*D1+p1-1];
#else
    _check_dimN(8);
    _check_fdimith(1);
    _check_fdimith(2);
    _check_fdimith(3);
    _check_fdimith(4);
    _check_fdimith(5);
    _check_fdimith(6);
    _check_fdimith(7);
    _check_fdimith(8);
    return Data[(((((((p8-1)*D7+p7-1)*D6+p6-1)*D5+p5-1)*D4+p4-1)*D3+p3-1)*D2+p2-1)*D1+p1-1];
#endif
  };
#undef _check_dimN
#undef _check_dimith
#undef _check_fdimith


  template<class type> template<class otype>
  __cond_host_device__ bool Matrix_T<type>::SameDimensionQ(const Matrix_T<otype>&other){
    if ( D1 != other.D1 ) return false;
    if ( D2 != other.D2 ) return false;
    if ( D3 != other.D3 ) return false;
    if ( D4 != other.D4 ) return false;
    if ( D5 != other.D5 ) return false;
    if ( D6 != other.D6 ) return false;
    if ( D7 != other.D7 ) return false;
    if ( D8 != other.D8 ) return false;
    return true;
  }

  template<class type> __cond_host_device__ bool Matrix_T<type>::SameDimensionQ(int *dim)
  {
    if ( dim == NULL ) return false;
#ifdef __CUDA_ARCH__
    if ( dim[0]!=ND ) return false;
    for (int i=1; i<=dim[0]; i++)
      if (dim[i]!=Dim(i)) 
        return false;
    return true;
#else
    if ( dim[0] !=ND ) return false;
    for (int i=1; i<=dim[0]; i++)
      if (dim[i]!=Dim(i)) 
        return false;
    return true;
#endif
  }

  template<class type> template<class otype> int Matrix_T<type>::DimensionCompare(const Matrix_T<otype>&mat)
  {
    if ( ND > mat.ND ) return 1;
    if ( ND < mat.ND ) return -1;
    for ( int i=1; i<= ND;i++){
      if ( Dim(i) > mat.Dim(i) )
        return 1;
      else
        if ( Dim(i) < mat.Dim(i) ) return -1;
    }
    return 0;
  }

  template<class type> int Matrix_T<type>::DimensionCompare(int *dim)
  {
    if ( ND > dim[0] ) return 1;
    if ( ND < dim[0] ) return -1;
    for ( int i=1; i<= ND;i++){
      if ( Dim(i) > dim[i] )
        return 1;
      else
        if ( Dim(i) < dim[i] ) return -1;
    }
    return 0;
  }

  template<class type> __cond_host_device__ type&Matrix_T<type>::Part(int *pArr)
  {
    int dim=ND,dt,index=pArr[0];
    for (int i=1; i< dim; i++) {
      dt=Dim(i+1);
      index=pArr[i] + index*dt;
    }
#ifndef __CUDA_ARCH__
    return Data[ index ];
#else
    return DataDevice[ index ];
#endif
  };//a base

  template<class type> __cond_host_device__ type&Matrix_T<type>::Index(int *pArr)
  {
    return Part(pArr);
  }

  template<class type> __cond_host_device__ type&Matrix_T<type>::operator()(int *pArr){
    return Part(pArr);
  }

  template<class type> __cond_host_device__ type&Matrix_T<type>::fPart(int *pArr)
  {
    int dim=DimN();
    int dt;
    int index=pArr[dim-1]-1;
    for (int i=dim-1; i>=0 ; i--) {
      dt = Dim(i+1);
      index = pArr[i] -1 + index*dt;
    }
#ifndef __CUDA_ARCH__
    return Data[ index ];
#else
    return DataDevice[ index ];
#endif
  };//a base

  template<class type> __cond_host_device__ type&Matrix_T<type>::fIndex(int *pArr)
  {
    return fPart(pArr);
  }

  /* template<class type> __cond_host_device__ type&Matrix_T<type>::operator()(char ch,int *pArr) */
  /* { */
  /*   return fPart(pArr); */
  /* } */

  template<class type> void Matrix_T<type>::Init(int *dimArr,int dataPosition,bool veryInit)
  {
    if ( state == MatrixStateCopied ){ // if just copied matrix, set to empty, not free memory
      Data = NULL;
      DataDevice = NULL;
    }else{
      if ( not veryInit )
        if ( Size() < Matrix::CalculateSize( dimArr ) )
          Free();
    }
    ND	=	dimArr[0];
    int EleSize =	1;
    for (int i=1;i<=ND;i++){
      if ( dimArr[i] < 1 ){
        _Warning("Matrix::Init","Dimension smaller than 1; set to 1");
        SetDim(i,1);
      }else{
        SetDim(i,dimArr[i]); 
      }
      EleSize*=Dim(i);
    }
    Malloc();
#if defined(__CUDACC__)
    if ( ( dataPosition == MatrixDevice or dataPosition == MatrixHostDevice ) and pond::GetParallelMode() == ParallelModeGpu ){
      MallocDevice();
    }
#endif
  }

  template<class type> template<class otype>
  void Matrix_T<type>::Init(const Matrix_T<otype>&other,int dataPosition,bool veryInit)
  {
    if ( state == MatrixStateCopied ){ // if just copied matrix, set to empty, not free memory
      Data = NULL;
      DataDevice = NULL;
    }else{
      if ( not veryInit )
        if ( Size() < other.Size() )
          Free();
    }
#define CP(item) item = other.item
    CP(ND);
    CP(D1);CP(D2);CP(D3);CP(D4);CP(D5);CP(D6);CP(D7);CP(D8);
#undef CP
    Malloc();
#if defined(__CUDACC__)
    if ( ( dataPosition == MatrixDevice or dataPosition == MatrixHostDevice ) and pond::GetParallelMode() == ParallelModeGpu ){
      MallocDevice();
    }
#endif
  }


  
  template<class type> __cond_host_device__ bool Matrix_T<type>::Free()
  {
#ifndef __CUDA_ARCH__
    if ( state != MatrixStateCopied ){
      if ( Data != NULL ) 			{
        delete []Data;
        Data=NULL;
      }
#  if defined(__CUDACC__)
      if ( DataDevice != NULL ) 		{
        cudaFree( DataDevice ) ;
        DataDevice=NULL;
      }
#  endif
    }
#endif
    return 0;
  }

  template<class type> void Matrix_T<type>::Init(int n,...)
  {
    va_list args;
    va_start(args,n);
    int *arr	=	new int [ n+ 2 ];
    arr[0]=n;
    for (int i=1; i<=n; i++)
      arr[i]	=	va_arg(args,int);
    int dataPosition	=	va_arg(args,int);
    va_end(args);
    Init(arr,dataPosition);
    delete[]arr;
  }

  template<class type> Matrix_T<type>::Matrix_T(int n,...)
  { //Matrix_T x(2,8,8);
    va_list args;
    va_start(args,n);
    int *arr	=	new int [ n+ 2 ];
    arr[0]=n;
    for (int i=1; i<=n; i++)
      arr[i]	=	va_arg(args,int);
    int dataPosition	=	va_arg(args,int);
    va_end(args);
    Init(arr,dataPosition,true);
    delete[]arr;
  }

  template<class type> int Matrix_T<type>::Malloc()
  {
    if (Data == NULL){
      Data=new type[Size()];
    }
    return 0;
  }

  template<class type> int Matrix_T<type>::MallocDevice(){
#if defined(__CUDACC__)
    if ( DataDevice == NULL and pond::GetParallelMode() == pond::ParallelModeGpu ){
      cudaMalloc((void**)&DataDevice,sizeof(type)*Size() );
      CUDA_LAST_ERROR();
    }
#endif
    return 0;
  }

  template<class type> int Matrix_T<type>::HostToDevice()
  {
#if defined(__CUDACC__)
    if (Data == NULL or (pond::GetParallelMode() == ParallelModeCpu) ) return -1;
    MallocDevice();
    cudaMemcpy((void*)DataDevice,(void*)Data,sizeof(type)*Size(),cudaMemcpyHostToDevice);
    cudaDeviceSynchronize();
    CUDA_LAST_ERROR();
#endif
    return 0;
  }

  template<class type>
  int Matrix_T<type>::DeviceToHost()
  {
#if defined(__CUDACC__)
    if (DataDevice == NULL or (pond::GetParallelMode() == ParallelModeCpu) )  return -1;
    Malloc();
    cudaMemcpy((void*)Data, (void*)DataDevice, sizeof(type)*Size(),cudaMemcpyDeviceToHost);
    cudaDeviceSynchronize();
    CUDA_LAST_ERROR();
#endif
    return 0;
  }

  template<class type>
  int Matrix_T<type>::ReadFile(std::string filename)
  {
    std::ifstream ifs;
    ifs.open(filename.c_str());
    if (ifs){
      ifs>>(*this); // all done by >>
      ifs.close();
      return 1;
    }else{
      return -1;
    }
  }

  template<class type>
  int Matrix_T<type>::ReadVTKFile(std::string filename)
  {
    Free();
    std::ifstream ifs;
    ifs.open(filename.c_str());
    if (ifs){
      std::string line,word;
      while ( getline( ifs, line) ){
        if ( line.compare(0,9,"DIMENSION")==0 or line.compare(0,9,"dimensions")==0 ){
          word = pond::RemoveFirstWord(line,line);
          word = pond::RemoveFirstWord(line,line);
          if ( word == "" ) { _Erroring("Matrix::ReadVTKFile", "Dimension specified is in the wrong form."); return -1; }
          D1 = pond::ToNumber( word.c_str() );
          word = pond::RemoveFirstWord(line,line);
          if ( word == "" ){
            ND = 1; continue;
          }
          D2 = pond::ToNumber( word.c_str() );
          word = pond::RemoveFirstWord(line,line);
          if ( word == "" ){
            ND = 2; continue;
          }
          D3 = pond::ToNumber( word.c_str() );
          if ( line != "" ) { _Erroring("Matrix::ReadVTKFile", "Dimension specified is in the wrong form."); return -1; }
          ND = 3;
          continue;
        }
        if ( line.compare(0,20,"LOOKUP_TABLE default" )==0 or line.compare(0,20,"lookup_table default" )==0 ){
          break;
        }
      }
      size_t size = Size();
      Malloc();
      if ( ND == 1 ){
        for ( u_int i=0; i< size; i++ )
          ifs >> Data[i];
      }else if ( ND == 2 ){
        for ( u_int j=0; j<D2; j++)
          for ( u_int i=0; i<D1; i++ )
            ifs >> (*this)(i,j);
        
      }else{
        for ( u_int k=0; k<D3; k++ )
          for ( u_int j=0; j<D2; j++)
            for ( u_int i=0; i<D1; i++ )
              ifs >> (*this)(i,j,k);
      }
      ifs.close();
      return 1;
    }else{
      return -1;
    }
  }


  template<class type>
  int Matrix_T<type>::BinaryReadFile(std::string filename)
  {
    std::ifstream ifs;
    ifs.open(filename.c_str(),std::ios::binary);
    if (!ifs){
      std::cerr<<"Error: \""<<filename<<"\" not found!\n";
      return -1;
    }
    Free();
    /////////////////////
    ifs.read( (char*)&ND, 23 );
    Malloc();
    ifs.read((char*)&Data[0],Size()*sizeof(type));
    ifs.close();
    return 0;
  }

  template<class type>
  int Matrix_T<type>::DumpFile(std::string filename)
  {
    if (Size() == 0 || Data == NULL ){ 
      _Warning("Matrix_T::DumpFile","Matrix_T is empty.");
      return -1;
    }
    std::ofstream os(filename.c_str());
    os.precision(EvaSettings.precision);
    os.setf( std::ios::uppercase);
    for (int i=0; i<= DimN(); i++)
      os<<Dim(i)<<" ";
    os<<"\n";
    int size = Size();
    for ( int i = 0; i < size ; i++)
      os<<Data[i]<<"\n";
    os.close();
    return 1;
  }

  template<class type>
  int Matrix_T<type>::DumpVTKFile(std::string filename)
  {
    if (Size() == 0 || Data == NULL ){ 
      _Warning("Matrix_T::DumpFile","Matrix_T is empty.");
      return -1;
    }
    bool resized = false;
    int  *newdim;
    if ( DimN() > 3 ){
      resized = true;
      newdim = NewDimArray();
      ReShape(3,Size()/ Dim(ND)/Dim(ND-1), Dim(ND-1), Dim(ND));
    }
    std::ofstream os(filename.c_str());
    os.precision(EvaSettings.precision);
    os.setf( std::ios::uppercase);
    os<<"# vtk DataFile Version 2.0\n";
    os<<"Volume example\n";
    os<<"ASCII\n";
    os<<"DATASET STRUCTURED_POINTS\n";
    os<<"DIMENSIONS ";
    if ( DimN() == 1 ){
      os<<Dim(1)<<" 1 1"<<"\n";
    }else if ( DimN()  == 2 ){
      os<<Dim(1)<<" "<<Dim(2)<<" 1"<<"\n";
    }else if ( DimN() == 3 ){
      os<<Dim(1)<<" "<<Dim(2)<<" "<<Dim(3)<<"\n";
    }
    os<<"ORIGIN 0 0 0"<<"\n";
    os<<"SPACING 1 1 1"<<"\n";
    os<<"POINT_DATA "<<Size()<<"\n";
    os<<"SCALARS volume_scalars float 1"<<"\n";
    os<<"LOOKUP_TABLE default"<<"\n";
    if ( ND == 3 ){
      for (u_int k=0; k<D3; k++)
        for (u_int j=0; j<D2; j++)
          for (u_int i=0; i< D1; i++)
            os<<(*this)(i,j,k)<<"\n";
    }else if ( ND == 2 ){
      for (u_int j=0; j<D2; j++)
        for (u_int i=0; i< D1; i++)
          os<<(*this)(i,j)<<"\n";
    }else{
      for (u_int i=0; i< D1; i++)
        os<<(*this)(i)<<"\n";
    }
    os.close();
    if ( resized ){
      ReShape( newdim );
      delete []newdim;
    }
    return 1;
  }

  template<class type>
  int Matrix_T<type>::BinaryDumpFile(std::string filename)
  {
    if (Size() ==0 || Data == NULL ){ 
      _Warning("Matrix_T::BinaryDumpFile","Matrix_T is empry.");
      return -1;
    }
    std::ofstream os(filename.c_str(),std::ios::binary);
    os.precision(EvaSettings.precision);
    os.setf( std::ios::uppercase);
    os.write( (char*)&ND, 23 );
    os.write( (char*)&Data[0], Size()*sizeof(type) );
    os.close();
    return 1;
  }

  
  template<class type> Matrix_T<type> Matrix_T<type>::SubMatrix(int n, ...)
  {
    if ( n >= ND ) {
      _Erroring("SubMatrix","SubMatrix dimension equal or ran out of the original Matrix."); 
      return (*this); 
    }
    Matrix_T<type> tMatrix;
    tMatrix.state = MatrixStateCopied; 
    tMatrix.ND = ND-n;
    for ( int i=1;i<=ND-n;i++ ){
      tMatrix.SetDim(i, Dim(i+n) );
    }

    int count = 0;
    va_list args;
    va_start(args,n);
    for (int i=1; i<=n; i++){
      count += va_arg(args,int);
      count *= Dim(i+1);
    }
    va_end(args);
    for ( int i= n+1; i< ND ;i ++ )
      count *= Dim(i+1);
    if ( Data != NULL ) tMatrix.Data = Data+count;
    if ( DataDevice != NULL ) tMatrix.DataDevice = DataDevice+count;
    return tMatrix;
  }

  template<class type> Matrix_T<type> Matrix_T<type>::ReShape(int n, ...)
  {
    va_list args;
    va_start(args,n);
    int *new_dim = new int(n+2);
    new_dim[0] = n; new_dim[n+1] = 1;
    for (int i=1; i<=n; i++){
      new_dim[i] = va_arg(args,int);
      new_dim[n+1]*= new_dim[i];
    }
    va_end(args);

    if ( new_dim[n+1] > Size() ) {
      _Erroring("Matrix","Matrix can only be reshaped to a matrix of the same or smaller size."); 
      return (*this);
    }

    ND = n;
    for ( int i=1; i <= ND; i++ ){
      SetDim(i, new_dim[i] );
    }
    return (*this);
  }

  template<class type> Matrix_T<type> Matrix_T<type>::ReShape(int *dim)
  {
    int size=1;
    for (int i=1; i<=dim[0]; i++){
      size *= dim[i];
    }
    if ( size > Size() ) {
      _Erroring("Matrix","Matrix can only be reshaped to a matrix of the same or smaller size."); 
      return (*this); 
    }

    ND = dim[0];
    for ( int i=1; i<=ND; i++ ){
      SetDim(i, dim[i] );
    }
    return (*this);
  }

  template<class type> template<class otype> Matrix_T<type>& Matrix_T<type>::Set(const Matrix_T<otype>& other)
  {
    if ( other.Size() <= 0 ) {
      _Erroring("Matrix::Set","Right Matrix is Empty."); return (*this); 
    }
    if ( not SameDimensionQ(other) )
      Init(other,pond::GetDataPosition() ); 
#ifdef __CUDACC__
    if ( pond::IsDataOnHost() ){
      if ( other.Data != NULL and Data != NULL ){
        if ( typeid(type) == typeid(otype) ){
          memcpy(Data,other.Data,sizeof(type)*Size());
        }else{
          set_host(Data,other.Data,Size());
        }
      }
    }
    if ( pond::IsDataOnDevice()){
      if ( other.DataDevice != NULL and DataDevice != NULL ){
        if ( typeid(type) == typeid(otype) ){
          cudaMemcpy(DataDevice,other.DataDevice,sizeof(type)*Size(),cudaMemcpyDeviceToDevice);
        }else{
          set_device(DataDevice,other.DataDevice,Size());
        }
      }
    }
#else
    if ( other.Data != NULL and Data != NULL ){
      if ( typeid(type) == typeid(otype) ){
        memcpy(Data,other.Data,sizeof(type)*Size() );
      }else{
        set_host(Data,other.Data,Size() );
      }
    }
#endif
    return *this;
  }

  template<class type> template<class otype> Matrix_T<type>& Matrix_T<type>::operator=(const Matrix_T<otype>& other)
  {
    return Set(other);
  }

  //if the 'const' is added, when evaluating expression 'mat1 = mat2' will not call the overload operator '='
  template<class type> template<class otype> Matrix_T<type>& Matrix_T<type>::operator=(Matrix_T<otype>& other)
  {
    return Set(other);
  }

  template<class type> template<class vtype>
  Matrix_T<type> &Matrix_T<type>::operator=(const vtype value)
  {
    if (Size() <=0 ){
      _Warning("Matrix_T::Set","Matrix_T is Empty.");
      return *this;
    }
#if defined(__CUDACC__)
    if ( pond::IsDataOnHost() ){ 
      Malloc();
      set_host(Data, value , Size());
    }
    if ( pond::IsDataOnDevice() ){
      MallocDevice();
      set_device(DataDevice,value,Size() );
    }
#else
    Malloc();
    set_host(Data, value , Size() );
#endif
    return *this;
  }

#define OPER_MATRIX_OVERLOAD_CU(name,oper,func)                         \
  template<class type> template<class otype>                            \
  Matrix_T<type> &Matrix_T<type>::operator oper(const Matrix_T<otype>&other){ \
    if ( other.Size() <= 0 ) {                                          \
      _Erroring("Matrix::"#name,"Right Matrix is Empty.");               \
      return (*this);                                                   \
    }                                                                   \
    if ( not SameDimensionQ(other) )  {                                 \
      _Erroring("Matrix::"#name,"Two Matrix is not the same size.");     \
      return (*this);                                                   \
    }                                                                   \
    if ( pond::IsDataOnHost() ){                                        \
      if ( other.Data != NULL and Data != NULL )                        \
        func##_host(Data,Data,other.Data,Size());                       \
    }else                                                               \
      if ( pond::IsDataOnDevice()){                                     \
      if ( other.DataDevice != NULL and DataDevice != NULL )            \
        func##_device(DataDevice,DataDevice,other.DataDevice,Size());   \
    }                                                                   \
    if ( other.state == MatrixStateTempOccupied )                       \
      *(const_cast<u_char*>(&other.state)) = MatrixStateTempFree;       \
    if ( state == MatrixStateTempFree )                                 \
      state = MatrixStateTempOccupied;                                  \
    return *this;                                                       \
  };                                                                    \
  template<class type> template<class vtype>                            \
    Matrix_T<type> &Matrix_T<type>::operator oper(const vtype value){   \
    if ( Size() <= 0 ){                                                 \
      _Erroring("Matrix::"#name,"Matrix is Empty.");                     \
      return *this;                                                     \
    }                                                                   \
    if ( pond::IsDataOnHost() ){                                        \
      func##_host(Data,Data,value,Size() );                             \
    }else                                                               \
      if ( pond::IsDataOnDevice()){                                     \
      func##_device(DataDevice,DataDevice,value,Size() );               \
    }                                                                   \
    if ( state==MatrixStateTempFree ) state = MatrixStateTempOccupied;  \
    return *this;                                                       \
  };

#define OPER_MATRIX_OVERLOAD(name,oper,func)                            \
  template<class type> template<class otype>                            \
  Matrix_T<type> &Matrix_T<type>::operator oper(const Matrix_T<otype>&other){ \
    if ( other.Size() <= 0 ) {                                          \
      _Erroring("Matrix::"#name,"Right Matrix is Empty.");               \
      return *this;                                                     \
    }                                                                   \
    if ( not SameDimensionQ(other) ) {                                  \
      _Erroring("Matrix::"#name,"Two Matrix is not the same size.");     \
      return *this;                                                     \
    }                                                                   \
    if ( other.Data != NULL and Data != NULL ){                         \
      func##_host(Data,Data,other.Data,Size());                         \
    }                                                                   \
    if ( other.state == MatrixStateTempOccupied )                       \
      *(const_cast<u_char*>(&other.state)) = MatrixStateTempFree;       \
    if ( state == MatrixStateTempFree )                                 \
      state = MatrixStateTempOccupied;                                  \
    return *this;                                                       \
  };                                                                    \
  template<class type> template<class vtype>                            \
  Matrix_T<type> &Matrix_T<type>::operator oper(const vtype value){     \
    if ( Size() <= 0 )                                                  \
      { _Erroring("Matrix::"#name,"Matrix is Empty."); return *this; }   \
    func##_host(Data,Data,value,Size() );                               \
    if ( state==MatrixStateTempFree ) state = MatrixStateTempOccupied;  \
    return *this;                                                       \
  };

#ifdef __CUDACC__
  OPER_MATRIX_OVERLOAD_CU(AddTo,+=,add)
  OPER_MATRIX_OVERLOAD_CU(MinusBy,-=,minus)
  OPER_MATRIX_OVERLOAD_CU(TimesBy,*=,multi)
  OPER_MATRIX_OVERLOAD_CU(DivideBy,/=,divi)
#else
  OPER_MATRIX_OVERLOAD(AddTo,+=,add)
  OPER_MATRIX_OVERLOAD(MinusBy,-=,minus)
  OPER_MATRIX_OVERLOAD(TimesBy,*=,multi)
  OPER_MATRIX_OVERLOAD(DivideBy,/=,divi)
#endif

#undef OPER_MATRIX_OVERLOAD
#undef OPER_MATRIX_OVERLOAD_CU

  template<class type>
  Matrix_T<type> &Matrix_T<type>::operator ^=(const double value)
  { 
    if ( Size() <= 0 ) 
      { _Erroring("Matrix::PowerBy","Matrix is Empty.");  return *this; }
#ifdef __CUDACC__
    if ( pond::IsDataOnHost() ){ 
      power_host(Data,Data,value,Size() );        
    } else {
      if ( pond::IsDataOnDevice()){ 
        power_device(DataDevice,DataDevice,value,Size() );  
      } 
    }
#else
    power_host(Data,Data,value,Size() );        
#endif
    return *this; 
  };

#ifdef __CUDACC__ 
  template<class type>
  __global__ void Matrix_Min_Kernel(type *arr,int n_in)
  {
    int tid= blockIdx.x*blockDim.x+threadIdx.x;
    int n_cal= gridDim.x*blockDim.x;
    int n_out= n_in/2;
    int n_inc; if (n_in%2==0) n_inc=n_out; else n_inc=n_out+1;
    while (tid<n_out){
      if ( arr[tid+n_inc] < arr[tid] ){
        arr[tid] = arr[tid+n_inc];
      }
      tid+=n_cal;
    }
  }
  template<class type>
  __global__ void Matrix_Max_Kernel(type *arr,int n_in)
  {
    int tid= blockIdx.x*blockDim.x+threadIdx.x;
    int n_cal= gridDim.x*blockDim.x;
    int n_out= n_in/2;
    int n_inc; if (n_in%2==0) n_inc=n_out; else n_inc=n_out+1;
    while (tid<n_out){
      if ( arr[tid+n_inc] > arr[tid] ){
        arr[tid] = arr[tid+n_inc];
      }
      tid+=n_cal;
    }
  }
  template<class type>
  __global__ void Matrix_Total_Kernel(type *arr,int n_in)
  {
    int tid= blockIdx.x*blockDim.x+threadIdx.x;
    int n_cal= gridDim.x*blockDim.x;
    int n_out= n_in/2;
    int n_inc; if (n_in%2==0) n_inc=n_out; else n_inc=n_out+1;
    while (tid<n_out){
      arr[tid]+=arr[tid+n_inc];
      tid+=n_cal;
    }
  }
#endif

  template<class type>
  type Matrix_T<type>::MinRevised()
  {
    int n_in=Size();
#ifdef __CUDACC__ 
    if ( pond::GetParallelMode() == ParallelModeGpu ){
      int tn = __CudaThreadNumberPerBlock;
      int bn;
      if ( n_in == 0 or DataDevice == NULL ) {
        _Erroring("Matrix_T::MinRevised","Matrix_T has no data on device side."); return 0;
      }
      while (n_in >1){
        bn = (n_in-1)/tn+1; if (bn > __CudaMaxBlockNumber) bn = __CudaMaxBlockNumber; 
        if (tn > n_in/2) {tn =n_in/2; bn=1;}
        Matrix_Min_Kernel<<<bn, tn>>>(DataDevice, n_in);
        n_in = (n_in+1)/2;
        cudaDeviceSynchronize();
      }
      type sum;
      cudaMemcpy(&sum,DataDevice,sizeof(type),cudaMemcpyDeviceToHost); // only the final sum is needed
      return sum;
    }
#endif
    if ( n_in == 0 or Data == NULL ) {
      _Erroring("Matrix_T::MinRevised","Matrix_T has no data on host side."); return 0;
    }
    type value = Data[0];
#pragma omp parallel for reduction(min:value)
    for (int i =1; i< Size(); i++){
      if ( Data[i] < value ){
        value = Data[i];
      }
    }
    return value;
  }

  template<class type>
  type Matrix_T<type>::MaxRevised()
  {
    type value;
    int n_in=Size();
#ifdef __CUDACC__ 
    if ( pond::GetParallelMode() == ParallelModeGpu ){
      int tn = __CudaThreadNumberPerBlock;
      int bn;
      if ( n_in == 0 or DataDevice == NULL ) {
        _Erroring("Matrix_T::MaxRevised","Matrix_T has no data on device side."); return 0;
      }
      while (n_in >1){
        bn = (n_in-1)/tn+1; if (bn > __CudaMaxBlockNumber) bn = __CudaMaxBlockNumber; 
        if (tn > n_in/2) {tn =n_in/2; bn=1;}
        Matrix_Max_Kernel<<<bn, tn>>>(DataDevice, n_in);
        n_in = (n_in+1)/2;
        cudaDeviceSynchronize();
      }
      cudaMemcpy(&value,DataDevice,sizeof(type),cudaMemcpyDeviceToHost); // only the final sum is needed
      return value;
    }
#endif
    if ( n_in == 0 or Data == NULL ) {
      _Erroring("Matrix_T::MaxRevised","Matrix_T has no data on host side."); return 0;
    }
    value = Data[0];
#pragma omp parallel for reduction(max:value)
    for (int i =1; i< Size(); i++){
      if ( Data[i] > value ){
        value = Data[i];
      }
    }
    return value;
  }

  template<class type>
  type Matrix_T<type>::TotalRevised()
  {
    type sum;
    int n_in=Size();
#ifdef __CUDACC__ 
    if ( pond::GetParallelMode() == ParallelModeGpu ){
      int tn = __CudaThreadNumberPerBlock;
      int bn;
      if ( n_in == 0 or DataDevice == NULL ) {
        _Erroring("Matrix_T::TotalRevised","Matrix_T has no data on device side."); return 0;
      }
      while (n_in >1){
        bn = (n_in-1)/tn+1; if (bn > __CudaMaxBlockNumber) bn = __CudaMaxBlockNumber; 
        if (tn > n_in/2) {tn =n_in/2; bn=1;}
        Matrix_Total_Kernel<<<bn, tn>>>(DataDevice, n_in);
        n_in = (n_in+1)/2;
        cudaDeviceSynchronize();
      }
      cudaMemcpy(&sum,DataDevice,sizeof(type),cudaMemcpyDeviceToHost); // only the final sum is needed
      return sum;
    }
#endif
    if ( n_in == 0 or Data == NULL ) {
      _Erroring("Matrix_T::TotalRevised","Matrix_T has no data on host side."); return 0;
    }
    sum = Data[0];
#pragma omp parallel for reduction(+:sum)
    for (int i =1; i< Size(); i++){
      sum += Data[i];
    }
    return sum;
  }

  template<class type>
  type Matrix_T<type>::TotalHost()
  {
    _Warning("Matrix_T::TotalRevised","TotalHost will be depreciated. Please use pond::Matrix_T::TotalRevised instead.");
    return TotalRevised();
  }

  template<class type>
  type Matrix_T<type>::TotalDevice()
  {
    _Warning("Matrix_T::TotalRevised","TotalDevice will be depreciated. Please use pond::Matrix_T::TotalRevised instead.");
    return TotalRevised();
  }
  

  template<class type> __cond_host_device__ type&MatrixPart(type*a,int*d,int p1){return a[ p1 ];};
  template<class type> __cond_host_device__ type&MatrixPart(type*a,int*d,int p1,int p2){return a[ p1*d[2]+p2 ];};
  template<class type> __cond_host_device__ type&MatrixPart(type*a,int*d,int p1,int p2,int p3){return a[(p1*d[2]+p2)*d[3]+p3 ];};
  template<class type> __cond_host_device__ type&MatrixPart(type*a,int*d,int p1,int p2,int p3,int p4){return a[ ((p1*d[2]+p2)*d[3]+p3)*d[4]+p4 ];};
  template<class type> __cond_host_device__ type&MatrixPart(type*a,int*d,int p1,int p2,int p3,int p4,int p5){return a[(((p1*d[2]+p2)*d[3]+p3)*d[4]+p4)*d[5]+p5 ];};
  template<class type> __cond_host_device__ type&MatrixPart(type*a,int*d,int p1,int p2,int p3,int p4,int p5,int p6){return a[((((p1*d[2]+p2)*d[3]+p3)*d[4]+p4)*d[5]+p5)*d[6]+p6];};
  template<class type> __cond_host_device__ type&MatrixPart(type*a,int*d,int p1,int p2,int p3,int p4,int p5,int p6,int p7){return a[(((((p1*d[2]+p2)*d[3]+p3)*d[4]+p4)*d[5]+p5)*d[6]+p6)*d[7]+p7];};
  template<class type> __cond_host_device__ type&MatrixPart(type*a,int*d,int p1,int p2,int p3,int p4,int p5,int p6,int p7,int p8){return a[((((((p1*d[2]+p2)*d[3]+p3)*d[4]+p4)*d[5]+p5)*d[6]+p6)*d[7]+p7)*d[8]+p8];};
  template<class type> __cond_host_device__ type&MatrixPart(type*a,int*d,int p1,int p2,int p3,int p4,int p5,int p6,int p7,int p8,int p9){return a[(((((((p1*d[2]+p2)*d[3]+p3)*d[4]+p4)*d[5]+p5)*d[6]+p6)*d[7]+p7)*d[8]+p8)*d[9]+p9];};


#define POND_Matrix_Arith(name,func)                                    \
  template<class type,class type1,class type2>                          \
  Matrix_T<type> &name(Matrix_T<type>&arrout,Matrix_T<type1> &data1,Matrix_T<type2> &data2){ \
    if (not (data1.SameDimensionQ(data2) ))                             \
      {_Erroring("Matrix::"#name,"Try to "#func" two Matrixs of different size."); return arrout; } \
    if ( !(arrout.SameDimensionQ(data1)) )                              \
      arrout.Init(data1, pond::GetDataPosition() ) ;                     \
    if ( pond::IsDataOnHost()  )                                        \
      func##_host( arrout.Data, 		data1.Data, 	data2.Data, 	data1.Size() ); \
    else if ( pond::IsDataOnDevice())                                   \
      func##_device(arrout.DataDevice,	data1.DataDevice,	data2.DataDevice,	data1.Size() ); \
    if ( data1.state == MatrixStateTempOccupied )                       \
      data1.state = MatrixStateTempFree;                                \
    if ( data2.state == MatrixStateTempOccupied )                       \
      data2.state = MatrixStateTempFree;                                \
    if ( arrout.state == MatrixStateTempFree )                          \
      arrout.state = MatrixStateTempOccupied;                           \
    return arrout;                                                      \
  };
#define POND_Matrix_Arith_Value(name,func)                              \
  template<class type,class type1,class type2>                          \
  Matrix_T<type> &name(Matrix_T<type> &arrout,Matrix_T<type1> &data1,type2 num){ \
    if ( not (arrout.SameDimensionQ(data1) ))                           \
      arrout.Init(data1, pond::GetDataPosition() ) ;                    \
    if ( pond::IsDataOnHost()  )                                        \
      func##_host( arrout.Data, 		data1.Data, 	num, 	data1.Size() ); \
    else if ( pond::IsDataOnDevice())                                   \
      func##_device(arrout.DataDevice,	data1.DataDevice,	num,	data1.Size() ); \
    if ( data1.state == MatrixStateTempOccupied )                       \
      data1.state = MatrixStateTempFree;                                \
    if ( arrout.state == MatrixStateTempFree )                          \
      arrout.state = MatrixStateTempOccupied;                           \
    return arrout;                                                      \
  }; 

  POND_Matrix_Arith(MatrixAdd,add)
  POND_Matrix_Arith(MatrixPlus,add)
  POND_Matrix_Arith(MatrixMinus,minus)
  POND_Matrix_Arith(MatrixTimes,multi)
  POND_Matrix_Arith(MatrixDivide,divi)

  POND_Matrix_Arith_Value(MatrixAdd,add)
  POND_Matrix_Arith_Value(MatrixPlus,add)
  POND_Matrix_Arith_Value(MatrixMinus,minus)
  POND_Matrix_Arith_Value(MatrixTimes,multi)
  POND_Matrix_Arith_Value(MatrixDivide,divi)


#undef POND_Matrix_Arith
#undef POND_Matrix_Arith_Value



  
};

#endif
