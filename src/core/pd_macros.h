#ifndef EVA_MACROS_POND_H
#define EVA_MACROS_POND_H

#define EMSCRIPTEN_KEEPALIVE


#define InitVar(variable,default_value) if ( not (EvaKernel->GetValue(#variable,variable)) ) { variable = default_value; }
#define InitVar2(variable,default_value) if ( not (EvaKernel->GetValue(#variable,variable)) ) { variable = default_value; }
#define InitVar3(pd_variable_name,variable,default_value) if ( not (EvaKernel->GetValue(pd_variable_name,variable)) ) { variable = default_value; }

#define InitNameVariable(name,variable,default_value)({ \
      if ( not (EvaKernel->GetValue(name,variable)) ) { \
        variable = default_value;                       \
      }                                                 \
    })
#define InitNameVariablePrint(name,variable,default_value)({            \
      if ( not (EvaKernel->GetValue(name,variable)) ) {                 \
        variable = default_value;                                       \
        cout<<name<<" is set to default value "<< variable<<std::endl;  \
      }else{                                                            \
        cout<<name<<" is set to "<< variable<<std::endl;                \
      }                                                                 \
    })

#define InitVariable(variable,default_value) InitNameVariable(#variable,variable,default_value)
#define InitVariablePrint(variable,default_value) InitNameVariablePrint(#variable,variable,default_value); 


#define InitNameArray(name,array) ({                    \
      int err=0;                                        \
      Object tempObj;                                   \
      tempObj.SetSymbol(name);                          \
      EvaKernel->Evaluate(tempObj);                     \
      if(not tempObj.ListQ() or tempObj.Size() == 0 ){  \
        err=-1;                                         \
      }else{                                            \
        for (u_int i=0;i<tempObj.Size();i++){           \
          if ( not tempObj[i+1].NumberQ() ){            \
            err = -1;                                   \
            break;                                      \
          }else{                                        \
            array[i] = tempObj[i+1].Number();           \
          }                                             \
        }                                               \
      }                                                 \
      err;                                              \
    })

#define InitNameArrayPrint(name,array) ({               \
      int err=0;                                        \
      Object tempObj;                                   \
      tempObj.SetSymbol(name);                          \
      EvaKernel->Evaluate(tempObj);                     \
      if(not tempObj.ListQ() or tempObj.Size() == 0 ){  \
        err=-1;                                         \
      }else{                                            \
        cout<< name <<" is set to {";                   \
        for (u_int i=0;i<tempObj.Size();i++){           \
          if ( not tempObj[i+1].NumberQ() ){            \
            err = -1;                                   \
            break;                                      \
          }else{                                        \
            array[i] = tempObj[i+1].Number();           \
            if ( i > 0 ) cout<<",";                     \
            cout<<array[i];                             \
          }                                             \
        }                                               \
        cout<<"}"<<std::endl;                           \
      }                                                 \
      err;                                              \
    })

#define InitArray(array) ({InitNameArray(#array,array);})
#define InitArrayPrint(array) ({InitNameArrayPrint(#array,array);})

#define InitNameMatrix(name,matrix) ({int err=0;      \
      Object tempObj;                                 \
      tempObj.SetSymbol(name);                        \
      EvaKernel->Evaluate(tempObj);                   \
      if( not MatrixModule::MatrixQ(tempObj) ){       \
        err=-1;                                       \
      }else{                                          \
        MatrixModule::Object2Matrix(tempObj,matrix);  \
      }                                               \
      err;                                            \
    })
#define InitNameMatrixPrint(name,matrix) ({int err=0;                 \
      Object tempObj;                                                 \
      tempObj.SetSymbol(name);                                        \
      EvaKernel->Evaluate(tempObj);                                   \
      if( not MatrixModule::MatrixQ(tempObj) ){                       \
        err=-1;                                                       \
      }else{                                                          \
        MatrixModule::Object2Matrix(tempObj, matrix);                 \
      }                                                               \
      if( err >= 0 ){                                                 \
        cout<< name <<" is set to "<<tempObj.ToString()<<std::endl;;  \
      }                                                               \
      err;                                                            \
    })
#define InitMatrix(matrix) InitNameMatrix(#matrix, matrix)
#define InitMatrixPrint(matrix) InitNameMatrixPrint(#matrix, matrix)


#define InitNameObject(name,obj) ({int err=0;   \
      obj.SetSymbol(name);                      \
      EvaKernel->Evaluate(obj);                 \
      if ( obj.SymbolQ( name ) )                \
        err = -1;                               \
      err;                                      \
    })


