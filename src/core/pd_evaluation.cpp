#include<pd_headers.h>
#include"dlfcn.h" //dlopen dlsym
// #include<emscripten.h>
#include<sstream>
#define __MODULE_NAME__ Kernel

using namespace std;
using namespace pond;

EMSCRIPTEN_KEEPALIVE
EvaRecord::EvaRecord(){
  memset(attributes,0,sizeof(bool)*24);
};

EMSCRIPTEN_KEEPALIVE
EvaRecord::~EvaRecord(){
};

EMSCRIPTEN_KEEPALIVE
EvaRecord::EvaRecord(const EvaRecord&er){
  for ( int i=0;i< 24;i++)
    attributes[i] = er.attributes[i];
  staticFunction = er.staticFunction;
  module = er.module;
  memberFunction = er.memberFunction;
  description = er.description;
}

EMSCRIPTEN_KEEPALIVE
void EvaRecord::Update(const EvaRecord&er){
  for ( int i=0;i< 24;i++)
    attributes[i] = attributes[i] or er.attributes[i];
  int seted = false;
#define deal(x) if ( seted ){ x = er.x; if (x != NULL) seted = true; }else{ x= NULL; }
  deal(staticFunction);
  module = er.module;
  memberFunction = er.memberFunction;
  deal(memberFunction);
#undef deal
  description = er.description;
}

void EvaRecord::ClearValues(){
  staticFunction = NULL;
  module = NULL;
  memberFunction = NULL;
  description = "";
}

void EvaRecord::ClearAll(){
  memset(attributes,0,sizeof(bool)*24);
  staticFunction = NULL;
  module = NULL;
  memberFunction = NULL;
  description = "";
}

int EvaRecord::Call(Object&ARGV){
  // if ( not isCallable() ){
  //   zhErroring("计算单元",ARGV.ToString()+" 不是可以执行的注册函数.");
  //   Erroring("EvaKernel",ARGV.ToString()+" is not a callable function.");
  //   return -1;
  // }
  if ( staticFunction )
    return (staticFunction)(ARGV);
  if ( module && memberFunction )
    return ((module)->*(memberFunction))(ARGV);
  return 0;
}

bool EvaRecord::isCallable(){
  if ( (module and memberFunction) or staticFunction ) return true;
  return false;
}

/******************
// statusCode,   statusObject is the store for status
0    normal nothing special

44    break;         nothing stored

66    continue;      nothing stored

88    return         return value stored in statusObject

55    throw          exception stored in statusObject


**********************/

static vector<Index> symidToPairIndex;

Evaluation::Evaluation(bool debug, bool pmark){
  // EvaKernel
  //dout<<"try initialize evaluation"<<endl;
  // exit(0);
  // __GlobalPoolPtr = new EvaMemoryPool();
  GlobalPool.Eva = this;
  maxRecursionDepth = 256;
  /////////////
  // init LANG info
  this->lang = GetEnv("POND_LANG","zh");
  /////////////
  this->DebugMode   = debug;
  this->pmark       = pmark;
  newContext();
  evaRecordTable.push_back(NULL);
  evaRecordTable.push_back(NULL); //Get Value will return a value ( > 2) when is a eva record
  EvaluationDepth = 0;
  ///////////////////////////////
  //dout<<"basic info ready, try register core functions"<<endl;
  {
    // Module::RegisterFunction("GetModule",Evaluation::PGetModule_Eva,NULL,"Get Module into Pond System");
    Module::RegisterFunction("Import",Evaluation::PD_Import,NULL,"Import Module into Pond Kernel");
    Module::RegisterFunction("import",Evaluation::PD_Import,NULL,"Import modules into Pond Kernel");
    GlobalPool.Symbols.GetOrNew("导入",0,220,1,1);
    Module::RegisterFunction("导入",Evaluation::PD_Import,NULL,"Import modules into Pond Kernel");
    Module::AddAttribute("import",AttributeType::HoldAll);
    Module::AddAttribute("导入",AttributeType::HoldAll);
    Module::RegisterFunction("Remove",Evaluation::PD_Remove,NULL,"Remove loaded modules");

    // a dangerous function, 
    // Module::RegisterFunction("ReImport",Evaluation::PD_ReImport,NULL,"ReImport Module into Pond Kernel");
  }
  
  // {
  //   Module::RegisterFunction("RemoveModule",Evaluation::PD_RemoveModule,NULL,"Remove Module from Bond System"); 
  //   Module::AddAttribute("RemoveModule",AttributeType::Listable);
  // }
  {
    Module::RegisterFunction("Print", Evaluation::PD_Print, NULL, "Print Exression Value" );
    Module::RegisterFunction("print", Evaluation::PD_Print, NULL, "Print Exression Value" );
    Module::RegisterFunction("打印", Evaluation::PD_Print, NULL, "Print Exression Value" );
    Module::AddAttribute("Print",AttributeType::HoldAll);
    Module::AddAttribute("打印",AttributeType::HoldAll);
    Module::AddAttribute("print",AttributeType::HoldAll);
    // Module::RegisterFunction("Console", Evaluation::PD_Console, NULL, "Print Exression Value to console" );
    // Module::RegisterFunction("console", Evaluation::PD_Console, NULL, "Print Exression Value to console" );
    // Module::RegisterFunction("控制台", Evaluation::PD_Console, NULL, "Print Exression Value to console" );
    Module::RegisterFunction("DumpToJson", Evaluation::PD_DumpToJson, NULL, "Print To Json String" );
    Module::RegisterFunction("PrintJson",Evaluation::PD_PrintJson,NULL,"Print as Json object" );
    Module::AddAttribute("JSON",AttributeType::Protected);
    // Module::RegisterFunction("Profiling",Evaluation::PD_Profiling,NULL,"Print as Json object" );
    // Module::AddAttribute("Profileing",AttributeType::HoldAll);
  }
  {
    Module::RegisterFunction("WriteFile", Evaluation::PD_WriteFile, NULL, "Write content to file");
    // Module::RegisterFunction("InitializeDin", Evaluation::PD_InitializeDin, NULL, "Initialize din");
  }
  {
    Module::AddAttribute("FullForm",AttributeType::OneIdentity);
  }
  {
    Module::RegisterFunction("FetchFullContext", Evaluation::PD_FetchFullContext, NULL, "" );
    Module::RegisterFunction("FetchContext", Evaluation::PD_FetchContext, NULL, "" );
    Module::RegisterFunction("SetMaxRecursionDepth",Evaluation::PD_SetMaxRecursionDepth  ,NULL,""); 
    Module::RegisterFunction("GetMemInfo",Evaluation::PD_GetMemInfo,NULL,""); 
    Module::RegisterFunction("GetStackInfo",Evaluation::PD_GetStackInfo,NULL,""); 
  }

  {
    Module::RegisterFunction("exit",Evaluation::PD_exit,NULL,"zh:退出程序|||exit current kernel"); 
    Module::RegisterFunction("Exit",Evaluation::PD_exit,NULL,"zh:退出程序|||exit current kernel"); 

    Module::RegisterFunction("help",Evaluation::PD_help,NULL,"zh:help([obj])\n  获得[obj]的帮助信息.|||help([obj])\n  get help info for [obj]."); 
    Module::RegisterFunction("Help",Evaluation::PD_help,NULL,""); 
    Module::AddAttribute("help",AttributeType::HoldAll);
    Module::AddAttribute("Help",AttributeType::HoldAll);

  }
  

  //dout<<"try load some preloaded modules"<<endl;
  GetModule("List");
  GetModule("Arithmatics");
  GetModule("System");
  GetModule("Class");
  GetModule("String");
  GetModule("OS");
  //dout<<"modules loaded"<<endl;

  if ( GetEnv("POND_ENABLE_PYTHON")   == "yes" ){
    GetModule("Python"); 
  }
  if ( GetEnv("POND_ENABLE_MPI")      == "yes" ){
    GetModule("MPI");
  }
  if ( GetEnv("POND_ENABLE_CUDA")     == "yes" ){
    GetModule("Matrix");
    GetModule("MatrixOperations");
  }
  // ///////////////////////////
  // string matlab_root = GetEnv("MATLAB_ROOT");
  // if (  matlab_root != "" ){ 
  //   GetModule("MatlabEngine");
  // }
  // string mathematica_root = GetEnv("MATHEMATICA_ROOT");
  // if (  mathematica_root != "" ){ 
  //   GetModule("Mathematica");
  // }
  
  //only enable symidToPairIndex in relase mode
  if ( ! this->DebugMode ) {
    //init symidToPairIndex
    //dout<<"init the global sym to pair table"<<endl;
    Object pairTable = (*valueTables.begin())->pairTable;
    //dout<<"init pair Table =" << pairTable <<endl;
    int maxID = 0;
    for (int i=1; i<= pairTable.Size(); i++ ){
      //dout<<"add "<< pairTable[i]<< " to map Table for evaluation"<<endl;
      int id = pairTable[i][1].SymbolId();
      //dout<<"current sym id = "<< id << endl;
      if ( id > maxID ) maxID = id;
      if ( id >= symidToPairIndex.size() )
        symidToPairIndex.resize( id+1 );
      symidToPairIndex[id] = pairTable[i].objid;
    }
    //dout<<"symidToPairIndex final size = "<< symidToPairIndex.size() <<" maxID ="<<maxID<<endl;
  }
  // currentValueTable = (*valueTables.begin()).first;

  //dout<<"new a context to evaluate in a level 1 context"<<endl;
  newContext();

  //dout<<"evaluation done"<<endl;

}


