#ifndef evamath_pond_h
#define evamath_pond_h

namespace pond{

  ///////////////////////////////////////////
  __cond_host_device__ inline double _pow2(double x){ return x*x; }
  __cond_host_device__ inline double _pow3(double x){ return x*x*x; }
  __cond_host_device__ inline double _pow4(double x){double pow2=_pow2(x); return pow2*pow2; }
  __cond_host_device__ inline double _pow5(double x){return _pow4(x)*x;}
  __cond_host_device__ inline double _pow6(double x){return _pow5(x)*x; }
  __cond_host_device__ inline double _pow7(double x){return _pow6(x)*x; }
  __cond_host_device__ inline double _pow8(double x){double pow4 = _pow4(x); return pow4*pow4; }
  __cond_host_device__ inline double _pow9(double x){return _pow8(x)*x; }
  __cond_host_device__ inline double _pow10(double x){return _pow8(x)*_pow2(x); }
  __cond_host_device__ inline double _pow11(double x){return _pow8(x)*_pow3(x); }
  __cond_host_device__ inline double _pow12(double x){double pow4 = _pow4(x); return pow4*pow4*pow4; }
  __cond_host_device__ inline double _pow13(double x){return _pow12(x)*x; }
  __cond_host_device__ inline double _pow14(double x){return _pow12(x)*_pow2(x); }
  __cond_host_device__ inline double _pow15(double x){return _pow12(x)*_pow3(x); }
  __cond_host_device__ inline double _pow16(double x){double pow8 = _pow8(x); return pow8*pow8; }
  constexpr double _powInt(double x,int n){
    return ( n==1 )?x:x*_powInt(x,n-1);
  }

  __cond_host_device__ inline double _pow(double x, double y){
#ifdef __CUDA_ARCH__
    return powf(x,y);
#else
    return pow(x,y);
#endif
  }
  __cond_host_device__ inline float _pow(float x, float y){
#ifdef __CUDA_ARCH__
    return powf(x,y);
#else
    return pow(x,y);
#endif
  }

  struct floatcomplex{
  public:
    float re,im;
                          inline floatcomplex()=default;
                          inline ~floatcomplex()=default;
    __cond_host_device__  inline floatcomplex(float in){
      re = in;
      im=0;
    };
    __cond_host_device__  inline floatcomplex(float in1,float in2){
      re = in1;
      im=in2;
    };
    ////////////////////////////////
    __cond_host_device__  explicit inline floatcomplex(float in[2]){
      re=in[0];
      im=in[1];
    };
    __cond_host_device__  explicit inline floatcomplex(double in[2]){
      re=in[0];
      im=in[1];
    };
    __cond_host_device__  explicit inline floatcomplex(complex in);
    __cond_host_device__  explicit inline operator int()const{
      return (int)re;
    };
    __cond_host_device__  explicit inline operator float()const{
      return re;
    };
    __cond_host_device__  explicit inline operator double()const{
      return re;
    };