#define InitNameObjectPrint(name,obj) ({int err=0;                \
      obj.SetSymbol(name);                                        \
      EvaKernel->Evaluate(obj);                                   \
      if ( obj.SymbolQ( name ) )                                  \
        err = -1;                                                 \
      if( err >= 0 ){                                             \
        cout<< name <<" is set to "<<obj.ToString()<<std::endl;;  \
      }                                                           \
      err;                                                        \
    })

#define InitObject(obj) InitNameObject(#obj, obj)
#define InitObjectPrint(obj) InitNameObjectPrint(#obj, obj)

#if defined(__CUDACC__)
#  define __cond_host_device__ __host__ __device__ 
#  define __cond_host__ __host__ 
#  define __cond_device__ __device__ 
#  define __cond_cuda__ 
#else
#  define __cond_host_device__ 
#  define __cond_host__ 
#  define __cond_device__ 
#  define __cond_cuda__  //
#endif

#define return_if_not(value,cvalue) { auto __temp_value__ = (value); if ( __temp_value__ != (cvalue) ) return __temp_value__; }
#define return_if(value,cvalue) { auto __temp_value__ = (value); if ( __temp_value__ == (cvalue) ) return __temp_value__; }

#define ThrowError(...)  ({                                             \
      std::cerr<<std::endl<<"Error at "<< __FILE__ <<":"<<__LINE__<<std::endl; \
      pond::Error err(__VA_ARGS__); pond::__stacktrace_pretty_print(); throw err; \
    })

#ifdef DEBUG
#  define deprintf(fmt,...) fprintf(stdout,"%s(%d): " fmt "\n",__FILE__,__LINE__,##__VA_ARGS__)
#  define deerrf(fmt,...)   fprintf(stderr,"%s(%d): " fmt "\n",__FILE__,__LINE__,##__VA_ARGS__)
#else
#  define deprintf(fmt,...) 
#  define deerrf(fmt,...) 
#endif

#define dout __DebugOutput::out