Evaluation::~Evaluation(){
  // deconstruct the many Modules or other malloced memory.
  while ( moduleTable.begin()!=moduleTable.end() ){
    // module itself will erase the module and delete the module with its type
    if ( moduleTable.begin()->second!=NULL )
      delete moduleTable.begin()->second;
    moduleTable.erase( moduleTable.begin() );
  }
  while ( evaRecordTable.begin()!=evaRecordTable.end() ){
    if ( *evaRecordTable.begin()  != NULL )
      delete *evaRecordTable.begin();
    evaRecordTable.erase( evaRecordTable.begin() );
  }
  while ( valueTables.begin()!=valueTables.end() ){
    if ( valueTables.back() !=NULL )
      delete *valueTables.begin() ;
    valueTables.erase( valueTables.begin() );
  }
}

int Evaluation::InsertModule(string moduleName,Module*module){
  pair< map<string,Module*>::iterator, bool > ret;
  ret = moduleTable.insert( pair<string,Module*>(moduleName,module) );
  if ( !ret.second )
    return false;
  return true;
}

Module *Evaluation::ModulePtr(string moduleName){
  map<string,Module*>::iterator iter;
  iter = moduleTable.find( moduleName );
  if ( iter == moduleTable.end() ){
    GetModuleLib(moduleName);
    iter = moduleTable.find(moduleName);
    if ( iter == moduleTable.end() )
      return NULL;
  }
  return iter->second;
}

int Evaluation::GetModuleLib(string moduleName,bool silent){
  //dout<< "try get lib of "<<moduleName<<" in "<<endl;
  //dout<<"start import "<< moduleName <<" symidToPairIndex size = "<< symidToPairIndex.size()<<endl;
  //checking moduleName spell
  if ( moduleTable.find( moduleName ) != moduleTable.end() ){ // already initialized
    //cerr<<"GetModule::Module: "<<moduleName<<" has already been got."<<endl;
    return -1;
  }
  ////// load the share library of the module
  map<string,void*>::iterator iter = libHandlerTable.find(moduleName);
  void *lib_Handler = NULL;
  bool module_find = false;
  string module_dir = "";
  if ( iter == libHandlerTable.end() ){
    // load the shared lib into the kernel from different directory
    // self own modules
    string current_dir = GetDirectory();
    string pond_root = GetEnv("POND_ROOT");
    string pond_home = GetEnv("POND_HOME");
    if ( not CheckFileType(pond_home,"DIR" ) ) {
      if ( FileExistQ(pond_home) ) System("\\rm -rf "+pond_home);
      System("\\mkdir "+pond_home);
    }
    string temp_dir = pond_home + "/.temp";
    if ( not CheckFileType(temp_dir,"DIR" ) ) {
      if ( FileExistQ(temp_dir) ) System("\\rm -rf "+temp_dir);
      System("\\mkdir "+temp_dir);
    }
    if ( pond_home == "" || pond_root == "" )
      ThrowError("Pond Environment variables are not properly set");
    string libname="lib"+moduleName+"Module.so", devlibname="lib"+moduleName+"Module-dev.so",so_file,temp_so_file;
    string culibname="lib"+moduleName+"Module_cuda.so",cudevlibname="lib"+moduleName+"Module_cuda-dev.so";
    bool nvcc_status = pond::CommandExist("nvcc");

#define TRY_LOAD_SO_IN(_module_dir,libname)                             \
    if ( not lib_Handler and CheckFileType(_module_dir,"DIR") ){        \
      module_find = true;                                               \
      so_file =  _module_dir+"/"+libname;                               \
      temp_so_file = temp_dir+"/"+libname;                              \
      if ( FileExistQ(so_file) ){                                       \
        if ( !FileExistQ(temp_so_file) ||                               \
             GetFileMD5(so_file) != GetFileMD5(temp_so_file) )          \
          System("rmcp "+so_file+" "+temp_dir );                        \
        lib_Handler=dlopen(temp_so_file.c_str(),RTLD_NOW|RTLD_GLOBAL ); \
        if ( !lib_Handler ) {                                           \
          if ( !this->DebugMode && silent ) return 1;                   \
          zhErroring("GetModule", "未能成功载入模块 '"+moduleName+"' 的二进制包")|| \
            Erroring("GetModule","Fail to get binary '" + moduleName+"' in."); \
          const char* dlsym_error=dlerror();                            \
          Warning("dlsym",((string)"dlsym_error::"+dlsym_error).c_str() ); \
          return -1;                                                    \
        }                                                               \
        module_dir = _module_dir;                                       \
        break;                                                          \
      }                                                                 \
    }
    for(;;){
      if ( DebugMode ){
        if ( nvcc_status ){
          TRY_LOAD_SO_IN(current_dir+"/",cudevlibname);
          TRY_LOAD_SO_IN(pond_home+"/"+moduleName,cudevlibname);
          TRY_LOAD_SO_IN(pond_root+"/modules/"+moduleName,cudevlibname);
          TRY_LOAD_SO_IN(pond_root+"/lib",cudevlibname);

          TRY_LOAD_SO_IN(current_dir+"/",culibname);
          TRY_LOAD_SO_IN(pond_home+"/"+moduleName,culibname);
          TRY_LOAD_SO_IN(pond_root+"/modules/"+moduleName,culibname);
          TRY_LOAD_SO_IN(pond_root+"/lib",culibname);
        }
        TRY_LOAD_SO_IN(current_dir+"/",devlibname);
        TRY_LOAD_SO_IN(pond_home+"/"+moduleName,devlibname);
        TRY_LOAD_SO_IN(pond_root+"/modules/"+moduleName,devlibname);
        TRY_LOAD_SO_IN(pond_root+"/lib",devlibname);

        TRY_LOAD_SO_IN(current_dir+"/",libname);
        TRY_LOAD_SO_IN(pond_home+"/"+moduleName,libname);
        TRY_LOAD_SO_IN(pond_root+"/modules/"+moduleName,libname);
        TRY_LOAD_SO_IN(pond_root+"/lib",libname);
      }else{
        if ( nvcc_status ){
          TRY_LOAD_SO_IN(current_dir+"/",culibname);
          TRY_LOAD_SO_IN(pond_home+"/"+moduleName,culibname);
          TRY_LOAD_SO_IN(pond_root+"/modules/"+moduleName,culibname);
          TRY_LOAD_SO_IN(pond_root+"/lib",culibname);

          TRY_LOAD_SO_IN(current_dir+"/",cudevlibname);
          TRY_LOAD_SO_IN(pond_home+"/"+moduleName,cudevlibname);
          TRY_LOAD_SO_IN(pond_root+"/modules/"+moduleName,cudevlibname);
          TRY_LOAD_SO_IN(pond_root+"/lib",cudevlibname);
        }
        TRY_LOAD_SO_IN(current_dir+"/",libname);
        TRY_LOAD_SO_IN(pond_home+"/"+moduleName,libname);
        TRY_LOAD_SO_IN(pond_root+"/modules/"+moduleName,libname);
        TRY_LOAD_SO_IN(pond_root+"/lib",libname);

        
        TRY_LOAD_SO_IN(current_dir+"/",devlibname);
        TRY_LOAD_SO_IN(pond_home+"/"+moduleName,devlibname);
        TRY_LOAD_SO_IN(pond_root+"/modules/"+moduleName,devlibname);
        TRY_LOAD_SO_IN(pond_root+"/lib",devlibname);
      }
      break;
    }
#undef TRY_LOAD_SO_IN
    // module not find
    if (not lib_Handler) {
      if ( not module_find ){
        Warning("GetModule","'"+moduleName+"' located in '"+module_dir+"' may not not load correctly.");
      }
      return -1;
    }
    //dout<<"module handler get"<<moduleName<<","<<lib_Handler<<endl;
    libHandlerTable[moduleName] = lib_Handler; // store handle for dlclose
  }else{
    //cout<<" already exists in libHandlerTable"<<endl;
    lib_Handler = iter->second;
  }
  /////////////////////////////////////////////////////////////////////
  ///// new create and destroy handle
  /////////////////////////////////////////////////////////////////////
  type_create* __create = (type_create*)dlsym(lib_Handler,("__create_"+moduleName+"Module").c_str() );
  const char* dlsym_error=dlerror();
  if ( __create == NULL || dlsym_error ){
    zhErroring("GetModule", "模块 '"+moduleName+"' 的初始化宏未找到. 请确保代码使用了正确的接口范式.") ||
      Erroring("GetModule", "Macro to init module '"+moduleName+"' not found. Please make sure you are following the standard template.");
    return -1;
  }
  type_destroy*__destroy = (type_destroy*)dlsym(lib_Handler,("__destroy_"+moduleName+"Module").c_str() );
  dlsym_error=dlerror();
  if ( __destroy == NULL || dlsym_error ){
    zhErroring("GetModule", "模块 '"+moduleName+"' 的卸载宏未找到. 请确保代码使用了正确的接口范式.") ||
      Erroring("GetModule", "Macro to delete '"+moduleName+"' not found. Please make sure you are following the standard tempalte.");
    return -1;
  }
  moduleCreateHandleTable[moduleName]=__create;
  moduleDestroyHandleTable[moduleName]=__destroy;//for destroy
  /////////////////////////////////////////////////////////////////////
  ///// Create module
  // registering to eva will happen once the new module being constructed.
  Module * new_module = __create();
  if ( new_module == NULL ){
    zhErroring("GetModule"," 创建模块时遇到错误.") ||
      Erroring("GetModule"," Error while new module.");
    return -1;
  }
  // register functions created by DefineFunction(func)
  type_return_functions *__return_functions = (type_return_functions*)dlsym(lib_Handler,("__return_functions_"+moduleName).c_str() );
  dlsym_error=dlerror();

  //dout<<"before import "<< moduleName <<" symidToPairIndex size = "<< symidToPairIndex.size()<<endl;
  if ( __return_functions != NULL ){ // write in template style,handle registered functions
    type_regsterfunction*regfunc;
    Index objid(__return_functions() );
    //dout<<"return function objid = "<<objid<<endl;
    Object  funclist( objid )  ;
    //dout<<"return functions for "<<moduleName<<"= "<<funclist<<endl;
    if ( funclist.ListQ() ){
      //dout<<" dealing functions for module "<<moduleName <<endl;
      for (u_int i=1;i<=funclist.Size();i++){
        string func = funclist[i].Key();
        //dout<<"dealing func "<<func<<endl;
        regfunc = (type_regsterfunction*)dlsym(lib_Handler, (func+"_Eva_Register_"+moduleName).c_str() );
        //dout<<(void*)regfunc << " - "<<new_module  << " : "<<  func << endl;
        dlsym_error=dlerror();
        if ( regfunc == NULL || dlsym_error ){
          zhWarning("GetModule","未能成功注册函数 '"+ func+ "'. 请检查代码是否符合标准范式.")||
            Warning("GetModule","Failed to register function "+ func+ ". Please check if your code follows the standard template.");
        } else {
          // register function by handler defined by mactor in pd_module.h
          // it should be noted that sysIdToPairIndex should be updated properly
          regfunc(new_module);
        }
      }
    }
    //dout<<"function processing done"<<endl;

    //dout<<"after import "<< moduleName <<" symidToPairIndex size = "<< symidToPairIndex.size()<<endl;
  }
  dlsym_error=dlerror();
  if ( dlsym_error ){
    zhWarning("dlsym",((string)"GeModule的最后出现dl错误:"+dlsym_error).c_str() ) ||
      Warning("dlsym",((string)"error at end of GetLib:"+dlsym_error).c_str() );
  }
  return 1;
}

