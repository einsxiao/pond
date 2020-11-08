#ifndef Evaluation_POND_H
#define Evaluation_POND_H

namespace pond{
  class EvaRecord{
  public:
    bool               attributes[24];
    StaticFunction     staticFunction =NULL ;
    Module*            module =NULL ;
    MemberFunction     memberFunction =NULL ;
    std::string        description ;
    EvaRecord();
    ~EvaRecord();
    EvaRecord(const EvaRecord&);
    // update do get something from old record
    //   attributes sum up
    //   others set to newone, old clear up
    //   description set to newone
    void               Update(const EvaRecord&); 
    void               ClearValues();     //clear values from staticFunction from the last, attributes will hold
    void               ClearAll();        //clear values from staticFunction from the last, attributes will hold
    int                Call(Object&Argv); // directly call dispite static or member method
    bool               isCallable(); 
  };

  class Evaluation{
  public:
    friend class ValueTable;
    friend class Kernel;
    friend class SystemModule;
    friend class MeioModule;
    friend class ClassModule;
    /* bool                                        ErrorOccured  = false; */
    /* bool                                        QuitOccured   = false; */
    //    66:       continue       obj:  not defined
    //    88:       return         obj:  return arg;
    //    44:       break          obj:  not defined;
    /* int                                         statusEnv     = 0; */
    int                                         statusCode    = 0;
    Object                                      statusObject;
  private:
    int                                         maxRecursionDepth = 256;
    int                                         EvaluationDepth;
    bool                                        DebugMode;
    /* bool                                        InteractivelyMode; */
    //////////////////////////////////////////////////////////////
    ///////  moduleTables
    std::map<std::string, Module*>				      moduleTable;
    std::map<std::string, type_create*>         moduleCreateHandleTable;
    std::map<std::string, type_destroy*>        moduleDestroyHandleTable;
    std::map<std::string, void*>                libHandlerTable;
    //string Context;
    //////////////////////////////////////////////////////////////
    ///////  id to others
    std::vector<EvaRecord*>                     evaRecordTable;
    std::vector<int>                            freeEvaRecord;
    //////////////////////////////////////////////////////////////
    /////// valueTables 
#ifdef DEBUG
  public:
#endif
    std::vector< ValueTable* >                  valueTables; 
    ValueTable                                 *currentValueTable;
  private:
    //////////////////////////////////////////////////////////////
    /////// specific affairs
    //Object                                      superList,inputList;
  public:
    /* DocIn                                       _din; */
    /* DocOut                                      _dout; */
    /* DocErr                                      _derr; */
    /* DebugOut                                    _deout; */
    int                                         argc;
    char                                      **argv;
    //////////////////////////////////////////////////////////////
    int                                         flag     = 0    ;
    std::string                                 lang     = "zh" ; // user language
    std::map<std::string,std::string>           messages; // main thread message
  public:
    Evaluation(bool debug = false);  ~Evaluation();
    //////////////////////////////////////////////////////////////
    ///// Evaluate something
    //  ofDelay if the calling form of delay function, all delayfunction attribute is apply to this form
    int             AttributeProcessing(Object&obj,bool *attri,bool isHold=false,bool ofDelay=false);
    int             PreEvaluate(        Object&obj);
    void            EvaluateHead(       Object&obj,bool isHold=false); 
    void            EvaluateFirst(      Object&obj,bool isHold=false); 
    void            EvaluateRest(       Object&obj,bool isHold=false); 
    void            EvaluateAll(        Object&obj,bool isHold=false); 
    int             ListableEvaluate(   Object&obj,bool isHold=false);
    int             Evaluate(           Object&obj,bool isHold=false,bool isRef=false); 
    int  	          SetMaxRecursionDepth(int n); 
    static int  	  PD_SetMaxRecursionDepth(Object&); 
    Object          EvaluateExpression(const std::string&expr); // a single expression string
    /////////////////////////////////////////////////////////////
    std::vector< std::pair<ImportList*,Object> >  inputs;
    std::set<std::string>                         readyModules;
    int                                           moduleRequesting = 0;
    int             EvaluateString(const std::string&cont, int depth = 0);
    int             EvaluateFile(const std::string filename, int depth=0);
    ///////////////////////////////////////////////////////////////
    int             EvaluateContinue(const std::string&extra_cont); //with extra content before continue
    /* int             EvaluateAppend(std::string&expr_cont); */
    //////////////////////////////////////////////////////////////
    ///// Context
    int             GetCurrentContext();
    int             NewInContext(              const char*instance);
    int             InContext(                 const char*instance);
    int             OutContext();
    int             ClearContext(              const char*instance);
    //////////////////////////////////////////////////////////////
    ///// Modules
    int             GetModuleDependence(       std::string moduleName);
    /* int             GetModuleLib(              std::string moduleName); */
    int             GetModulePondScript(       std::string moduleName);
    int             GetModulePathReady(        std::string moduleName);
    int             GetModuleMatPathReady(     std::string moduleName);
    int             GetModulePyPathReady(      std::string moduleName);
    //////////////////////////////
    int             GetModuleLib(              std::string moduleName);
    int             GetModule(                 std::string moduleName);
    //////////////////////////////////////////////////////////////
    int             RemoveModule(              std::string moduleName);
    int             InsertModule(              std::string moduleName,Module* module);
    Module*         ModulePtr(                 std::string moduleName);//if none return NULL
    //////////////////////////////////////////////////////////////
    /* static int      PD_RemoveModule(           pond::Object&Argv); */
    static int      PD_Import(                 pond::Object&Argv);
    static int      PD_Remove(                 pond::Object&Argv);
    int             ImportRequest(             pond::Object&Argv);
    int             ImportLoad(                pond::Object&Argv);
    static int      PD_ReImport(               pond::Object&Argv);
    //////////////////////////////////////////////////////////////
    ///// EvaRecord Info operations
    // get EvaRecord may related to a symbol
    EvaRecord      *GetEvaRecord(               Object&obj    ); // get record already in value table
    EvaRecord      *GetEvaRecord(               const char*   ); // get record already in value table
    EvaRecord      *GetOrNewEvaRecord(          Object&obj    ); // create valuepair in value table same time
    /////////////////////////////////////////////////////////////
    std::pair<int,EvaRecord*> NewEvaRecord(); // new EvaRecord, return value is (id,rec), no value pair
    bool            ClearEvaRecord(             int i);
    int             Clear(                      Object&obj,bool repeated = false);
    //////////////////////////////////////////////////////////////
    int             Call(                       const char*funcName,Object&Argv);
    int             Call(                       const EvaRecord *funcEvaRecord,Object&Argv);
    //////////////////////////////////////////////////////////////
    ///// ValueTable operations
    //////////////////////////////////////////////////////////////
    Object          StackPushCopy(const Object&left,const Object&right);
    void            StackPushPairRef(const Object&pair);
    int             StackPop();
    int             GetStackPtr();
    bool            SetStackToPtr(int ptr);
    //////////////////////////////////////////////////////////////////////////////////////
    //return value are value pair
    Object          InsertOrUpdatePairValue(const Object&left, const Object&right,bool onlyUpdate=false,bool isCheckedPattern=false);
    Object          InsertOrUpdateBottomPairValue(const Object&left, const Object& right);
    Object          InstantInsertOrUpdatePairValue(const Object&left,const Object& right);
    //search and get value pair
    Object          GetOrNewValuePairRef(       const Object&obj, bool onlyGet=false, bool isCheckedNoPattern=false );
    Object          GetOrNewValuePairRef(       const char*sym);
    Object          GetOrNewBottomValuePairRef( const Object&obj, bool isCheckedNoPattern=false );
    Object          GetValuePairRef(            const Object&obj, bool isCheckedNoPattern=false );
    //////////////////////////////////////////////////////////////////////////////////////
    // Ref is checked no pattern
    Object          RefInstantInsertOrUpdatePairValue(const Object&left, const Object&right);
    Object          RefInsertOrUpdatePairValue(const Object&left, const Object&right);
    //////////////////////////////////////////////////////////////////////////////////////
    // 0 not find, -1 protected and found, 1 unProtected and found
    int             GetValueRef(const           Object&obj, Object&ref);
    int             GetValue(   const           Object&obj, Object&value, bool onlyCheckPatternTable=false);
    template<class type>
    bool            GetValue(   const           std::string key,type&num){
      Object  list(ObjectType::Symbol, key );
      Object  value = GetValuePairRef( list );
      if ( value.NullQ() or value[1] == value[2] ){return false;}
      if ( !value[2].NumberQ() ){return false;}
      num = value[2].Number();
      return true;
    }
    bool            GetValue(                   std::string,std::string&);
    bool            GetValue(                   std::string,double&);
    bool            GetValue(                   std::string,float&);
    bool            GetValue(                   std::string,int&);
    bool            GetValue(                   std::string,bool&);
    ///////////
    //////////////////////////////////////////////////////////////
    char            Boolean(                    Object&obj);
    //////////////////////////////////////////////////////////////
    ///// new ValueTable 
    Object          newContext();    // return the pairTalle ref
    Object          newContext(           ValueTable*); 
    Object          newContext(           Object&pairTable ); 
    Object          newContext(           Object&pairTable, Object&patternTable ); 
    int             deleteContext();
    int             ClearValueTablesTo(         u_int);
    //////////////////////////////////////////////////////////////
    ///// Attributes operations Attributes['Set']={'HoldFirst'}
    bool           *SetAttribute(               Object&obj,AttributeType attriType,bool val=true);
    bool           *SetAttribute(               const char*key,AttributeType attriType,bool val=true);
    bool           *SetAttribute(               const unsigned int id,AttributeType attriType,bool val=true);
    bool           *GetAttributes(              Object&obj); 
    bool           *GetAttributes(              const char *); 
    bool           *GetAttributes(              const unsigned int id); 
    bool            AttributeQ(                 Object&obj,AttributeType intype);
    bool            AttributeQ(                 const char*inkey,AttributeType intype);
    bool            AttributeQ(                 const  unsigned int id,AttributeType intype);
    ////////////////////////////////////////
    ////// Imbeded Matrix
    Matrix          *GetMatrix(                 std::string matrixName);
    ComplexMatrix   *GetComplexMatrix(          std::string matrixName);
    Matrix          *GetOrNewMatrix(            std::string matrixName);
    ComplexMatrix   *GetOrNewComplexMatrix(     std::string matrixName);
    int              RemoveMatrix(              std::string matrixName);
    int              RemoveComplexMatrix(       std::string matrixName);
    ////////////////////////////////////////
    ////////////////////////////////////////
    static int       PD_Print(                   Object&Argv); 
    static int       PD_Println(               Object&Argv);
    /* static int       PD_Console(                 Object&Argv); */
    /* static int       PD_Profiling(               Object&Argv); */
    static int       Print(                      const std::string);
    static int       Println(                    const std::string="");
    /* static int       Println(); */
    static int       PrintJson(                  Object&obj);
    static int       PD_PrintJson(               Object&Argv);
    static int       PD_DumpToJson(              Object&Argv);
    ////////////////////////////////////////
    static int       PD_WriteFile(               Object&Argv);
    /* static int       PD_InitializeDin(           Object&Argv); */
    ////////////////////////////////////////
    static int       PD_FetchFullContext(        Object&Argv);
    static int       PD_FetchContext(            Object&Argv);
    /* static int       GetMemInfo(); */
    static int       PD_GetMemInfo(              Object&Argv);
    static int       PD_GetStackInfo(            Object&Argv);
    /* static int       FetchAllVariablesToJson_Eva(   Object&Argv); */
    /* static int       FetchCurrentVariablesToJson_Eva(  Object&Argv); */
  };
#define INIT_VALUETABLEPAIR_ID_OF(sym) Index VALUETABLEPAIR_ID_OF_##sym = EvaKernel->GetOrNewValuePairRefAtTop(#sym).objid

/* #define din     (pond::__GlobalPoolPtr->Eva->_din) */
/* #define dscanf  (pond::__GlobalPoolPtr->Eva->_din.dscanf) */
/* #define dout    (pond::__GlobalPoolPtr->Eva->_dout) */
/* #define dprintf (pond::__GlobalPoolPtr->Eva->_dout.dprintf) */
/* #define derr    (pond::__GlobalPoolPtr->Eva->_derr) */
/* #define deout   (pond::__GlobalPoolPtr->Eva->_deout) */

};


#endif