#ifdef DEBUG
#  define OUT(str,var) ({ std::cerr<<__FILE__<<"("<<__LINE__<<#var<<"):"<<str<<" = "<<(var)<<std::endl; })
#  define VOUT(var) ({ std::cerr<<__FILE__<<"("<<__LINE__<<"):"<<#var<<" = "<<(var)<<std::endl; })
#  define MOUT(mat)  ({ std::cerr<<"matrix output "<<__FILE__<<"("<<__LINE__<<"):"<<#mat << std::endl; \
      (mat).DumpFile(#mat ".dat"); })
#  ifdef __CUDACC__
#    define DMOUT(mat) ({ std::cerr<<"matrix device output "<<__FILE__<<"("<<__LINE__<<"):"<<#mat<<std::endl; \
      (mat).DeviceToHost(); (mat).DumpFile(#mat ".dat"); })
#  else
#    define DMOUT(mat) (mat).DumpFile(#mat ".dat");
#  endif
#else
#  define OUT(str,var)
#  define VOUT(var) 
#  define DMOUT(mat)  
#  define MOUT(mat)  
#endif


#define ReturnObject(arg)  ({ ARGV = arg; return 1; })
#define ReturnString(arg)  ({ ARGV.SetString(arg); return 1; })
#define ReturnSymbol(arg)  ({ ARGV.SetSymbol(arg); return 1; })
#define ReturnBoolean(arg) ({ ARGV.SetBoolean(arg); return 1; })
#define ReturnNumber(arg)  ({ ARGV.SetNumber(arg); return 1; })
#define ReturnHold         ({ return 0;})
#define ReturnNormal       ({ return 1;})
#define ReturnNull         ({ ARGV.SetNull(); return 1;})
#define ReturnError        ({ return -1;})

//////////////////////////////////////////////////////////////////////////////

#define POND_CONCAT_IMPL(A, B) A##B
#define POND_CONCAT(A, B) POND_CONCAT_IMPL(A, B)
#define POND_GET_N(N, ...) POND_CONCAT(POND_GET_N_, N)(__VA_ARGS__)
#define POND_GET_N_0( _0, ...) _0
#define POND_GET_N_1( _0, _1, ...) _1
#define POND_GET_N_2( _0, _1, _2, ...) _2
#define POND_GET_N_3( _0, _1, _2, _3, ...) _3
#define POND_GET_N_4( _0, _1, _2, _3, _4, ...) _4
#define POND_GET_N_5( _0, _1, _2, _3, _4, _5, ...) _5
#define POND_GET_N_6( _0, _1, _2, _3, _4, _5, _6, ...) _6
#define POND_GET_N_7( _0, _1, _2, _3, _4, _5, _6, _7, ...) _7
#define POND_GET_N_8( _0, _1, _2, _3, _4, _5, _6, _7, _8, ...) _8
#define POND_GET_N_9( _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, ...) _9
#define POND_GET_N_10(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, ...) _10
#define POND_GET_N_11(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, ...) _11
#define POND_GET_N_12(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, ...) _12
#define POND_GET_N_13(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, ...) _13
#define POND_GET_N_14(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, ...) _14
#define POND_GET_N_15(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, ...) _15
#define POND_GET_N_16(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, ...) _16

#define POND_IS_EMPTY(...)                                              \
  POND_AND(POND_AND(POND_NOT(POND_HAS_COMMA(__VA_ARGS__)),              \
                    POND_NOT(POND_HAS_COMMA(__VA_ARGS__()))),           \
           POND_AND(POND_NOT(POND_HAS_COMMA(POND_COMMA_V __VA_ARGS__)), \
                    POND_HAS_COMMA(POND_COMMA_V __VA_ARGS__())))
#define POND_HAS_COMMA(...) POND_GET_N_16(__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0)
#define POND_COMMA_V(...) ,
#define POND_VA_OPT_COMMA(...) POND_COMMA_IF(POND_NOT(POND_IS_EMPTY(__VA_ARGS__)))
#define POND_NARG(...) POND_GET_N(16, __VA_ARGS__ POND_VA_OPT_COMMA(__VA_ARGS__) \
                                  16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

#define POND_INC_0 1
#define POND_INC_1 2
#define POND_INC_2 3
#define POND_INC_3 4
#define POND_INC_4 5
#define POND_INC_5 6
#define POND_INC_6 7
#define POND_INC_7 8
#define POND_INC_8 9
#define POND_INC_9 10
#define POND_INC_10 11
#define POND_INC_11 12
#define POND_INC_12 13
#define POND_INC_13 14
#define POND_INC_14 15
#define POND_INC_15 16
#define POND_INC_16 17
#define POND_INC_17 18

#define POND_CHECK_FOR_EACH(DO, ...)                                    \
  POND_CONCAT(POND_CHECK_FOR_EACH_, POND_CONCAT(POND_INC_,POND_NARG(__VA_ARGS__)))(DO, 1, __VA_ARGS__)
#define POND_CHECK_FOR_EACH_0(DO, IND, ...)
#define POND_CHECK_FOR_EACH_1(DO, IND, VAR, ...) DO(VAR, IND)
#define POND_CHECK_FOR_EACH_2(DO, IND, VAR, ...) DO(VAR, IND)         \
  POND_CHECK_FOR_EACH_1(DO, POND_CONCAT(POND_INC_,IND), __VA_ARGS__)
#define POND_CHECK_FOR_EACH_3(DO, IND, VAR, ...) DO(VAR, IND)         \
  POND_CHECK_FOR_EACH_2(DO, POND_CONCAT(POND_INC_,IND), __VA_ARGS__)
#define POND_CHECK_FOR_EACH_4(DO, IND, VAR, ...) DO(VAR, IND)         \
  POND_CHECK_FOR_EACH_3(DO, POND_CONCAT(POND_INC_,IND), __VA_ARGS__)

#define CHECK_POS(pos_type,i) {                                         \
    if (  ((unsigned char)pos_type) != __Any__ && ((unsigned char)pos_type) != ((unsigned char)ARGV[i].type()) ) { \
      zhWarning(std::string(MODULE_NAME)+"::"+(ARGV).Key(),"第 " +pond::ToString(i)+" 个参数应该是 "+pond::ToString(pos_type)+".") || \
        _Warning(std::string(MODULE_NAME)+"::"+(ARGV).Key(),Math::OrderForm(i)+" argument should be a "+pond::ToString(pos_type)+"."); \
      ReturnError;                                                      \
    }                                                                   \
  }
  
#define PD_ARGS_CHECK(...) {                                            \
    const int N_ARG = POND_NARG(__VA_ARGS__)+1;                         \
    if ( N_ARG > ARGV.Size() ){                                         \
      zhWarning(std::string(MODULE_NAME)+"::"+(ARGV).Key(),(ARGV).ToString()+" 调用参数个数应当是 "+pond::ToString(N_ARG)+" 个. 实际提供了 "+pond::ToString((ARGV).Size())+" 个参数" )|| \
        _Warning(std::string(MODULE_NAME)+"::"+(ARGV).Key(),(ARGV).ToString()+" called with "+pond::ToString((ARGV).Size())+((ARGV).Size()>1?" arguments; while ":" argument; while ")+pond::ToString(N_ARG)+" argument"+((N_ARG)>1?"s are":" is")+" required."); \
      ReturnError;                                                      \
    }                                                                   \
    POND_CHECK_FOR_EACH(CHECK_POS, __VA_ARGS__);                        \
  }

//////////////////////////////////////////////////////////////////////////////