int Evaluation::ImportRequest(Object&ARGV){
  //dout<<"try import "<<ARGV<<" in depth "<<EvaKernel->EvaluationDepth <<endl;
  for (int i=1; i<= ARGV.Size(); i++ ){
    if ( ARGV[i].StringQ() or ARGV[i].SymbolQ() ){
      //dout<<" try deal "<<ARGV[i]<<endl;
      // EvaKernel->GetModule( ARGV[i].Key() );
    }else if ( ARGV[i].ListQ() ){
      EvaKernel->ImportRequest( ARGV[i] );
    }
  }
  ReturnNull;
}

int Evaluation::ImportLoad(Object&ARGV){
  //dout<<"try import "<<ARGV<<" in depth "<<EvaKernel->EvaluationDepth <<endl;
  for (int i=1; i<= ARGV.Size(); i++ ){
    if ( ARGV[i].StringQ() or ARGV[i].SymbolQ() ){
      //dout<<" try deal "<<ARGV[i]<<endl;
      EvaKernel->GetModule( ARGV[i].Key() );
    }else if ( ARGV[i].ListQ() ){
      EvaKernel->ImportLoad( ARGV[i] );
    }
  }
  ReturnNull;
}

int Evaluation::PD_Import(Object&ARGV){
  //deout<<" depth addr "<< &(EvaKernel->EvaluationDepth) <<endl;

  // check if slient option present
  static INIT_SYMID_OF(silent);
  bool is_silent = false;
  for (int i=1; i<=ARGV.Size(); i++ ){
    //dout<<"check "<<ARGV[i]<<","<<ARGV[i].ListQ(SYMID_OF_Set) <<endl;
    if ( ARGV[i].ListQ(SYMID_OF_Set) && ARGV[i].Size() == 2  ){
      if ( ARGV[i][1].SymbolQ(SYMID_OF_silent) ){
        is_silent = ARGV[i][2].Boolean();
      }
    }
  }
  //dout<<"is slient load:"<< is_silent <<endl;

  for (int i=1; i<= ARGV.Size(); i++ ){
    if ( ARGV[i].StringQ() or ARGV[i].SymbolQ() ){
      //dout<<"try load "<< ARGV[i] << is_silent <<endl;
      EvaKernel->GetModule( ARGV[i].Key(), is_silent );
    }else if ( ARGV[i].ListQ(SYMID_OF_as) ){
      // import with as, collectting in a namespace(a variable)
      //dout<<"try import "<<ARGV[i][1]<<" as "<<ARGV[i][2]<<endl;
      Object module_context_pair;
      // EvaKernel->newContext( vartable, patternTable );
      //dout<<"load module in a context with keyword 'as'"<<endl;
    } else if ( ARGV[i].ListQ() && not ARGV[i].ListQ(SYMID_OF_Set)  ){
      EvaKernel->PD_Import( ARGV[i] );
    }
  }
  ReturnNull;
}

int Evaluation::PD_Remove(Object&ARGV){
  for ( auto i=1; i<= ARGV.Size(); i++ ){
    if ( ARGV[i].StringQ() or ARGV[i].SymbolQ() ){
      EvaKernel->RemoveModule( ARGV[i].Key() );
    }else if ( ARGV[i].ListQ(SYMID_OF_List) ){
      EvaKernel->PD_Remove( ARGV[i] );
    }
  }
  ReturnNull;
}
int Evaluation::PD_ReImport(Object&ARGV){
  for ( auto i=1; i<= ARGV.Size(); i++ ){
    if ( ARGV[i].StringQ() or ARGV[i].SymbolQ() ){
      EvaKernel->RemoveModule( ARGV[i].Key() );
      EvaKernel->GetModule( ARGV[i].Key() );
    }else if ( ARGV[i].ListQ(SYMID_OF_List) ){
      EvaKernel->PD_ReImport( ARGV[i] );
    }
  }
  ReturnNull;
}

// EMSCRIPTEN_KEEPALIVE
// int Evaluation::PD_RemoveModule(Object&ARGV){
//   for ( auto i=1; i<= ARGV.Size(); i++ ){
//     CheckShouldBeString(i);
//     if ( EvaKernel->RemoveModule(ARGV[i].Key()) < 0 ) ReturnError;
//   }
//   ReturnNull;
// }

int Evaluation::GetModuleDependence(string moduleName){
  string module_dir;
  string depend_file;
  string content;
  vector<string> words;
  module_dir =  GetEnv("POND_HOME")+"/"+moduleName;
  if ( CheckFileType(module_dir,"DIR") ){
    depend_file =  module_dir + "/.requirements";
    if ( CheckFileType( depend_file,"REG" ) ){
      content = pond::GetFileContent( depend_file );
      pond::StringSplit( content, words);
      for ( u_int i=0; i< words.size(); i++ ){
        GetModule( words[i] );
      }
    }
  }
  module_dir =  GetEnv("POND_ROOT")+"/modules/"+moduleName;
  if ( CheckFileType(module_dir,"DIR") ){
    depend_file =  module_dir + "/.requirements";
    if ( CheckFileType( depend_file,"REG" ) ){
      content = pond::GetFileContent( depend_file );
      pond::StringSplit( content, words);
      for ( u_int i=0; i< words.size(); i++ ){
        GetModule( words[i] );
      }
    }
  }
  return 0;
}

int Evaluation::GetModulePyPathReady(string moduleName){
  //Matlab path
#define __ADD_PATH(dir) {                         \
    if( FileExistQ(dir) ) {                       \
      Object  expr; expr.SetList( "PyAddPath" ); \
      expr.PushBackString( (dir).c_str() );       \
      Evaluate( expr );                           \
    }                                             \
  }
  __ADD_PATH( GetEnv("POND_ROOT")+"/modules/"+moduleName );
  __ADD_PATH( GetEnv("POND_HOME")+"/"+moduleName );
#undef __ADD_PATH
  return 0;
}

int Evaluation::GetModulePondScript(string moduleName){
  string sysdir,filename;
  Object tmp;
#define __GET_PATH(dir) {                                   \
    filename=dir+"/"+moduleName+".pd";                      \
    if ( FileExistQ(filename) ) {                           \
      EvaluateString(GetFileContent(filename),tmp );        \
      return 0;                                             \
    }                                                       \
  }
  __GET_PATH( GetCwd() );
  __GET_PATH( GetEnv("POND_HOME")+"/"+moduleName );
  __GET_PATH( GetEnv("POND_ROOT")+"/modules/"+moduleName );
  __GET_PATH( GetEnv("POND_ROOT")+"/lib/"+moduleName );
#undef __GET_PATH
  return -1;
}