    ////////////////////////////////
    __cond_host_device__  inline  int compare(floatcomplex other)const{
      if (re<other.re){
        return -1;
      } else{
        if( re==other.re){
          if ( im == other.im )
            return 0;
          else
            if ( im<other.im) return -1; else return 1;
        }else{
          return 1;
        }
      }
    }
    __cond_host_device__ inline bool realq()const{
      return im == 0 ;
    }
    // overload = and - (reverse number) and += -= *= /=
                          inline  floatcomplex &operator=(const floatcomplex&v)=default;
    __cond_host_device__  inline  floatcomplex &operator=(const float v){
      re=v; im=0;
      return *this;
    }
    __cond_host_device__  inline  floatcomplex operator-(){
      return floatcomplex(-re,-im);
    }
    __cond_host_device__  inline  floatcomplex &operator+=(const floatcomplex v){
      re+=v.re;im+=v.im;
      return *this;
    }
    __cond_host_device__  inline  floatcomplex &operator+=(const float v){
      re+=v;
      return *this;
    }
    __cond_host_device__  inline  floatcomplex &operator-=(const floatcomplex v){
      re-=v.re;im-=v.im;
      return *this;
    }
    __cond_host_device__  inline  floatcomplex &operator-=(const float v){
      re-=v;
      return *this;
    }
    __cond_host_device__  inline  floatcomplex &operator*=(const floatcomplex v) {
      float tre = re;
      re =re*v.re-im*v.im;
      im=tre*v.im+im*v.re;
      return *this;
    }
    __cond_host_device__  inline  floatcomplex &operator*=(const float v) {
      re*=v;
      im*=v;
      return *this;
    }
    __cond_host_device__  inline  floatcomplex &operator/=(const floatcomplex v){
      float mo=_pow2(v.re)+_pow2(v.im);
      float tre = re;
      re=(re*v.re+im*v.im)/mo;
      im=(im*v.re-tre*v.im)/mo;
      return *this;
    }
    __cond_host_device__  inline  floatcomplex &operator/=(const float v){
      re/=v;
      im/=v;
      return *this;
    }
    // overload + - * /
    __cond_host_device__  inline  friend floatcomplex operator+(const floatcomplex c1,const floatcomplex c2){
      return floatcomplex(c1.re+c2.re,c1.im+c2.im);
    }
    __cond_host_device__  inline  friend floatcomplex operator+(const floatcomplex c1,const float c2){
      return floatcomplex(c1.re+c2,c1.im);
    }
    __cond_host_device__  inline  friend floatcomplex operator+(const float c2,const floatcomplex c1){
      return floatcomplex(c1.re+c2,c1.im);
    }

    __cond_host_device__  inline  friend floatcomplex operator-(const floatcomplex c1,const floatcomplex c2){
      return floatcomplex(c1.re-c2.re,c1.im-c2.im);
    }
    __cond_host_device__  inline  friend floatcomplex operator-(const floatcomplex c1,const float c2){
      return floatcomplex(c1.re-c2,c1.im);
    }
    __cond_host_device__  inline  friend floatcomplex operator-(const float c2, const floatcomplex c1){
      return floatcomplex(c1.re-c2,c1.im);
    }

    __cond_host_device__  inline  friend floatcomplex operator*(const floatcomplex c1,const floatcomplex c2){
      return floatcomplex(c1.re*c2.re-c1.im*c2.im,c1.re*c2.im+c1.im*c2.re);
    }
    __cond_host_device__  inline  friend floatcomplex operator*(const floatcomplex c1,const float c2){
      return floatcomplex(c1.re*c2,c1.im*c2);
    }
    __cond_host_device__  inline  friend floatcomplex operator*(const float c2,const floatcomplex c1){
      return floatcomplex(c1.re*c2,c1.im*c2);
    }

    __cond_host_device__  inline  friend floatcomplex operator/(const floatcomplex c1,const floatcomplex c2){
      float mo=_pow2(c2.re)+_pow2(c2.im);
      return floatcomplex((c1.re*c2.re+c1.im*c2.im)/mo,(c1.im*c2.re-c1.re*c2.im)/mo);
    }
    __cond_host_device__  inline  friend floatcomplex operator/(const floatcomplex c1,const float c2){
      return floatcomplex(c1.re/c2,c1.im/c2);
    }
    __cond_host_device__  inline  friend floatcomplex operator/(const float c1,const floatcomplex c2){
      float mo=_pow2(c2.re)+_pow2(c2.im);
      return floatcomplex( (c1*c2.re)/mo,(-c1*c2.im)/mo);
    }
    // overload ^
    // overload != ==
    __cond_host_device__  inline  friend int operator!=(const floatcomplex c1,const floatcomplex c2){
      if (c1.re!=c2.re||c1.im!=c2.im)
        return 1;
      else return 0;}
    __cond_host_device__  inline  friend int operator==(const floatcomplex c1,const floatcomplex c2){
      if (c1.re!=c2.re||c1.im!=c2.im)
        return 0;
      else
        return 1;
    }

    inline friend std::ofstream& operator<<(std::ofstream &os,const floatcomplex c){
      os<<pond::ToString(c);
      return os;
    }