#define CheckArgsShouldNotEqual(ARGV,n)  {                              \
    if((ARGV).Size()==(n)){                                             \
      zhWarning(std::string(MODULE_NAME)+"::"+(ARGV).Key(),(ARGV).ToString()+" 调用参数个数不能是 "+pond::ToString((ARGV).Size())+" 个" )|| \
      _Warning(std::string(MODULE_NAME)+"::"+(ARGV).Key(),(ARGV).ToString()+" is forbidden to call with "+pond::ToString((ARGV).Size())+((ARGV).Size()>1?" arguments.":" argument.")); \
      ReturnError;                                                      \
    }                                                                   \
  }
#define CheckArgsShouldEqual(ARGV,n) {                                  \
    if ( (ARGV).Size() != (n) ){                                        \
      zhWarning(std::string(MODULE_NAME)+"::"+(ARGV).Key(),(ARGV).ToString()+" 调用参数个数应当是 "+pond::ToString(n)+" 个. 实际提供了 "+pond::ToString((ARGV).Size())+" 个参数" )|| \
      _Warning(std::string(MODULE_NAME)+"::"+(ARGV).Key(),(ARGV).ToString()+" called with "+pond::ToString((ARGV).Size())+((ARGV).Size()>1?" arguments; while ":" argument; while ")+pond::ToString(n)+" argument"+((n)>1?"s are":" is")+" required."); \
      ReturnError;                                                      \
    }                                                                   \
  }
#define CheckArgsShouldBe(ARGV,n) CheckArgsShouldEqual(ARGV,n)
#define CheckArgsShouldNotBe(ARGV,n) CheckArgsShouldNotEqual(ARGV,n)
#define CheckArgsShouldNoLessThan(ARGV,n) {                             \
    if ( (ARGV).Size() < (n) ){                                         \
      zhWarning(std::string(MODULE_NAME)+"::"+(ARGV).Key(),(ARGV).ToString()+" 调用参数个数不能少于 "+pond::ToString(n)+" 个. 实际提供了 "+pond::ToString((ARGV).Size())+" 个参数" )|| \
      _Warning(std::string(MODULE_NAME)+"::"+(ARGV).Key(),(ARGV).ToString()+" called with "+pond::ToString((ARGV).Size())+" argument"+((ARGV).Size()>1?"s":"")+"; while no less than "+pond::ToString(n)+" argument"+((n)>1?"s are":" is")+" required."); \
      ReturnError;                                                      \
    }                                                                   \
  }
#define CheckArgsShouldNoMoreThan(ARGV,n) {                             \
    if ( (ARGV).Size() > (n) ){                                         \
      zhWarning(std::string(MODULE_NAME)+"::"+(ARGV).Key(),(ARGV).ToString()+" 调用参数个数不能多于 "+pond::ToString(n)+" 个. 实际提供了 "+pond::ToString((ARGV).Size())+" 个参数" )|| \
      _Warning(std::string(MODULE_NAME)+"::"+(ARGV).Key(),(ARGV).ToString()+" called with "+pond::ToString((ARGV).Size())+" argument"+((ARGV).Size()>1?"s":"")+"; while no more than "+pond::ToString(n)+" argument"+((n)>1?"s are":" is")+" required."); \
      ReturnError;                                                      \
    }                                                                   \
  }
#define CheckArgsShouldBeWithin(ARGV,n1,n2) {                           \
    if ( (ARGV).Size()<(n1) || (ARGV).Size()>(n2) ){                    \
      zhWarning(std::string(MODULE_NAME)+"::"+(ARGV).Key(),(ARGV).ToString()+" 调用参数个数只能介于 "+ToString(n1)+", "+ToString(n2)+" 之间. 实际提供了 "+pond::ToString((ARGV).Size())+" 个参数" )|| \
      _Warning(std::string(MODULE_NAME)+"::"+(ARGV).Key(),(ARGV).ToString()+" called with "+pond::ToString((ARGV).Size())+" argument"+((ARGV).Size()>1?"s":"")+"; while the required number of arguments should be in the range of "+ToString(n1)+", "+ToString(n2)+"."); \
      ReturnError;                                                      \
    }                                                                   \
  }
#define CheckArgsShouldNotBeWithin(ARGV,n1,n2) {                        \
    if ( (ARGV).Size()>=(n1) && (ARGV).Size()<=(n2) ){                  \
      zhWarning(std::string(MODULE_NAME)+"::"+(ARGV).Key(),(ARGV).ToString()+" 调用参数个数不能能介于 "+ToString(n1)+", "+ToString(n2)+" 之间. 实际提供了 "+pond::ToString((ARGV).Size())+" 个参数" )|| \
      _Warning(std::string(MODULE_NAME)+"::"+(ARGV).Key(),(ARGV).ToString()+" called with "+pond::ToString((ARGV).Size())+" argument"+((ARGV).Size()>1?"s":"")+"; while the required number of arguments should not be in the range of "+ToString(n1)+", "+ToString(n2)+"."); \
      ReturnError;                                                      \
    }                                                                   \
  }
