#ifndef EvaError_POND_H
#define EvaError_POND_H

namespace pond{
  class Error:public std::exception{
  public:
    std::string module,function,message;
    Error(){}
    Error(std::string mes){ message =mes ; }
    Error(std::string func,std::string mes){ function=func; message = mes; }
    Error(std::string mod, std::string func,std::string mes){ module = mod; function = func ;message = mes; }
    virtual ~Error()throw(){};
    virtual const char *what()const throw();
    virtual const std::string swhat()const throw();
    void trace()const;
    inline friend std::ofstream &operator<<(std::ofstream &os, Error e){ os<<e.what()<<std::endl; return os;}
    inline friend std::ostream &operator<<(std::ostream &os, Error e){ os<<e.what()<<std::endl; return os;}
    static std::string decode_undefined_sybmol_string(char*message);
  };
  class ExceptionQuit:public Error{
  public:
    int code;
    ExceptionQuit(){ code = 0; }
    ExceptionQuit(int c){ code = c; }
  };


  class DebugOutput{
  public:

    template<class type>
      DebugOutput&operator<<(type val){
#ifdef DEBUG
      std::cerr<<val;
#endif
      return *this;
    }

    typedef DebugOutput& (*manipulator)(DebugOutput&);

    DebugOutput&operator<<(manipulator manip){
      return manip(*this);
    }

    static DebugOutput&endl(DebugOutput&out){
#ifdef DEBUG
      std::cerr<<std::endl;
#endif
      return out;
    }

    // this is the type of std::cout
    typedef std::basic_ostream<char, std::char_traits<char> > CoutType;
    // this is the function signature of std::endl
    typedef CoutType& (*StandardEndLine)(CoutType&);

    // define an operator<< to take in std::endl
    DebugOutput& operator<<(StandardEndLine manip)
      {
        // call the function, but we cannot return it's value
#ifdef DEBUG
        manip(std::cerr);
#endif
        return *this;
      }
  };
  class __DebugOutput{public: static DebugOutput out;};


#define Warning(cat,mes) ({                                   \
      std::cerr<< std::string(cat) << ":Warning(" <<          \
        std::string(__FILE__) << ":" << __LINE__ << "): "  << \
        std::string(mes) << "\n";                             \
      true;                                                   \
    })

#define Erroring(cat,mes) ({                                  \
      std::cerr << std::string(cat) << ":Error(" <<           \
        std::string(__FILE__) << ":" << __LINE__ <<  "): " << \
        std::string(mes) << "\n";                             \
      true;                                                   \
    })

  // multi language support

#define zhWarning(cat, mes)                                 \
  ( ( EvaKernel->lang ==  "zh" )?                           \
    ({                                                      \
      std::cerr<< std::string(cat) << ":警告(" <<           \
        std::string(__FILE__) << ":" << __LINE__<<  "): "<< \
        std::string(mes) << "\n";                           \
      true;                                                 \
    })                                                      \
    :                                                       \
    false                                                   \
    )

#define zhErroring(cat, mes)                                \
  ( ( EvaKernel->lang ==  "zh" )?                           \
    ({                                                      \
      std::cerr<< std::string(cat) << ":错误(" <<           \
        std::string(__FILE__) << ":" << __LINE__<<  "): "<< \
        std::string(mes) << "\n";                           \
      true;                                                 \
    })                                                      \
    :                                                       \
    false                                                   \
    )


};


#endif