EMSCRIPTEN_KEEPALIVE
int Evaluation::GetModule(string moduleName,bool silent){ //fetch resources from remote server and load into
  if ( moduleName.size() == 0 )
    return -1;

  //dout<<"try get module "<<moduleName <<" into system"<<endl;
  // check if already been imported. if imported just ignore
  auto itermodule=moduleTable.find( moduleName );
  if ( itermodule != moduleTable.end() ){
    //cerr<<"GetModule::Module: "<<moduleName<<" has been imported."<<endl;
    return -1;
  }
  //dout<<"prepare send signal"<<endl;
  try{
    bool success = false;
    if ( moduleName[0] < 'A' or moduleName[0] > 'Z' ){
      GetModuleDependence(moduleName);
      // GetModulePathReady(moduleName);
      // GetModuleMatPathReady(moduleName);
      GetModulePyPathReady(moduleName);
      success  = (GetModulePondScript(moduleName)>=0) || success;
    }
    success  = (GetModuleLib(moduleName, silent)>=0) || success;
    if ( not success && not silent ){
      zhWarning("GetModule","模块'"+moduleName+"' 未找到") ||
        Warning("GetModule","Module '"+moduleName+"' not found.") ;
      return -1;
    }
  }catch ( const Error&err ){
    cerr<<err.swhat()<<endl;
  }catch ( const exception &err){
    zhErroring("源代码",err.what() )||
      Erroring("Sourcecode",err.what() );
  }catch ( ... ){
    zhErroring("系统","出现未预料到的错误." )||
      Erroring("System","Unexpected error occured." );
  }
  return 0;
}

EMSCRIPTEN_KEEPALIVE
int Evaluation::RemoveModule(string moduleName){
  //moduleTable
  map<string,Module*>::iterator itermodule;
  itermodule=moduleTable.find( moduleName );
  // already initialized
  if ( itermodule == moduleTable.end() ){
    // do nothing if the module is not imported
    // Warning("RemoveModule","Module "+moduleName+" has not been imported yet.");
    return -1;
  }
  //moduleDestroyHandleTable
  map<string,type_destroy*>::iterator iterdestroy;
  iterdestroy = moduleDestroyHandleTable.find(moduleName);
  if ( iterdestroy != moduleDestroyHandleTable.end() ){ // already initialized
    iterdestroy->second( itermodule->second );
    moduleDestroyHandleTable.erase(iterdestroy);
  }
  map<string,void*>::iterator iterlibhandle;
  iterlibhandle = libHandlerTable.find(moduleName);
  if ( iterlibhandle != libHandlerTable.end() ){
    dlclose( iterlibhandle->second );
    libHandlerTable.erase(iterlibhandle);
  }
  moduleTable.erase(itermodule);
  return 0;
}

Object Evaluation::newContext(){
  //cerr<<"new Grammer Module"<<endl;
  valueTables.push_back( new ValueTable() );
  currentValueTable = (*valueTables.rbegin());
  return currentValueTable->pairTable;
}

Object Evaluation::newContext(ValueTable*vt){
  //vt->upValueTable =  (*valueTables.rbegin()).first;
  valueTables.push_back( vt );
  currentValueTable = *valueTables.rbegin();
  return currentValueTable->pairTable;
}

Object Evaluation::newContext(Object&pairT){
  ValueTable* vt = new ValueTable( pairT);
  valueTables.push_back( vt );
  currentValueTable = (*valueTables.rbegin());
  return currentValueTable->pairTable;
}

Object Evaluation::newContext(Object&pairT, Object&patternT){
  ValueTable* vt = new ValueTable( pairT, patternT );
  valueTables.push_back( vt );
  currentValueTable = (*valueTables.rbegin());
  return currentValueTable->pairTable;
}

int Evaluation::deleteContext(){
  //cerr<<"delete Grammer Module"<<endl;
  if ( valueTables.size() > 1 ){
    if ( (*valueTables.rbegin()) != NULL ){
      delete (*valueTables.rbegin());
    }
    valueTables.pop_back();
    currentValueTable = (*valueTables.rbegin());
  }else{
    Erroring("Evaluation","Top Grammar Unit reaches, fail to deleteGrammerModule");
    return -1;
  }
  return 0;
}

int Evaluation::ClearValueTablesTo(u_int N){
  //cerr<<"clear tabke to "<<N<<" current depth:"<< valueTables.size()  <<endl;
  if ( N < 1 ) { Erroring("ClearValueTableTo","Argument N is smaller than 1."); return -1; }
  while ( valueTables.size() > N ){
    if ( (*valueTables.rbegin()) != NULL ){
      delete (*valueTables.rbegin());
    }
    valueTables.pop_back();
  }
  currentValueTable = (*valueTables.rbegin());
  return 0;
}

int Evaluation::PreEvaluate(Object&list){//prebuild value table
  if ( list.NullQ() ) return 0;
  switch ( list.type() ){ 
  case __String__:
  case __Number__: break;
  case __Symbol__:{
    Object pair = GetOrNewValuePairRef(list);
    if ( not pair.NullQ() ){
      list.set_idx( pair.objid );
    }
    break;
  }
  case __List__:
    //Function  Module Compound and so on will no go deep 
    for ( auto iter = list.ElementsBegin() ; iter != list.ElementsEnd(); iter++){
      PreEvaluate( *iter);
    }
    break;
  }
  return 0;
}


void Evaluation::EvaluateHead(Object&list,bool isHold){
  Evaluate( list[0], isHold );
}

void Evaluation::EvaluateFirst(Object&list,bool isHold){
  if ( list.Size() < 1) return;
  Evaluate( list[1], isHold );
}

void Evaluation::EvaluateRest(Object&list,bool isHold){
  if ( list.Size() < 2 ) return;
  for ( auto iter = list.Begin()+1;  iter!=list.End(); iter++){
    Evaluate( *iter, isHold );
  }
}

void Evaluation::EvaluateAll(Object&list,bool isHold){
  for ( auto iter = list.Begin(); iter!=list.End(); iter++){
    Evaluate( *iter, isHold );
  }
}

int Evaluation::ListableEvaluate(Object &ARGV,bool isHold){
  if ( ARGV.Size() == 0 ) return 0;
  //Check if condition
  u_int size = 0;
  for ( auto iter = ARGV.Begin(); iter != ARGV.End(); iter++ ){
    if ( iter->ListQ(SYMID_OF_List) ){
      if ( size == 0 ){
        size = (*iter).Size();
      }
      else{
        if ( iter->Size() != size ) {
          Erroring( string( ARGV.Key() )+"::shape","Try to apply listable attribute, but the shape does not match.");
          return 0;
        }
      }
    }
  }
  if ( size == 0 ) return 0;
  //condition met
  Object obj; obj.SetList();
  for ( u_int i=1;i<=size;i++){
    Object chobj;
    chobj.SetList( ARGV[0].ids() );
    chobj[0].set_idx( ARGV[0].idx() );
    for ( auto iter = ARGV.Begin(); iter != ARGV.End(); iter++ ){
      if ( iter->ListQ(SYMID_OF_List) ){
        chobj.PushBackRef( (*iter)[i] );
      }else{
        chobj.PushBackRef( (*iter) );
      }
    }
    Evaluate( chobj );
    obj.PushBackRef( chobj );
  }
  ARGV = obj;
  return 1;
}

// return 1 means already processed someway, no more operation needed
int Evaluation::AttributeProcessing(Object&ARGV,bool *attri,bool isHold,bool ofDelay){
  if ( not ARGV.ListQ() or ARGV.Size() == 0 )
    return 0;
  if ( attri == NULL ){
    ARGV.FlattenSequence();
    EvaluateAll(ARGV,isHold);
    return 0;
  }
  //DelayFunction like Function PureFunction, will block its content automatically
  if ( pond::AttributeQ(attri,AttributeType::DelayFunction) and not ofDelay ){
    //dout<<"is dealy function when ARGV = "<<ARGV<<endl;
    return 1;
  }
  // flat and sequence
  if ( pond::AttributeQ(attri,AttributeType::Flat) ){
    if ( pond::AttributeQ(attri,AttributeType::SequenceHold) )
      ARGV.Flatten();
    else
      ARGV.FlattenListAndSequence();
  }else{
    if ( not pond::AttributeQ(attri,AttributeType::SequenceHold) )
      ARGV.FlattenSequence();
  }
  // Listable
  if ( pond::AttributeQ(attri,AttributeType::Listable) ){
    if ( ListableEvaluate( ARGV )>0) return 1;
  }
  // Hold Some
  if ( not pond::AttributeQ(attri,AttributeType::HoldAll) ){
    if ( not pond::AttributeQ(attri,AttributeType::HoldFirst) ){
      EvaluateFirst(ARGV,isHold);
    }
    if ( not pond::AttributeQ(attri,AttributeType::HoldRest) ){
      EvaluateRest(ARGV,isHold);
    }
  }//HoldAll

  // Orderless
  if ( ARGV.Size()>1 && pond::AttributeQ(attri,AttributeType::Orderless) ){
    ARGV.ExpressionSort();
  }
  return 0;
}




// return value and its meaning
// 0 nothing done ( hold the same )
// 1 normal return
// -1 some error occur, but nothing matter