#define CheckArgsShouldInForm(ARGV,form) {                              \
    if ( !Pattern::CheckArgs( (ARGV),(form) ) ) {                       \
      zhWarning(std::string(MODULE_NAME)+"::"+(ARGV).Key(),(ARGV).Key()+" 应该以 "+(ARGV).Key()+"("+form+") 的形式调用") || \
      _Warning(std::string(MODULE_NAME)+"::"+(ARGV).Key(),(ARGV).Key()+" should be called in the form of "+(ARGV).Key()+"["+form+"]"); \
      ReturnError;                                                      \
    }                                                                   \
  }
#define CheckArgsShouldNotBeEmpty(ARGV) {                               \
    if ( (ARGV).Size() < 1){                                            \
      zhWarning(std::string(MODULE_NAME)+"::"+(ARGV).Key(),"调用参数不能为空") || \
      _Warning(std::string(MODULE_NAME)+"::"+(ARGV).Key(),"Argument of "+(ARGV).Key()+" should not be empty."); \
      ReturnError;                                                      \
    }                                                                   \
  }
#define CheckArgsShouldBeEmpty(ARGV) {                                  \
    if ( (ARGV).Size() > 0){                                            \
      zhWarning(std::string(MODULE_NAME)+"::"+(ARGV).Key(),"调用参数应该为空") || \
      _Warning(std::string(MODULE_NAME)+"::"+(ARGV).Key(),"Argument of "+(ARGV).Key()+" should be empty."); \
      ReturnError;                                                      \
    }                                                                   \
  }
#define CheckArgsShouldBeNumber(ARGV,i) {                               \
    if ( !(ARGV)[i].NumberQ() ){                                        \
      zhWarning(std::string(MODULE_NAME)+"::"+(ARGV).Key(),"第" +pond::ToString(i)+" 个参数应该是数字.") || \
      _Warning(std::string(MODULE_NAME)+"::"+(ARGV).Key(),Math::OrderForm(i)+" argument should be a Number."); \
      ReturnError;                                                      \
    }                                                                   \
  }
#define CheckArgsShouldBeString(ARGV,i) {                               \
    if ( !(ARGV)[i].StringQ() ){                                        \
      zhWarning(std::string(MODULE_NAME)+"::"+(ARGV).Key(),"第" +pond::ToString(i)+" 个参数应该是字符串.") || \
      _Warning(std::string(MODULE_NAME)+"::"+(ARGV).Key(),Math::OrderForm(i)+" argument should be a String."); \
      ReturnError;                                                      \
    }                                                                   \
  }
#define CheckArgsShouldBeSymbol(ARGV,i) {                               \
    if ( !(ARGV)[i].SymbolQ() ){                                        \
      zhWarning(std::string(MODULE_NAME)+"::"+(ARGV).Key(),"第" +pond::ToString(i)+" 个参数应该是符号.") || \
      _Warning(std::string(MODULE_NAME)+"::"+(ARGV).Key(),Math::OrderForm(i)+" argument should be a Symbol."); \
      ReturnError;                                                      \
    }                                                                   \
  }
#define CheckArgsShouldBeList(ARGV,i) {                                 \
    if ( (ARGV)[i].AtomQ() ){                                           \
      zhWarning(std::string(MODULE_NAME)+"::"+(ARGV).Key(),"第" +pond::ToString(i)+" 个参数应该是列表.") || \
      _Warning(std::string(MODULE_NAME)+"::"+(ARGV).Key(),Math::OrderForm(i)+" argument should be an List Object."); \
      ReturnError;                                                      \
    }                                                                   \
  }
#define CheckArgsShouldBeListWithHead(ARGV,i,head) {                    \
    if ( not (ARGV)[i].ListQ( head ) ){                                 \
      zhWarning(std::string(MODULE_NAME)+"::"+(ARGV).Key(),"第" +pond::ToString(i)+" 个参数应该是 "+Object(ObjectType::Symbol,head).ToString()+" 列表.") || \
      _Warning(std::string(MODULE_NAME)+"::"+(ARGV).Key(),Math::OrderForm(i)+" argument should be an Object with head of "+Object(ObjectType::Symbol,head).ToString()+"."); \
      ReturnError;                                                      \
    }                                                                   \
  }
#define CheckArgsShouldNotBeNumber(ARGV,i) {                            \
    if ( (ARGV)[i].NumberQ() ){                                         \
      zhWarning(std::string(MODULE_NAME)+"::"+(ARGV).Key(),"第" +pond::ToString(i)+" 个参数不能是数字.") || \
      _Warning(std::string(MODULE_NAME)+"::"+(ARGV).Key(),Math::OrderForm(i)+" argument should not be a Number."); \
      ReturnError;                                                      \
    }                                                                   \
  }
