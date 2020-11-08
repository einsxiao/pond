#ifndef evamath_pond_h
#define evamath_pond_h

namespace pond{

  ///////////////////////////////////////////
#define pow2(x) ((x)*(x))
#define pow3(x) ((x)*(x)*(x))
#define pow4(x) ((x)*(x)*(x)*(x))
#define pow5(x) ((x)*(x)*(x)*(x)*(x))
#define pow6(x) ((x)*(x)*(x)*(x)*(x)*(x))
#define pow7(x) ((x)*(x)*(x)*(x)*(x)*(x)*(x))
#define pow8(x) ((x)*(x)*(x)*(x)*(x)*(x)*(x)*(x))
#define sign(num) ((num)>0?1:((num)<0?-1:0)); 

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
      float mo=pow2(v.re)+pow2(v.im);
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
      float mo=pow2(c2.re)+pow2(c2.im);
      return floatcomplex((c1.re*c2.re+c1.im*c2.im)/mo,(c1.im*c2.re-c1.re*c2.im)/mo);
    }
    __cond_host_device__  inline  friend floatcomplex operator/(const floatcomplex c1,const float c2){
      return floatcomplex(c1.re/c2,c1.im/c2);
    }
    __cond_host_device__  inline  friend floatcomplex operator/(const float c1,const floatcomplex c2){
      float mo=pow2(c2.re)+pow2(c2.im);
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
      double mo=pow2(v.re)+pow2(v.im);
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
      double mo=pow2(c2.re)+pow2(c2.im);
      return complex((c1.re*c2.re+c1.im*c2.im)/mo,(c1.im*c2.re-c1.re*c2.im)/mo);
    }
    __cond_host_device__  inline  friend complex operator/(const complex c1,const double c2){
      return complex(c1.re/c2,c1.im/c2);
    }
    __cond_host_device__  inline  friend complex operator/(const double c1,const complex c2){
      double mo=pow2(c2.re)+pow2(c2.im);
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

  floatcomplex::floatcomplex( complex in)
  {
    re = in.re;
    im = in.im;
  }

  struct Index{
    u_int i,j;
    ////////////////////////////////////
    Index() = default;
    ~Index() = default;
    explicit Index(const double value){
      *(reinterpret_cast<double*>(this)) = value;
    }
    Index(const u_int f,const u_int s){
      i = f;  j = s;
    }
    Index&operator=(const Index&ind)=default;
    inline bool zeroQ()const{  return *(reinterpret_cast<const double*>(this)) == 0; }
    inline bool nonzeroQ()const{  return *(reinterpret_cast<const double*>(this)) != 0; }
    inline void zero(){  *(reinterpret_cast<double*>(this))=0; }
    /* inline void set(double val){ *(reinterpret_cast<double*>(this)) = val; }; */
    inline void set(u_int f,u_int s){ i=f; j=s;};
#define _deal(oper)                                                     \
    inline friend bool operator oper(const Index&ind1,const Index&ind2){ \
      return *reinterpret_cast<const double*>(&ind1) oper *reinterpret_cast<const double*>(&ind2); \
    }
    _deal(  == );
    _deal(  != );
#undef _deal
    /* inline friend bool operator<(const Index ind1,const Index ind2){  */
    /*   return  ind1.i < ind2.i  or ind1.i == ind2.j and ind1.j < ind2.j ; */
    /* }; */
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

};


#endif