#define EvaReturn(expr) {                       \
    int res = expr;                             \
    EvaluationDepth--;                          \
    return res;                                 \
  }

int Evaluation::Evaluate(Object&ARGV){
  return this->Evaluate(ARGV, false, false);
}
int Evaluation::Evaluate(Object&ARGV, bool isHold){
  return this->Evaluate(ARGV, isHold, false);
}
int Evaluation::Evaluate(Object&ARGV, bool isHold , bool isRef){
  //dout<<"try evaluate "<<ARGV<<" depth ="<<EvaluationDepth<<endl;
  //dout<<" depth addr "<< &(EvaKernel->EvaluationDepth) <<endl;
  if ( ARGV.NullQ() ) return 0;
  EvaluationDepth++;
  //dout<<"evaluate "<<ARGV << " in depth "<< EvaluationDepth<<endl;
  if( EvaluationDepth > EvaKernel->maxRecursionDepth ) {
    zhErroring("求值", "递归深度超过了最大允许值 "+ToString( (int)maxRecursionDepth )+".")||
      Erroring("Evaluate", "Recursion depth exceeds "+ToString( (int)maxRecursionDepth )+".");
    EvaReturn(0);
  }
  if ( isHold ){
    EvaReturn(0);
  }
  switch ( ARGV.type() ){
  case ObjectType::Number: case ObjectType::String:
    EvaReturn(0);
  case ObjectType::Symbol:{
    if ( ARGV.ids() == 0 ) EvaReturn(0);
    // if ( ARGV.ids() == SYMID_OF_Exit || ARGV.ids() == SYMID_OF_exit ){
    //   return PD_exit( ARGV );
    // }
    // special ones like $  just return
    if ( ARGV.ids() == SYMID_OF___Variable or
         ARGV.ids() == SYMID_OF_FunctionVariable or
         ARGV.ids() == SYMID_OF_SerialCode ) 
      EvaReturn(0);

    if ( ARGV.ValuedSymbolQ() ){ // a valued symbol
      //if(flag)dout<<"valued symbol: "<<ARGV<<endl;
      //dout<<"eval valued symboled "<< ARGV<<endl;
      Object pair_value( ARGV.idx() );
      //if(flag)dout<<"pair value: "<<pair_value<<endl;
      //dprintf("valued symbol %s paired value: %s", ARGV.ToString().c_str(), pair_value.ToString().c_str());
      if ( not pair_value[2].SymbolQ( ARGV.ids() ) ){ // normal valued symbol
        if ( isRef ){
          ARGV =  pair_value[2] ;
        }else{
          ARGV.CopyObject( pair_value[2] );
        }
        EvaReturn( Evaluate(ARGV) );
      }else{ // already a function pair value
        EvaReturn( 5 );
      }
    }else{ //for a normal Symbol
      // treated as a normal symbol
      // a single sysmbol can not be an pattern, so get ref is ok
      //dout<<"deal normal symbol:"<<ARGV<<endl;
      Object pair_value = GetValuePairRef( ARGV ); 
      //dout<<"pair_value = "<< pair_value <<endl;
      if ( not pair_value.NullQ() ){
        if ( pair_value[2].SymbolQ( ARGV.ids() ) ){ // is a registered function value pair
          ARGV.set_idx( pair_value.objid );
          EvaReturn( 5 );
        }else{ // a normal value pair
          if ( isRef ){
            ARGV =  pair_value[2] ;
          }else{
            ARGV.CopyObject( pair_value[2] );
          }
          EvaReturn( Evaluate(ARGV) );
        }
      }else{
        EvaReturn(0);//Nothing need to do more
      }
    }
  }
  case ObjectType::List:
    //dout<<" try deal this ARGV "<<ARGV<<endl;
    if ( ARGV.SimpleListQ() ){
      u_int id = ARGV[0].SymbolId(); 
      if ( id >= symidToPairIndex.size() ){
        Evaluate( ARGV[0] );
      }else{
        Index tind = symidToPairIndex[ id ] ;
        //dout<<"meet symid To Pair table for "<< ARGV[0] << " tind = "<< tind << " id = "<<id<< endl;
        ARGV[0].set_idx( tind );
      }
    }else{
      Evaluate( ARGV[0] );
    }
    if ( ARGV[0].ValuedSymbolQ() ){// is a unChanged Symbol or something with EvaRecordTable id
      //dout<<"eval valued symboled head "<< ARGV<<endl;
      //dout<<"head has something when eval "<<ARGV.ToString().c_str()<<endl;
      Object valuepair( ARGV[0].idx() );
      int recId = valuepair.ids();
      EvaRecord *rec = NULL;
      if ( recId > 0 and recId < evaRecordTable.size() )
        rec = evaRecordTable[ recId ];
      //dout<<"the rec for "<<ARGV<<" is "<<(void*)rec<<endl;
      if ( rec != NULL ){
        //cerr<<"Attribute processing for "<< ARGV <<endl;
        if ( AttributeProcessing(ARGV,rec->attributes) > 0 ) EvaReturn(1);
        if ( rec->staticFunction ) EvaReturn( (*rec->staticFunction)(ARGV) );
        if ( rec->module and rec->memberFunction ) EvaReturn( ((rec->module)->*(rec->memberFunction))(ARGV) );
        // if ( rec->isCallable() ) EvaReturnAfter( Call( rec, ARGV ) );
      }else{
        AttributeProcessing(ARGV,NULL);
      }
      //dprintf("nothing done when eval %s afterall its head",ARGV.ToString().c_str());
    }else{
      // after Processing ARGV will surely be a list.
      if ( ARGV[0].ListQ() and ARGV[0][0].ValuedSymbolQ() ){
        //dout<<"eval valued symboled delay head "<< ARGV<<endl;
        Object valuepair( ARGV[0][0].idx() );
        int recId = valuepair.ids();
        //dout<<"get recId "<<recId<<" for "<<ARGV[0][0]<<endl;
        EvaRecord *rec = NULL;
        if ( recId > 0 and recId < evaRecordTable.size() ){
          rec = evaRecordTable[ recId ];
        }
        //dout<<"get rec "<<rec<<endl;

        if ( rec != NULL ){
          if ( pond::AttributeQ(rec->attributes, AttributeType::DelayFunction) ){
            AttributeProcessing( ARGV, rec->attributes,false,true);
            if ( rec->staticFunction ){
              EvaReturn((*rec->staticFunction)(ARGV) );
            }
            if ( rec->module and rec->memberFunction ){
              EvaReturn(((rec->module)->*(rec->memberFunction))(ARGV));
            }
          }
        }
      }else{
        AttributeProcessing(ARGV,NULL);
      }
    }
    Object nobj;
    if ( GetValueRef( ARGV,nobj ) > 0 ){//get a new Value
      if ( ARGV != nobj ){
        if ( isRef ){
          ARGV = nobj;
        }else{
          ARGV.CopyObject(nobj);
        }
        EvaReturn( Evaluate(ARGV) );
      }else{
        EvaReturn(0);
      }
    }
    // from pattern table
    //dprintf("Try get %s from pattern.",ARGV.ToString().c_str() );
    if ( GetValue(ARGV,nobj,true) == 0 ){
      //dprintf("no any value for %s.",ARGV.ToString().c_str() );
      EvaReturn(0);
    }
    ARGV =  nobj ;
    EvaReturn( Evaluate( ARGV ) );
  }
  EvaReturn(0);
}
#undef EvaReturn
//#undef EvaReturnSimplify

///////////////////////////////////////////////////////////
Object Evaluation::EvaluateExpression(const string&expr){
  Object obj = ImportList::ToExpression(expr);
  Evaluate(obj);
  return obj;
}

int Evaluation::EvaluateContinue(const string&excont){
  return 0;
}

int Evaluation::EvaluateString(const string&cont, Object&result){
  return this->EvaluateString(cont, result, 0, false);
}
int Evaluation::EvaluateString(const string&cont, Object&result, const int depth, const bool noprint){
  // static INIT_SYMID_OF( IMPORT );
  ////////////////////////////////////////////////////
  EvaKernel->statusCode = 0;
  EvaKernel->statusObject.SetVoid();
  ImportList importList( cont );
  Object superlist(__List__,SYMID_OF_List);
  Object list;
  static INIT_SYMID_OF( Import );
  while ( !importList.End() ){
    // result = 0;
    if ( importList.GetList( superlist, 0) < 0 ){
      break; //get list
    }
    //dout<< "deal "<<superlist<<endl;
    if ( superlist.Size() > 0 ){
      list = superlist.Back();
      superlist.PopBack();
      if ( not list.NullQ() ){
        if ( list.ListQ( SYMID_OF_import ) or
             // list.ListQ( SYMID_OF_IMPORT ) or
             list.ListQ( SYMID_OF_Import ) ){
          //dout<<"is a import clause"<<endl;
          EvaKernel->PD_Import( list );
          continue;
        }
        while( list.ListQ(SYMID_OF_CompoundExpression) and list.Size() > 1 and (
                 list[1].ListQ( SYMID_OF_import ) or
                 list[1].ListQ( SYMID_OF_Import ) ) ){
          EvaKernel->PD_Import( list[1] );
          list.Delete(1);
        }
        EvaKernel->Evaluate(list);
        static EvaRecord *sprec= NULL;
        if ( sprec == NULL ){
          sprec = GetEvaRecord( "SimpleSimplify" );
          if ( sprec == NULL ) {
            zhErroring("Kernel","未能成功从'System'模块中载入'SimpleSimplify'函数.")||
              Erroring("Kernel","'SimpleSimplify' from 'System' is not loaded properly."); 
            EvaKernel->Println( list.ToString() );
            break; 
          }
        }
        // Apply SimpleSimplify to result
        sprec->staticFunction( list );

        if ( EvaKernel->statusCode == KernelStatusQuit ) break;
        if ( EvaKernel->statusCode == KernelStatusErrorThrow ) break;
        if ( ! list.NullQ() && ! noprint ){
          EvaKernel->Println( list.ToString() );
        }
        continue;
      }
    }
  }
  result = list;
  return 0;
}

