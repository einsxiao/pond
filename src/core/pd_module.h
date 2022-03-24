#ifndef Modules_POND_H
#define Modules_POND_H

////////////////////////////////////////////////////////////////////////////////////////
namespace pond{
  
  class Module{
  public:
    std::string		  moduleName, moduleDescreption;
  public: //
                    Module(std::string name); 
    virtual        ~Module();
    //////////////////////////////
    static int      RegisterFunction(const char*funcName,MemberFunction func,const Module*mod,const char*des="");
    static int      RegisterFunction(const char*funcName,StaticFunction func,const Module*mod=NULL,const char*des="");
    static int      AddAttribute(const char*key, const char*attribute);
    static int      AddAttribute(const char*key, const AttributeType type);
    //////////////////////////////
    std::string     GetModuleName();
    std::string     ModuleDescription();
    int             SetModuleDescription(std::string des);
    //////////////////////////////
  };
  typedef Module   *type_create();
  typedef void      type_destroy(Module*);
  typedef double    type_return_functions();
  typedef int       type_regsterfunction(Module*);

  /////////new way
  // #define __MODULE_NAME__ System
  ////////////////////////////////////////////////////////////////////////////////////////
#define _MODULE_NAME(moduleName) #moduleName
#define __MODULE_NAME(moduleName) _MODULE_NAME(moduleName)
#define MODULE_NAME __MODULE_NAME(__MODULE_NAME__)
////////////////////////////////////////////////////////////////////////////////////////
#define _ModuleType(moduleName) moduleName##Module
#define __ModuleType(moduleName) _ModuleType(moduleName)
#define ModuleType __ModuleType(__MODULE_NAME__)
////////////////////////////////////////////////////////////////////////////////////////
#define _create_ModuleType(moduleName) __create_##moduleName##Module
#define __create_ModuleType(moduleName) _create_ModuleType(moduleName)
#define CreateModuleType __create_ModuleType(__MODULE_NAME__)
////////////////////////////////////////////////////////////////////////////////////////
#define _destroy_ModuleType(moduleName) __destroy_##moduleName##Module
#define __destroy_ModuleType(moduleName) _destroy_ModuleType(moduleName)
#define DestroyModuleType __destroy_ModuleType(__MODULE_NAME__)
////////////////////////////////////////////////////////////////////////////////////////
#define _destroy_ModuleType(moduleName) __destroy_##moduleName##Module
#define __destroy_ModuleType(moduleName) _destroy_ModuleType(moduleName)
#define DestroyModuleType __destroy_ModuleType(__MODULE_NAME__)
////////////////////////////////////////////////////////////////////////////////////////
#define _ReturnFunctionsType(module) __return_functions_##module
#define __ReturnFunctionsType(module) _ReturnFunctionsType(module)
#define ReturnFunctionsType __ReturnFunctionsType(__MODULE_NAME__)
////////////////////////////////////////////////////////////////////////////////////////
#define _RegisterFunctionsType(module,func) func##_Eva_Register_##module
#define RegisterFunctionsType(module,func) _RegisterFunctionsType(module,func)
////////////////////////////////////////////////////////////////////////////////////////

#define __REGISTER_POND_FUNCTION__(func,description...)                 \
  int func##_Eva_init_int(__register_function(#func));                  \
  extern "C" int RegisterFunctionsType(__MODULE_NAME__,func)(Module*module){ \
    return Module::RegisterFunction(                                    \
                                    #func,                              \
                                    (MemberFunction)(&ModuleType::PD_##func), \
                                    module,                             \
                                    ##description                       \
                                                                        ); \
  };
  ////////////////////////////////////////////////////////////////////////////////////////
#define __DEBUG_MAIN__(file)                                            \
  int main(int argc,char*argv[]){                                       \
    pond::dout<<">>>debug main to run '"<<file<<"'<<<"<<std::endl;      \
    pond::Evaluation eva(true);                                         \
    eva.argc = argc; eva.argv = argv;                                   \
    try{                                                                \
      pond::Object tmp;                                                 \
      eva.EvaluateFile(file,tmp);                                       \
    }catch ( const pond::ExceptionQuit&err ){                           \
      return err.code;                                                  \
    }catch ( const pond::Error&err){                                    \
      std::cerr<<std::endl<<err.swhat()<<std::endl;                     \
    }catch ( const std::exception &err){                                \
      std::cerr<<std::endl<<"Sourcecode::Error: "<<err.what()<<std::endl; \
      return 1;                                                         \
    }catch ( ... ){                                                     \
      std::cerr<<"\nSystem::Error: Unexpected error occured."<<std::endl; \
      return 1;                                                         \
    }                                                                   \
    return 0;                                                           \
  }                                                                 
  ////////////////////////////////////////////////////////////////////////////////////////

#define __REGISTER_POND_MODULE__                                        \
  static pond::Object __module_function_list;                           \
  static int __register_function(std::string func){                     \
    if ( __module_function_list.NullQ() ){                              \
      __module_function_list.SetList();                                 \
    }                                                                   \
    __module_function_list.PushBackSymbol( func.c_str() );              \
    return 0;                                                           \
  }                                                                     \
  extern "C" double ReturnFunctionsType(){                              \
    return *reinterpret_cast<double*>(&__module_function_list.objid);   \
  };                                                                    \
  extern "C" pond::Module *CreateModuleType(){                          \
    return new ModuleType();                                            \
  }                                                                     \
  extern "C" void DestroyModuleType(pond::Module*p){                    \
    delete (ModuleType*)p;                                              \
  }


};

#endif