    inline friend std::ostream& operator<<(std::ostream &os,const floatcomplex c){
      os<<pond::ToString(c);
      return os;
    }

    inline friend std::ifstream& operator>>(std::ifstream &is,floatcomplex c){
      char ctemp;
      is>>ctemp>>c.re>>ctemp>>c.im>>ctemp;
      return is;
    }

    inline friend std::istream& operator>>(std::istream &is,floatcomplex c){
      char ctemp;
      is>>ctemp>>c.re>>ctemp>>c.im>>ctemp;
      return is;
    }

  };

  struct complex{
  public:
    double re,im;
                          inline complex()=default;
                          inline ~complex()=default;
    __cond_host_device__  inline complex(double in){
      re = in;
      im=0;
    };
    __cond_host_device__  inline complex(double in1,double in2){
      re = in1;
      im=in2;
    };
    __cond_host_device__  inline complex(floatcomplex in){
      re = in.re;
      im = in.im;
    }
    ////////////////////////////////
    __cond_host_device__  explicit inline complex(double in[2]){
      re=in[0];
      im=in[1];
    };
    __cond_host_device__  explicit inline operator int()const{
      return (int)re;
    };
    __cond_host_device__  explicit inline operator float()const{
      return re;
    };
    __cond_host_device__  explicit inline operator double()const{
      return re;
    };

    ////////////////////////////////
    __cond_host_device__  inline  int compare(complex other)const{
      if (re<other.re){
        return -1;
      } else{
        if( re==other.re){
          if ( im == other.im )
            return 0;
          else
            if ( im<other.im) return -1; else return 1;
        }else{
          return 1;
        }
      }
    }
    __cond_host_device__ inline bool realq()const{
      return im == 0 ;
    }
    // overload = and - (reverse number) and += -= *= /=
                          inline  complex &operator=(const complex&v)=default;
    __cond_host_device__  inline  complex &operator=(const double v){
      re=v; im=0;
      return *this;
    }
    __cond_host_device__  inline  complex operator-(){
      return complex(-re,-im);
    }
    __cond_host_device__  inline  complex &operator+=(const complex v){
      re+=v.re;im+=v.im;
      return *this;
    }
    __cond_host_device__  inline  complex &operator+=(const double v){
      re+=v;
      return *this;
    }
    __cond_host_device__  inline  complex &operator-=(const complex v){
      re-=v.re;im-=v.im;
      return *this;
    }
    __cond_host_device__  inline  complex &operator-=(const double v){
      re-=v;
      return *this;
    }
    __cond_host_device__  inline  complex &operator*=(const complex v) {
      double tre = re;
      re =re*v.re-im*v.im;
      im=tre*v.im+im*v.re;
      return *this;
    }
    __cond_host_device__  inline  complex &operator*=(const double v) {
      re*=v;
      im*=v;
      return *this;
    }
    __cond_host_device__  inline  complex &operator/=(const complex v){
      double mo=_pow2(v.re)+_pow2(v.im);
      double tre = re;
      re=(re*v.re+im*v.im)/mo;
      im=(im*v.re-tre*v.im)/mo;
      return *this;
    }
    __cond_host_device__  inline  complex &operator/=(const double v){
      re/=v;
      im/=v;
      return *this;
    }
    // overload + - * /
    __cond_host_device__  inline  friend complex operator+(const complex c1,const complex c2){
      return complex(c1.re+c2.re,c1.im+c2.im);
    }
    __cond_host_device__  inline  friend complex operator+(const complex c1,const double c2){
      return complex(c1.re+c2,c1.im);
    }
    __cond_host_device__  inline  friend complex operator+(const double c2,const complex c1){
      return complex(c1.re+c2,c1.im);
    }

    __cond_host_device__  inline  friend complex operator-(const complex c1,const complex c2){
      return complex(c1.re-c2.re,c1.im-c2.im);
    }
    __cond_host_device__  inline  friend complex operator-(const complex c1,const double c2){
      return complex(c1.re-c2,c1.im);
    }
    __cond_host_device__  inline  friend complex operator-(const double c2, const complex c1){
      return complex(c1.re-c2,c1.im);
    }

