#ifndef BASIC_POND_H
#define BASIC_POND_H

namespace pond{

  ////////////////////////////////////////////
  typedef       unsigned int   u_int;
  typedef       unsigned long  u_long;
  typedef       unsigned char  u_char;
  typedef       unsigned short u_short;
  class         EvaSettings;
  ////////////////////////////////////////////
  struct        complex;
  struct        floatcomplex;
  struct        Index;
  enum struct   ObjectType:unsigned char;
  class         Object;
  ////////////////////////////////////////////
  class         Evaluation;
  class         Module;
  typedef       int (Module::*MemberFunction)(Object &);
  typedef       int (*StaticFunction)(Object &);
  ////////////////////////////////////////////
  template<class type> class Matrix_T;
  typedef       Matrix_T<double> Matrix;
  typedef       Matrix_T<complex> ComplexMatrix;
  typedef       Matrix_T<float> FloatMatrix;
  typedef       Matrix_T<floatcomplex> FloatComplexMatrix;
  typedef       Matrix_T<int> IntMatrix;
  ////////////////////////////////////////////
  class         MatrixModule;
  class         TensorModule;
  class         MatlabEngineModule;
  class         PythonModule;
  class         AlgebraicModule;
  class 				ClassModule;
  class         MPIModule;
  ////////////////////////////////////////////
  void swap(short unsigned int&t1,short unsigned int&t2);
  void swap(int&t1,int&t2);
  void swap(u_int&t1,u_int&t2);
  void swap(char*&t1,char*&t2);
  void swap(double&t1,double&t2);
  void swap(float&t1,float&t2);
  void swap(long&t1,long&t2);
  void swap(u_long&t1,u_long&t2);
  ////////////////////////////////////////////
  template<class type> type abs(type x){ return x>=0?x:-x; }
  ////////////////////////////////////////////
  void sleep_micro(int sec);
  void sleep_sec(int sec);
  ////////////////////////////////////////////
  enum struct AttributeType: unsigned char{Null,
      Flat,                // f(a,f(b,c) )           ->      f(a,b,c)
      Listable,            // f([a,b,c] )            ->      [ f(a), f(b), f(c) ]
      OneIdentity,         // MatchQ( f(a), a)       ->       true
      HoldFirst,           // f(1+2,3+4)             ->       f(1+2, 7)
      HoldRest,            // f(1+2,3+4,5+6)         ->       f(3,3+4,5+6)
      HoldAll,             // f(1+2,3+4,5+6)         ->       unchanged
      HoldAllCompleted,    // f(__,Evaluate(1+2),__) ->       unchanged
      SequenceHold,        // f(__,Sequence(__),__)  ->       unchanged
      Orderless,           // f(a,b,a)               ->       f(a,a,b)
      /* GrammarUnit,         // f(a,b,c)              a newContext is auto attached and removed */
      Constant,            // not changable
      DelayFunction,       // f(a,b,c)(d)           f will find its c++ defination in this form   
      DelayHold,           // f(a,b,c)(d)           if f has DelayHold, d will be hold
      DelaySetable,        // f(...)(...) = expr    whole expression will send to f

      Conjunctable,        // f(...).g()            whole expression will send to f
      Setable,             // f(...) = expr         whole expression will send to f 
      Partable,            // f(...)[...]           whole expression will send to f
      /* Destroyable, */
      Protected,           // not able to change defination in valuetable
      Locked,
      MAXENUM,
      };
  ////////////////////////////////////////////
  unsigned int   SimpleHashCodeOfString(const char *);
  const char    *Type2String(ObjectType);
  const char    *AttributeType2String(AttributeType);
  AttributeType  String2AttributeType(const char*);
  bool           AttributeQ(bool*,AttributeType);
  bool           AttributeQ(bool*,const char*);
  void           SetAttribute(bool*,AttributeType,bool val=true);
  void           SetAttribute(bool*,const char*,bool val=true);
  bool           SpecialCharecterQ(char);
  ////////////////////////////////////////////
  ////////////////////////////////////////////
  std::string    ToString(const int          num);
  std::string    ToString(const u_int        num);
  std::string    ToString(const u_long       num);
  std::string    ToString(const Index        num);
  std::string    ToString(const float        num);
  std::string    ToString(const double       num);
  std::string    ToString(const complex      num);
  std::string    ToString(const floatcomplex num);
  std::string    ToString(const void        *num);
  std::string    ToString(const ObjectType   type);
  std::string    ToString(const char        *val);
  template<class type>
  std::string    ToString(const type val){
    std::ostringstream ss;
    ss.str("");
    ss<<val;
    return ss.str();
  }
  ///////////////////////////////////////////////
  double         ToNumber(const char*);
  ///////////////////////////////////////////////
  ///////////////////////////////////////////////compare
  int            compare(double num1,double num2);
  int            compare(std::string ,std::string );
  int            compare(const char*,const char*);
  ///////////////////////////////////////////////
  ///////////////////////////////////////////////
  ////////////////////// Base on operation system
  bool           FileExistQ(std::string filename);
  bool           CheckFileType(std::string dirname,std::string type);//DIR  REG  LINK  FIFO
  bool           CommandExist(std::string cmd);
  int            SetFileContent(std::string filename,std::string content);
  std::string    GetFileContent(std::string filename);
  int            RemoveFile(std::string filename);
  std::string    IthWord(std::string str,int n);
  int 	         WordCount(std::string str);
  int 	         StringSplit(std::string str,std::vector<std::string>&strs,std::string split);
  int 	         StringSplit(std::string str,std::vector<std::string>&strs);
  std::string    RandomString(int len);
  std::string    RandomNumberString(int len);
  std::string    RemoveFirstWord(std::string str,std::string &leftword);
  std::string    GetEnv(std::string name, std::string default_value="");
  int            SetEnv(std::string var,std::string expr,bool overwrite=false);
  std::string    GetDirectory();
  std::string    GetBaseName(std::string path);
  std::string    GetDirName(std::string path);
  /* std::string    TestCommand(std::string cmd); */
  inline
    std::string  GetCwd(){return pond::GetDirectory(); };
  bool           SetDirectory(std::string dirname);
  std::string    GetParentDirectory();
  bool           SetToParentDirectory();
  std::string    System(std::string cmd);
  int            System(std::string cmd, std::vector<std::string>&result);
  int            Execute(std::string cmd);
  int            Showing(std::string str);
  std::string    GetFileMD5(std::string filename);
  constexpr unsigned
  int            RSimpleHashCodeOfString(const char *s,unsigned int code = 5381, int off = 0){return (((s[off]=='\0') ? code : RSimpleHashCodeOfString(s,(code*33)^s[off],off+1)));}
  //#define KCase(key) case RSimpleHashCodeOfString(#key)
  /* long           GetMemInUse(); */
  bool           GPUDeviceQ();
  ////////////////

  ////////////////
  inline double  Rand(){ return (double)rand()/RAND_MAX; }
  double         GaussRand(double mean = 0.0, double variance = 1.0);
  ////////////////
  int            specialCharReplace(std::string&str, const std::string ori, const std::string rep);
  int            PondInnerStringRestoreNormal(std::string &str);
};

#endif