#define CheckArgsShouldNotBeString(ARGV,i) {                            \
    if ( (ARGV)[i].StringQ() ){                                         \
      zhWarning(std::string(MODULE_NAME)+"::"+(ARGV).Key(),"第" +pond::ToString(i)+" 个参数不能是字符串.") || \
      _Warning(std::string(MODULE_NAME)+"::"+(ARGV).Key(),Math::OrderForm(i)+" argument should not be a String."); \
      ReturnError;                                                      \
    }                                                                   \
  }
#define CheckArgsShouldNotBeSymbol(ARGV,i) {                            \
    if ( (ARGV)[i].SymbolQ() ){                                         \
      zhWarning(std::string(MODULE_NAME)+"::"+(ARGV).Key(),"第" +pond::ToString(i)+" 个参数不能是符号.") || \
      _Warning(std::string(MODULE_NAME)+"::"+(ARGV).Key(),Math::OrderForm(i)+" argument should not be a Symbol."); \
      ReturnError;                                                      \
    }                                                                   \
  }
#define CheckArgsShouldNotBeList(ARGV,i) {                              \
    if ( (ARGV)[i].ListQ() ){                                           \
      zhWarning(std::string(MODULE_NAME)+"::"+(ARGV).Key(),"第" +pond::ToString(i)+" 个参数不能是列表.") || \
      _Warning(std::string(MODULE_NAME)+"::"+(ARGV).Key(),Math::OrderForm(i)+" argument should not be an List."); \
      ReturnError;                                                      \
    }                                                                   \
  }
#define CheckArgsShouldNotBeListWithHead(ARGV,i,head) {                 \
    if ( (ARGV)[i].ListQ( head) ){                                      \
      zhWarning(std::string(MODULE_NAME)+"::"+(ARGV).Key(),"第" +pond::ToString(i)+" 个参数不能是 "+Object(ObjectType::Symbol,head).ToString()+" 列表.") || \
      _Warning(std::string(MODULE_NAME)+"::"+(ARGV).Key(),Math::OrderForm(i)+" argument should not be an List with head of "+Object(ObjectType::Symbol,head).ToString()+"."); \
      ReturnError;                                                      \
    }                                                                   \
  }

#define CheckShouldNotEqual(n)                 CheckArgsShouldNotEqual(ARGV,n)
#define CheckShouldEqual(n)                    CheckArgsShouldEqual(ARGV,n)
#define CheckShouldNoLessThan(n)               CheckArgsShouldNoLessThan(ARGV,n)
#define CheckShouldNoMoreThan(n)               CheckArgsShouldNoMoreThan(ARGV,n)
#define CheckShouldBeWithin(n1,n2)             CheckArgsShouldBeWithin(ARGV,n1,n2)
#define CheckShouldNotBeWithin(n1,n2)          CheckArgsShouldNotBeWithin(ARGV,n1,n2)
#define CheckShouldInForm(form)                CheckArgsShouldInForm(ARGV,form)
#define CheckShouldNotBeEmpty()                CheckArgsShouldNotBeEmpty(ARGV)
#define CheckShouldBe(n)                    CheckArgsShouldEqual(ARGV,n)
#define CheckShouldBeEmpty()                   CheckArgsShouldBeEmpty(ARGV)
#define CheckShouldBeNumber(i)                 CheckArgsShouldBeNumber(ARGV,i)
#define CheckShouldBeString(i)                 CheckArgsShouldBeString(ARGV,i)
#define CheckShouldBeSymbol(i)                 CheckArgsShouldBeSymbol(ARGV,i)
#define CheckShouldBeList(i)                   CheckArgsShouldBeList(ARGV,i)
#define CheckShouldBeListWithHead(i,head)      CheckArgsShouldBeListWithHead(ARGV,i,head)
#define CheckShouldNotBeNumber(i)              CheckArgsShouldNotBeNumber(ARGV,i)
#define CheckShouldNotBeString(i)              CheckArgsShouldNotBeString(ARGV,i)
#define CheckShouldNotBeSymbol(i)              CheckArgsShouldNotBeSymbol(ARGV,i)
#define CheckShouldNotBeList(i)                CheckArgsShouldNotBeList(ARGV,i)
#define CheckShouldNotBeListWithHead(i,head)   CheckArgsShouldNotBeListWithHead(ARGV,i,head)