    __cond_host_device__  inline  friend complex operator*(const complex c1,const complex c2){
      return complex(c1.re*c2.re-c1.im*c2.im,c1.re*c2.im+c1.im*c2.re);
    }
    __cond_host_device__  inline  friend complex operator*(const complex c1,const double c2){
      return complex(c1.re*c2,c1.im*c2);
    }
    __cond_host_device__  inline  friend complex operator*(const double c2,const complex c1){
      return complex(c1.re*c2,c1.im*c2);
    }

    __cond_host_device__  inline  friend complex operator/(const complex c1,const complex c2){
      double mo=_pow2(c2.re)+_pow2(c2.im);
      return complex((c1.re*c2.re+c1.im*c2.im)/mo,(c1.im*c2.re-c1.re*c2.im)/mo);
    }
    __cond_host_device__  inline  friend complex operator/(const complex c1,const double c2){
      return complex(c1.re/c2,c1.im/c2);
    }
    __cond_host_device__  inline  friend complex operator/(const double c1,const complex c2){
      double mo=_pow2(c2.re)+_pow2(c2.im);
      return complex( (c1*c2.re)/mo,(-c1*c2.im)/mo);
    }
    // overload ^
    // overload != ==
    __cond_host_device__  inline  friend int operator!=(const complex c1,const complex c2){
      if (c1.re!=c2.re||c1.im!=c2.im)
        return 1;
      else return 0;}
    __cond_host_device__  inline  friend int operator==(const complex c1,const complex c2){
      if (c1.re!=c2.re||c1.im!=c2.im)
        return 0;
      else
        return 1;
    }

    inline friend std::ofstream& operator<<(std::ofstream &os,const complex c){
      os<<pond::ToString(c);
      return os;
    }

    inline friend std::ostream& operator<<(std::ostream &os,const complex c){
      os<<pond::ToString(c);
      return os;
    }

    inline friend std::ifstream& operator>>(std::ifstream &is,complex c){
      char ctemp;
      is>>ctemp>>c.re>>ctemp>>c.im>>ctemp;
      return is;
    }

    inline friend std::istream& operator>>(std::istream &is,complex c){
      char ctemp;
      is>>ctemp>>c.re>>ctemp>>c.im>>ctemp;
      return is;
    }

  };

  __cond_host_device__ floatcomplex::floatcomplex( complex in)
  {
    re = in.re;
    im = in.im;
  }

  struct Index{
    u_int row, col;
    ////////////////////////////////////
    Index() = default;
    ~Index() = default;
    explicit Index(const double value){
      *(reinterpret_cast<double*>(this)) = value;
    }
    Index(const u_int r,const u_int c){
      row = r;  col = c;
    }
    Index&operator=(const Index&ind)=default;
    inline bool zeroQ()const{  return *(reinterpret_cast<const double*>(this)) == 0; }
    inline bool nonzeroQ()const{  return *(reinterpret_cast<const double*>(this)) != 0; }
    inline void zero(){  *(reinterpret_cast<double*>(this))=0; }
    /* inline void set(double val){ *(reinterpret_cast<double*>(this)) = val; }; */
    inline void set(u_int r,u_int c){ row=r; col=c;};
#define _deal(oper)                                                     \
    inline friend bool operator oper(const Index&ind1,const Index&ind2){ \
      return *reinterpret_cast<const double*>(&ind1) oper *reinterpret_cast<const double*>(&ind2); \
    }
    _deal(  == );
    _deal(  != );
#undef _deal
    inline friend std::ostream& operator<<(std::ostream &os,const Index c){
      os<<pond::ToString(c);
      return os;
    }
  };

  class Math{
  public:
    static bool          NumberQ(double);
    static bool          IntegerQ(double);
    static bool          FiniteQ(double);
    static bool          InfQ(double);
    static bool          NanQ(double);
    static bool          NumberQ(std::string);
    static std::string   OrderForm(int id);
  };

  // the inverse of a matrix
  int GaussCMInverse(double *AA,double *B,int n);
  int GaussCMInverse(double *AA,double *A, double *B,int n);

};


#endif