int Evaluation::EvaluateFile(const std::string filename, Object&result){
  return this->EvaluateFile(filename, result, 0, false );
}
int Evaluation::EvaluateFile(const std::string filename, Object&result, int depth, bool noprint){
  ifstream ifs( filename.c_str() , ios::in );
  if ( !ifs ){
    Warning("Get","file "+filename+" cannot be opened.");
    return -1;
  }
  istreambuf_iterator<char> beg(ifs),end;
  string cont(beg,end);
  ifs.close();
  ////////////////////////////////////////////////////
  return this->EvaluateString(cont, result, depth, noprint);
};

////////////////////////////////////////////////////////
// 0 not find, -1 protected and found, 1 unProtected and found
int Evaluation::GetValueRef(const Object &obj,Object&ref){
  switch ( obj.type() ){
  case ObjectType::Number: case ObjectType::String:
    ref.SetVoid();
    return 0;
  case ObjectType::Symbol:
    for ( auto iter = valueTables.rbegin(); iter!= valueTables.rend(); iter++){
      int res =  (*iter)->GetValueRef(obj,ref);
      if (  res > 0 ) return 1;
      else if ( res < 0 ) return -1;
    }
    ref.SetVoid();
    return 0;
  case ObjectType::List: 
    if ( obj(0).SymbolQ( SYMID_OF_Part ) ){
      ref.CopyObject(obj);
      static StaticFunction func_part = NULL;
      if ( func_part == NULL ){
        EvaRecord *rec = GetEvaRecord( "Part" ); 
        if ( rec == NULL ) {
          Erroring("EvaKernel","Function 'Part' is not loaded properly.");
          return 0;
        }
        func_part  =  rec->staticFunction;
      }
      (*func_part)(ref);
      return 1;
    }
    for ( auto iter = valueTables.rbegin(); iter!= valueTables.rend(); iter++){
      int res =  (*iter)->GetValueRef(obj,ref);
      if (  res > 0 ) return 1;
      else if ( res < 0 ) return -1;
    }
    ref.SetVoid();
    return 0;
  }
  Erroring("GetValueRef","Switch error.");
  return 0;
}

//////////////////////////////////////////////////
int Evaluation::GetValue(const Object &list,Object &value,bool onlyCheckPatternTable){
  switch (list.type()){
  case ObjectType::Number: case ObjectType::String:
    return 0;
  case ObjectType::Symbol: case ObjectType::List: 
    for ( auto iter = valueTables.rbegin(); iter!= valueTables.rend(); iter++){
      return_if_not( (*iter)->GetValue(list,value, onlyCheckPatternTable) , 0);
    }
    return 0;
  }
  { Erroring("GetValue","swtich error"); return 0; }
};

// int Evaluation::GetValue(Object &list){
//   return GetValue( list,list);
// };

bool Evaluation::GetValue(string key,string&str){//??????
  Object  list; list.SetSymbol( key.c_str() );
  Object  value = GetValuePairRef( list );
  if ( value.NullQ() or value[1] == value[2] ){return false;}
  if ( value[2].StringQ() ){
    str = value[2].Key();
  }else{
    str = value[2].ToString();
  }
  return true;
}

bool Evaluation::GetValue(string key,double&num){
  Object  list; list.SetSymbol( key.c_str() );
  Object  value = GetValuePairRef( list );
  if ( value.NullQ() or value[1] == value[2] ){return false;}
  if ( !value[2].NumberQ() ){return false;}
  num = value[2].Number();
  return true;
}

bool Evaluation::GetValue(string key,float&num){
  Object  list; list.SetSymbol( key.c_str() );
  Object  value = GetValuePairRef( list );
  if ( value.NullQ() or value[1] == value[2] ){return false;}
  if ( !value[2].NumberQ() ){return false;}
  num = value[2].Number();
  return true;
}

bool Evaluation::GetValue(string key,int&num){
  Object  list; list.SetSymbol( key.c_str() );
  Object  value = GetValuePairRef( list );
  if ( value.NullQ() or value[1] == value[2] ){return false;}
  if ( !value[2].NumberQ() ){return false;}
  num = value[2].Number();
  return true;
}

bool Evaluation::GetValue(string key,bool&num){
  Object  list; list.SetSymbol( key.c_str() );
  Object  value = GetValuePairRef( list );
  if ( value.NullQ() or value[1] == value[2] ){return false;}
  if ( !value[2].NumberQ() ){return false;}
  num = value[2].Number();
  return true;
}

// onlyGet == true
// onlyGet == false,
// return value is the pair Object from ValueTable
// get value somewhere or new value at top
Object Evaluation::GetOrNewValuePairRef(const Object&obj,bool onlyGet,bool isCheckedNoPattern){
  if ( obj.NumberQ() or obj.StringQ() )
    return NullObject;
  if ( obj.ValuedSymbolQ() ){//already a get value r
    return Object( obj.idx() );
  }
  for ( auto iter = valueTables.rbegin(); iter!= valueTables.rend(); iter++){
    Object res = (*iter)->GetOrNewValuePairRef(obj,true,isCheckedNoPattern) ;
    if ( not res.NullQ() )
      return res;
  }
  if ( onlyGet ) return NullObject;
  return (*valueTables.rbegin() )->GetOrNewValuePairRef(obj,false,isCheckedNoPattern);
}

Object Evaluation::GetOrNewBottomValuePairRef(const Object&obj, bool isCheckedNoPattern){
  if ( obj.NumberQ() or obj.StringQ() ) return NullObject;
  return (*valueTables.begin() )->GetOrNewValuePairRef(obj,false,isCheckedNoPattern);
}

Object Evaluation::GetOrNewValuePairRef(const char*sym_str){
  Object obj(ObjectType::Symbol, sym_str );
  return GetOrNewValuePairRef( obj );
}

Object Evaluation::GetValuePairRef(const Object &list,bool isCheckedNoPattern)
{
  return GetOrNewValuePairRef(list,true,isCheckedNoPattern);
};

// Object Evaluation::GetOrNewValuePairRef(Object &list,bool isCheckedNoPattern){
//   return GetOrNewValuePairRef(list,false,isCheckedNoPattern);
// };

//onlyUpdate
//atTop
Object Evaluation::InsertOrUpdatePairValue(const Object&left,  const Object&right,
                                       bool onlyUpdate, bool isCheckedPattern){
  if ( left.ValuedSymbolQ() ){
    Object valuepair( left.idx() );
    valuepair[2].CopyObject( right );
    return valuepair;
  }
  for ( auto iter = valueTables.rbegin(); iter != valueTables.rend(); iter++){
    Object result = (*iter)->UpdateOrInsert(left,right,true,isCheckedPattern);
    if ( not result.NullQ() ) return result;
  }
  if ( not onlyUpdate ){
    return (*valueTables.rbegin())->UpdateOrInsert(left,right,false,isCheckedPattern);
  }
  return NullObject;
}

Object Evaluation::RefInsertOrUpdatePairValue(const Object&left, const Object&right){
  if ( left.ValuedSymbolQ() ){
    Object valuepair( left.idx() );
    valuepair[2] = right;
    return valuepair;
  }
  for ( auto iter = valueTables.rbegin(); iter != valueTables.rend(); iter++){
    Object pairvalue = (*iter)->GetOrNewValuePairRef(left, true, true);
    if ( not pairvalue.NullQ() ){
      pairvalue[2] = right;
      return pairvalue;
    }
  }
  return (*valueTables.rbegin())->RefInsertOrUpdatePairValue(left,right);
}
                                     
Object Evaluation::InstantInsertOrUpdatePairValue(const Object &left,const Object &right){
  if ( left.ValuedSymbolQ() ){
    Object valuepair( left.idx() );
    valuepair[2].CopyObject( right );
    return valuepair;
  }
  return (*valueTables.rbegin())->UpdateOrInsert(left,right);
};


Object Evaluation::RefInstantInsertOrUpdatePairValue(const Object &left,const Object &right){
  if ( left.ValuedSymbolQ() ){
    Object valuepair( left.idx() );
    valuepair[2] =  right;
    return valuepair;
  }
  return (*valueTables.rbegin())->RefInsertOrUpdatePairValue(left,right);
};

Object Evaluation::InsertOrUpdateBottomPairValue(const Object &left,const Object &right){
  if ( left.ValuedSymbolQ() ){
    Object valuepair( left.idx() );
    valuepair[2].CopyObject( right );
    return valuepair;
  }
  return (*valueTables.begin())->UpdateOrInsert(left,right);
};