#define ReturnIfNotEqual(n){if( (ARGV).Size()!=(n)){ ReturnHold; } }
#define ReturnIfEqual(n) { if ( (ARGV).Size() == (n) ){  ReturnHold; } }
#define ReturnIfLessThan(n) { if ( (ARGV).Size() < (n) ){  ReturnHold; } }
#define ReturnIfMoreThan(n) { if ( (ARGV).Size() > (n) ){  ReturnHold; } }
#define ReturnIfNotWithin(n1,n2) { if ( (ARGV).Size()<(n1) || (ARGV).Size()>(n2) ){  ReturnHold; } }
#define ReturnIfWithin(n1,n2) { if ( (ARGV).Size()>=(n1) && (ARGV).Size()<=(n2) ){  ReturnHold; } }
#define ReturnIfNotInForm(form) { if ( !Pattern::CheckArgs( (ARGV),(form) ) ) {  ReturnHold; } }
#define ReturnIfNotEmpty() { if ( (ARGV).Size() >= 1){  ReturnHold; } }
#define ReturnIfEmpty() { if ( (ARGV).Size() < 1){  ReturnHold; } }
#define ReturnIfNotNumber(i) { if ( !(ARGV)[i].NumberQ() ){  ReturnHold; } }
#define ReturnIfNotString(i) { if ( !(ARGV)[i].StringQ() ){  ReturnHold; } }
#define ReturnIfNotSymbol(i) { if ( !(ARGV)[i].SymbolQ() ){  ReturnHold; } }
#define ReturnIfNotAtom(i) { if ( !(ARGV)[i].AtomQ() ){  ReturnHold; } }
#define ReturnIfNotList(i) { if ( (ARGV)[i].AtomQ() ){  ReturnHold; } }

#define Conjunct_Context(func) if ( ARGV.ListQ( SYMID_OF_Conjunct ) and ARGV[1].ListQ( SYMID_OF_##func) )
#define Conjunct_Var_Case_Condition( sym ) ( ARGV[2].SymbolQ( SYMID_OF_##sym ) )
#define Conjunct_Var_Case( sym ) if ( Conjunct_Var_Case_Condition(sym) )
#define Conjunct_Var_Case2( sym,sym2 ) if ( Conjunct_Var_Case_Condition(sym)  || \
                                            Conjunct_Var_Case_Condition(sym2) )
#define Conjunct_Var_Case3( sym,sym2,sym3 ) if ( Conjunct_Var_Case_Condition(sym)  || \
                                                 Conjunct_Var_Case_Condition(sym2) || \
                                                 Conjunct_Var_Case_Condition(sym3) )
#define Conjunct_Var_Case4( sym,sym2,sym3, sym4 ) if ( Conjunct_Var_Case_Condition(sym)  || \
                                                       Conjunct_Var_Case_Condition(sym2) || \
                                                       Conjunct_Var_Case_Condition(sym3) || \
                                                       Conjunct_Var_Case_Condition(sym4) )
#define Conjunct_Var_Case5( sym,sym2,sym3, sym4,sym5 ) if ( Conjunct_Var_Case_Condition(sym)  || \
                                                            Conjunct_Var_Case_Condition(sym2) || \
                                                            Conjunct_Var_Case_Condition(sym3) || \
                                                            Conjunct_Var_Case_Condition(sym4) || \
                                                            Conjunct_Var_Case_Condition(sym5) )
#define Conjunct_Var_Case6( sym,sym2,sym3, sym4,sym5,sym6 ) if ( Conjunct_Var_Case_Condition(sym)  || \
                                                                 Conjunct_Var_Case_Condition(sym2) || \
                                                                 Conjunct_Var_Case_Condition(sym3) || \
                                                                 Conjunct_Var_Case_Condition(sym4) || \
                                                                 Conjunct_Var_Case_Condition(sym5) || \
                                                                 Conjunct_Var_Case_Condition(sym6) )

#define Conjunct_Case_Condition( sym ) ( ARGV[2].ListQ( SYMID_OF_##sym ) )
#define Conjunct_Case( sym ) if ( Conjunct_Case_Condition(sym) )
#define Conjunct_Case2( sym,sym2 ) if ( Conjunct_Case_Condition(sym)  || \
                                        Conjunct_Case_Condition(sym2) )
#define Conjunct_Case3( sym,sym2,sym3 ) if ( Conjunct_Case_Condition(sym)  || \
                                             Conjunct_Case_Condition(sym2) || \
                                             Conjunct_Case_Condition(sym3) )
#define Conjunct_Case4( sym,sym2,sym3, sym4 ) if ( Conjunct_Case_Condition(sym)  || \
                                                   Conjunct_Case_Condition(sym2) || \
                                                   Conjunct_Case_Condition(sym3) || \
                                                   Conjunct_Case_Condition(sym4) )
#define Conjunct_Case5( sym,sym2,sym3, sym4,sym5 ) if ( Conjunct_Case_Condition(sym)  || \
                                                        Conjunct_Case_Condition(sym2) || \
                                                        Conjunct_Case_Condition(sym3) || \
                                                        Conjunct_Case_Condition(sym4) || \
                                                        Conjunct_Case_Condition(sym5) )
#define Conjunct_Case6( sym,sym2,sym3, sym4,sym5,sym6 ) if ( Conjunct_Case_Condition(sym)  || \
                                                             Conjunct_Case_Condition(sym2) || \
                                                             Conjunct_Case_Condition(sym3) || \
                                                             Conjunct_Case_Condition(sym4) || \
                                                             Conjunct_Case_Condition(sym5) || \
                                                             Conjunct_Case_Condition(sym6) )

#define Set_Context(func) if ( ARGV.ListQ( SYMID_OF_Set ) and ARGV[1].ListQ( SYMID_OF_##func) )

#define Part_Context(func) if ( ARGV.ListQ( SYMID_OF_Part ) and ARGV[1].ListQ( SYMID_OF_##func) )

#define Set_Part_Context if ( ARGV.ListQ( SYMID_OF_Set ) and ARGV[1].ListQ( SYMID_OF_Part) )

#define DelaySet_Context(func) if ( ARGV.ListQ( SYMID_OF_Set) and ARGV[1].ListQ() and ARGV[1][0].ListQ(SYMID_OF_##func) )

#define DelayFunction_Context(func) if ( ARGV[0].ListQ( SYMID_OF_##func ) )

#define Declare_ValueTablePair_Id_Of(sym) Index ValueTablePair_Id_Of_##sym;
#define Init_ValueTablePair_Id_Of(sym) ({ Object obj; obj.SetSymbol(#sym); ValueTablePair_Id_Of_##sym = EvaKernel->GetOrNewValuePairRefAtTop( obj ).objid ; })


#if defined(__CUDACC__)
inline void HandleError( cudaError_t cu_err,const char *file,int line ) {
  if (cu_err != cudaSuccess) {
    printf( "%s in %s at line %d\n", cudaGetErrorString( cu_err ), file, line );
    exit( EXIT_FAILURE );
  }
}
#  define CUDA_HANDLE_ERROR( cu_err ) (HandleError( cu_err, __FILE__, __LINE__ ))
#  define CUDA_HANDLE_NULL( a ) {                                       \
    if (a == NULL) { printf( "Host memory failed in %s at line %d\n",	\
                             __FILE__, __LINE__ );                      \
                     exit( EXIT_FAILURE );}}
#  define CUDA_LAST_ERROR() ({ cudaError_t cu_err = cudaGetLastError(); if ( cu_err!=cudaSuccess ) ThrowError("Cuda", (std::string) cudaGetErrorString(cu_err)); })
#else
#  define CUDA_HANDLE_ERROR( cu_err ) 
#  define CUDA_HANDLE_NULL( a )
#  define CUDA_LAST_ERROR() 
#endif

#define _blockDim3(ni,nj,nk) dim3(ni,nj,nk)
#define _blockDim2(ni,nj) dim3(ni,nj)
#define _blockDim1(ni) dim3(ni)
#define _blockIdx3(ii,jj,kk) const u_int ii = blockIdx.x,jj = blockIdx.y,kk = blockIdx.z
#define _blockIdx2(ii,jj) const u_int u_ii = blockIdx.x, jj = blockIdx.y
#define _blockIdx1(ii) const u_int u_ii = blockIdx.x

#define _threadDim3(ni,nj,nk) dim3(ni,nj,nk)
#define _threadDim2(ni,nj) dim3(ni,nj)
#define _threadDim1(ni) dim3(ni)
#define _threadIdx3(ii,jj,kk) const u_int ii = threadIdx.x, jj = threadIdx.y, kk = threadIdx.z
#define _threadIdx2(ii,jj) const u_int ii = threadIdx.x, jj = threadIdx.y;
#define _threadIdx1(ii) const u_int ii = threadIdx.x

#define __CudaThreadNumberPerBlock EvaSettings.threadNumberPerBlock
#define _cuDim(N) dim3(__CudaBlockNumber(N)),dim3(__CudaThreadNumberPerBlock)

#define __ASSERT_PRAGMA_MATRIX_T_DATA(var)                                     \
    {                                                                          \
        if (pond::GetParallelMode() == ParallelModeCpu && !(var).Data) {       \
            ThrowError("pragma::launch_kernel",                                \
                       "Data of Matrix_T argument '" #var                      \
                       "' not initialized when kernel called on cpu mode.");   \
        }                                                                      \
        if (pond::GetParallelMode() == ParallelModeGpu && !(var).DataDevice) { \
            ThrowError("pragma::launch_kernel",                                \
                       "DataDevice of Matrix_T argument '" #var                \
                       "' not initialized when kernel called on gpu mode.");   \
        }                                                                      \
    }

#endif