char Evaluation::Boolean(Object &list){
  Object tobj; tobj.CopyObject(list);
  Evaluate(tobj);
  return tobj.Boolean();
}

bool* Evaluation::SetAttribute(Object &list,AttributeType attriType,bool val){
  EvaRecord* rec = GetOrNewEvaRecord(list);
  rec->attributes[ (u_char)attriType] = val;
  return rec->attributes;
}

bool* Evaluation::SetAttribute(const char*key,AttributeType attriType,bool val){
  Object  list; list.SetSymbol(key);
  return SetAttribute(list,attriType,val);
}


bool* Evaluation::SetAttribute(const unsigned int id,AttributeType attriType,bool val){
  if ( id > 0 and id < evaRecordTable.size() ){
    EvaRecord*rec = evaRecordTable[id];
    if ( rec == NULL ) { Erroring("SetAttributes","Try to set attributes record which is deleted."); return NULL; }
    rec->attributes[(u_char)attriType] = val;
    return rec->attributes;
  }
  { Erroring("SetAttributes","Try to set attributes record out of range."); return NULL; }
}

bool *Evaluation::GetAttributes(const unsigned int id){
  if ( id > 0 and id < evaRecordTable.size() ){
    EvaRecord*rec = evaRecordTable[id];
    if ( rec == NULL ) { Erroring("GetAttributes","Try to get attributes record which is deleted."); return NULL; }
    return rec->attributes;
  }
  { Erroring("GetAttributes","Try to get attributes record out of range."); return NULL; }
}
bool *Evaluation::GetAttributes(Object &list){
  EvaRecord*rec = GetOrNewEvaRecord(list);
  if ( rec == NULL ) return NULL;
  return rec->attributes;
}

bool *Evaluation::GetAttributes(const char* key){
  Object  list; list.SetSymbol(key);
  return GetAttributes(list);
}

bool Evaluation::AttributeQ(Object &list, AttributeType intype){
  bool *attris = GetAttributes( list );
  if ( attris == NULL ) return false;
  return pond::AttributeQ(attris,intype);
}

bool Evaluation::AttributeQ(const char*inkey, AttributeType intype){
  Object  list; list.SetSymbol(inkey);
  return AttributeQ(list,intype);
}

bool Evaluation::AttributeQ(const unsigned int id, AttributeType intype){
  bool *attris = GetAttributes(id);
  return pond::AttributeQ(attris,intype);
}

pair<int,EvaRecord*> Evaluation::NewEvaRecord(){
  int id = 0;
  EvaRecord *rec;
  if ( freeEvaRecord.size() > 0 ){
    id = *freeEvaRecord.rbegin();
    if ( evaRecordTable[ id ] != NULL ){
      rec = evaRecordTable[ id ];
      rec->ClearAll();
    }else{
      rec = new EvaRecord();
      evaRecordTable[ id ] = rec;
    }
    freeEvaRecord.pop_back();
  }else{
    id = evaRecordTable.size();
    rec = new EvaRecord();
    evaRecordTable.push_back( rec );
  }
  return pair<int,EvaRecord*>(id,rec);
}

bool Evaluation::ClearEvaRecord(int id){
  if ( id <= 0 ) return true;
  if ( id >= (int)evaRecordTable.size() ) {
    zhErroring("ClearEvaRecord","清除记录超界.") ||
      Erroring("ClearEvaRecord","Try to clear EvaRecord out of range.");
    return false;
  }
  if ( evaRecordTable[id] == NULL ) return false;
  delete evaRecordTable[id];
  evaRecordTable[id] = NULL;
  freeEvaRecord.push_back(id);
  return true;
}

int Evaluation::Clear(Object&obj,bool repeated){
  for ( auto iter = valueTables.rbegin(); iter != valueTables.rend(); iter++){
    int tes =  (*iter)->Clear(obj,this);
    if ( tes < 0 )
      return -1;
    if (  tes > 0 and not repeated )
      return 1;
  }
  return 0;
}

int Evaluation::Call( const char *funcName, Object &ARGV)
{
  EvaRecord*rec = GetEvaRecord(funcName);
  if ( rec == NULL ) {
    Erroring("EvaKernel",(string)"'"+funcName+"' is not available in yet.");
    return -1;
  }
  if ( rec->staticFunction )
    return (*rec->staticFunction)(ARGV);
  if ( rec->module and rec->memberFunction )
    return ((rec->module)->*(rec->memberFunction))(ARGV);
  Erroring("EvaKernel",(string)"'"+funcName+"' is not callable.");
  return -1;
}

int Evaluation::Call(const EvaRecord*rec,Object&ARGV)
{
  if ( rec == NULL ) {
    Erroring("EvaKernel",ARGV.ToString()+" '"+ToString((void*)rec)+"' is NULL.");
    return -1;
  }
  if ( rec->staticFunction )
    return (*rec->staticFunction)(ARGV);
  if ( rec->module and rec->memberFunction )
    return ((rec->module)->*(rec->memberFunction))(ARGV);
  Erroring("EvaKernel",ARGV.ToString()+" '"+ToString((void*)rec)+"' is not callable.");
  return -1;
}

EvaRecord *Evaluation::GetEvaRecord(const char*str)
{
  Object obj; obj.SetSymbol( str );
  return GetEvaRecord( obj );
}

EvaRecord *Evaluation::GetEvaRecord(Object&list){
  if ( list.ValuedSymbolQ() ){
    Object valuepair(list.idx());
    auto ids = valuepair.ids();
    if ( ids > 0 and ids < evaRecordTable.size() )
      return evaRecordTable[ ids ];
    return NULL;
  }
  for ( auto iter = valueTables.rbegin(); iter != valueTables.rend(); iter++){
    Object valuepair = (*iter)->GetOrNewValuePairRef( list,true );
    if ( valuepair.NullQ() ) continue;
    if ( valuepair.ids() > 0 and valuepair.ids() < evaRecordTable.size() ){
      return evaRecordTable[ valuepair.ids() ];
    }
  }
  return NULL;
}

EvaRecord *Evaluation::GetOrNewEvaRecord(Object&list){
  // Object valuepair = currentValueTable->GetOrNewValuePairRef( list );
  //Object valuepair = GetOrNewBottomValuePairRef( list );
  Object valuepair = GetOrNewValuePairRef( list );
  if ( valuepair.ids() > 0 and valuepair.ids() < evaRecordTable.size() ){
    return evaRecordTable[ valuepair.ids() ];
  }else{
    EvaRecord *rec = new EvaRecord();
    if ( freeEvaRecord.size() > 0 ){
      valuepair.set_ids( *freeEvaRecord.rbegin() );
      evaRecordTable[ *freeEvaRecord.rbegin() ] = rec;
      freeEvaRecord.pop_back();
    }else{
      valuepair.set_ids( evaRecordTable.size() );
      evaRecordTable.push_back(rec);
    }
    return rec;
  }
}

int Evaluation::SetMaxRecursionDepth(int n){
  if (n<=0) return -1;
  EvaKernel->maxRecursionDepth  = n;
  return 0;
}

// Matrix things

Matrix *Evaluation::GetMatrix(string name){
  return GlobalPool.Matrices.GetMatrix(name);
}

ComplexMatrix *Evaluation::GetComplexMatrix(string name){
  return GlobalPool.ComplexMatrices.GetMatrix(name);
}

Matrix *Evaluation::GetOrNewMatrix(string name){
  return GlobalPool.Matrices.GetOrNewMatrix(name);
}

ComplexMatrix *Evaluation::GetOrNewComplexMatrix(string name){
  return GlobalPool.ComplexMatrices.GetOrNewMatrix(name);
}

int Evaluation::RemoveMatrix(string name){
  return GlobalPool.Matrices.RemoveMatrix(name);
}

int Evaluation::RemoveComplexMatrix(string name){
  return GlobalPool.ComplexMatrices.RemoveMatrix(name);
}

///////////////////////////////////////////// 
int Evaluation::PD_Print(Object & ARGV){
  static INIT_SYMID_OF(end);
  // static INIT_SYMID_OF(END);
  static INIT_SYMID_OF(sep);
  // static INIT_SYMID_OF(SEP);
  string end = "\n";
  string sep = " ";
  int i = 1; 
  while ( i <= ARGV.Size() ){
    if ( ARGV[i].PairQ(SYMID_OF_Set) ){
      if ( ARGV[i][1].SymbolQ(SYMID_OF_end) ){
        EvaKernel->Evaluate( ARGV[i][2] );
        end = ARGV[i][2].ToString();
        ARGV.Delete(i);
        continue;
      }else if ( ARGV[i][1].SymbolQ(SYMID_OF_sep ) ){
        EvaKernel->Evaluate( ARGV[i][2] );
        sep = ARGV[i][2].ToString();
        ARGV.Delete(i);
        continue;
      }else{
        zhErroring("打印","未知参数 "+ARGV[i].ToString() ) ||
          Erroring("print","Unknown argument "+ARGV[i].ToString() );
        ReturnError;
      }
    }else{
      EvaKernel->Evaluate( ARGV[i] );
    }
    i++;
  }
  string str;
  for (auto iter=ARGV.Begin(); iter != ARGV.End(); iter++){
    bool isEnd = (  iter == ARGV.End() -1 );
    // check if is end
    if(  (*iter).NullQ() ) continue;
    str += ( (*iter).ToString()+(isEnd?"":sep) );
  }
  str += end;
  PondInnerStringRestoreNormal(str);
  // pond::PostMessage( str+"print" );
  printf("%s",str.c_str() );
  ReturnNull;
}

// int Evaluation::PD_Console(Object & ARGV){
//   string str;
//   if ( ARGV.Size()>0 ){
//     for (auto iter=ARGV.Begin(); iter!=ARGV.End(); iter++){
//       // EvaKernel->Evaluate(*iter);
//       if(  (*iter).NullQ() ) continue;
//       str += (*iter).ToString() + " ";
//       specialCharReplacement(str);
//     }
//     cout<< str <<endl;
//   }else{
//     cout<< endl;
//   }
//   ReturnNull;
// }


// the next three function is for directly print to nb, so no specialchar replacement is needed
int Evaluation::Print(string str){
  if ( str.size() ){
    // specialCharReplacement(str);
    // pond::PostMessage( str+"print" );
    printf("%s",str.c_str() );
  }
  return 0;
}

int Evaluation::Println(string str){
  // specialCharReplacement(str);
  // pond::PostMessage( str+"\nprint" );
  printf("%s\n",str.c_str() );
  return 0;
}

int Evaluation::PrintJson(Object&ARGV){
  // string str = ARGV.DumpToJson() + "pjson";
  // is not used to print out for display, specialChar should not be replaced back
  //specialCharReplacement(str);
  // pond::PostMessage( str );

  printf("%s", ARGV.ToString().c_str() );
  return 0;
}

// for the same reason as print to nb dump json should not do spe replacement neither
// only print function as tranditional function will require spe replacement
int Evaluation::PD_PrintJson(Object& ARGV){
  if ( ARGV.Size()>0 ){
    for (auto iter=ARGV.Begin(); iter!=ARGV.End(); iter++){
      if(  (*iter).NullQ() ) continue;
      // string str = (*iter).DumpToJson()+"pjson";
      // //specialCharReplacement(str);
      // pond::PostMessage( str );
      printf("%s",(*iter).ToString().c_str());
    }
  } 
  ReturnNull;
}

int Evaluation::PD_WriteFile(Object& ARGV ){
  CheckShouldEqual(2);
  const string filename   = ARGV[1].ToString();
  const string content    = ARGV[2].ToString();
  //cout<<"try to write content "<<content<<" to file "<<filename<<endl;
  ofstream outfile;
  outfile.open( filename );
  outfile<< content << endl;
  outfile.close();
  //cout<<"write file done"<<endl; 
  
  ReturnNull;
}

// int Evaluation::PD_InitializeDin(Object& ARGV ){
//   CheckShouldEqual(1);
//   EvaKernel->_din.Initialize( ARGV[1].Key() );
//   ReturnNull;
// }

int Evaluation::PD_DumpToJson(Object& ARGV){
  if ( ARGV.Size()>0 ){
    for (auto iter=ARGV.Begin(); iter!=ARGV.End(); iter++){
      if(  (*iter).NullQ() ) continue;
      // string str = (*iter).DumpToJson()+"print";
      //specialCharReplacement(str);
      // pond::PostMessage( str );
      printf("%s",(*iter).ToString().c_str() );
    }
  } 
  ReturnNull;
}

Object Evaluation::StackPushCopy(const Object&left,const Object&right){
  return currentValueTable->StackPushCopy(left,right);
}

void Evaluation::StackPushPairRef(const Object&pair){
  return currentValueTable->StackPushRef(pair);
}

int Evaluation::StackPop(){
  return currentValueTable->StackPop();
}

int  Evaluation::GetStackPtr(){
  return currentValueTable->stackTable.Size();
}
bool Evaluation::SetStackToPtr(int ptr){
  Object&table = currentValueTable->stackTable;
  if ( table.Size() > 0 and table.Size() > ptr ){
    currentValueTable->stackTable.Delete( table.Begin() + ptr, table.End() );
    return true;
  }
  return true;
}

int Evaluation::PD_FetchFullContext( Object &ARGV){
  //dout<<"try get all value tables"<<endl;
  Object output(ObjectType::List,SYMID_OF_List);
  for (auto iter = EvaKernel->valueTables.begin(); iter != EvaKernel->valueTables.end(); iter++){
    output.PushBackList(  SYMID_OF_List );
    output.Last().PushBackRef( (*iter)->patternTable );
    output.Last().PushBackRef( (*iter)->pairTable );
    output.Last().PushBackRef( (*iter)->stackTable );
  }
  // string strout = output.DumpToJson() + "vtmem";
  // specialCharReplacement(strout);
  // pond::PostMessage( strout );
  printf("%s", output.ToString().c_str() );
  ReturnNull;
}

int Evaluation::PD_FetchContext( Object &ARGV){
  Object output(__List__,SYMID_OF_List );
  output.PushBackRef( EvaKernel->currentValueTable->pairTable )
    .PushBackRef(     EvaKernel->currentValueTable->patternTable)
    .PushBackRef(     EvaKernel->currentValueTable->stackTable );
  // string strout = output.DumpToJson() + "vtmem";
  // pond::PostMessage( strout );
  printf("%s",output.ToString().c_str() );
  ReturnNull;
}


// int Evaluation::PD_Profiling(Object&ARGV){
//   CheckShouldEqual(1);
//   EM_ASM({
//       console.profile();
//     });
//   EvaKernel->Evaluate( ARGV[1] );
//   EM_ASM({
//       console.profileEnd();
//     });
//   ReturnNull;
// }

int Evaluation::PD_SetMaxRecursionDepth(Object&ARGV){
  CheckShouldEqual(1);
  CheckShouldBeNumber(1);
  int d = ARGV[1].Number();
  if ( d <1 ) d = 256;
  EvaKernel->SetMaxRecursionDepth(d);
  ReturnNull;
}

int Evaluation::PD_GetMemInfo(Object&ARGV){
  // CheckShouldEqual(0);
  cout<<"GlobalPool    Objects" <<"  size :"<<GlobalPool.Objects.idx.row*GlobalPool.Objects.RowSize+ GlobalPool.Objects.idx.col<< "    buffer :"<<GlobalPool.Objects.freeObjs.size()<<endl;
  cout<<"              Lists  " <<"  size :"<<GlobalPool.Lists.idx.row*GlobalPool.Objects.RowSize+ GlobalPool.Lists.idx.col << "    buffer :"<<GlobalPool.Lists.freeObjs.size()<<endl;
  ReturnNull;
}

int Evaluation::PD_GetStackInfo(Object&ARGV){
  cout<<"EvaKernel valueTable size = "<<EvaKernel->valueTables.size()<<endl;
  cout<<"EvaKernel EvaluationDption = "<<EvaKernel->EvaluationDepth<<endl;
  ReturnNull;
}

// static INIT_OPER_SYMID_OF( exit,         0,220,   1,1    );
static INIT_SYMID_OF(exit);

int Evaluation::PD_exit(Object&ARGV){
  EvaKernel->statusCode = KernelStatusQuit;
  if ( ARGV.Size() == 1 && ARGV[1].NumberQ() ){
    EvaKernel->statusObject = ARGV[1];
  }
  ReturnNull;
}

//static INIT_OPER_SYMID_OF(help,         0,220,   1,1    );
static INIT_SYMID_OF(help);
int Evaluation::PD_help(Object&ARGV){
  CheckShouldEqual(1);
  if ( ARGV[1].ListQ( SYMID_OF_Parenthesis ) ){
    ARGV[1] = ARGV[1][1];
    CheckShouldEqual(1);
  }
  const char *sym = ARGV[1].Key();
  EvaRecord *rec = EvaKernel->GetEvaRecord( sym );
  string res;
  if ( rec ){
    string doc = rec->description+"|||";
    int start_p = 0, split_p=-1;
    while ( true ){
      split_p = doc.find( "|||", start_p );
      if ( split_p < 0 ){
        break;
      } else {
        while ( doc[start_p] == '\n'
                || doc[start_p] == '\r'
                || doc[start_p] == ' '
                || doc[start_p] == '\t' ){
          start_p += 1;
        }
        res = doc.substr(start_p, split_p-start_p);
        //dout<<"update res to >"<<res<<endl;
        start_p = split_p + 3;
        if ( res.substr(0,3) == (EvaKernel->lang + ":") ){
          res = res.substr(3, res.size()-3);
          while ( res.size()>0 && res[0] == ' ' ){
            res = res.substr(1, res.size()-1);
          }
          if ( res.size()>0 && res[0] == '\n' ){
            res = res.substr(1, res.size()-1);
          }
          break;
        }
      }
    }

    if ( res.size() > 0 ){
      cout << endl<< res << endl;
    } else {
      cout<<endl;
      zhMessage("没有找到 '"+(string)ARGV[1].Key()+"' 的帮助信息.") ||
        Message("Help info for '"+(string)ARGV[1].Key()+"' is not found.");
    }
  } else {
    cout<<endl;
    zhMessage("'"+(string)ARGV[1].Key()+"' 是一个尚未注册的符号变量.") ||
      Message("'"+(string)ARGV[1].Key()+"' is an unregistered symbol.");
  }
  ReturnNull;
  //ReturnNormal;
}
