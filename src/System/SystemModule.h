#ifndef Gisp_SystemModule_H
#define Gisp_SystemModule_H

namespace pond{
  class SystemModule:public Module{
  public:
    SystemModule(); ~SystemModule();
    //DeclareFunction(Test);
    ///////////////////////////////
    //Evaluate
    int PD_Evaluate(Object &);
    int PD_EvaluateString(Object &);
    int PD_EvaluateFile(Object &);
    int PD_EvaluateFileWithReturn(Object &);
    //Options && Attributes
    int PD_Options(Object &);
    static int SetOptions(Object &);
    static int CurrentValue(Object &);
    ///////////////////////////////
    int PD_Attributes(Object &);
    static int SetAttribute(Object&left,Object&attris);
    static int SetAttribute_Eva(Object &);
    static int AddAttribute_Eva(Object &);
    static int AddLocalAttribute_Eva(Object &);
    static int RemoveAttributes_Eva(Object &);
    static int RemoveLocalAttributes_Eva(Object &);
    int PD_Protect(Object &);
    int PD_UnProtect(Object &);
    ///////////////////////////////
    static int Parenthesis(Object &);
    static int CompoundExpression(Object &);
    static int ExpressionList(Object &);
    //static int Print(Object &);
    ///////////////////////////////
    static int ToString_Eva(Object &);
    static int ToExpression(Object &);
    ///////////////////////////////
    /* static int Hold(Object &); */
    static int HoldOnce(Object &);
    /* static int HoldPattern(Object &); */
    static int UnifyRule(Object &);
    static int UnifiedMatchQ(Object &);
    static int MatchQ(Object &);
    static int PatternCompare(Object &);
    static int Cases(Object &);
    static int Position(Object &);
    static int Count(Object &);
    static int Replace(Object &);
    static int ReplaceAll(Object &);
    static int ReplaceAllRepeated(Object &);
    static int DeleteCases(Object &);
    ////////////////////////////////////////
    ///////////////////////////////
    static int GrammarModule(Object &);
    static int Function(Object &);
    static int PureFunction(Object &);
    ////////////////////////////////////////
    static int Conjunct(Object &);
    ///////////////////////////////
    static int If(Object &);
    static int While(Object &);
    static int For(Object &);
    static int Do(Object &);
    ///////////////////////////////
    static int Foreach(Object &); //Foreach[i,{...},expr]
    ///////////////////////////////
    int PD_System(Object &);
    static int SystemWithoutOutput(Object &);
    ///////////////////////////////
    static int Set(Object  &Argv);
    static int SetDelayed(Object  &Argv);
    ///////////////////////////////
    static int ShowModuleFunctions(Object &);
    static int ShowModules(Object &);
    // static int In(Object  &);
    // static int Out(Object  &);
    static int SetOutputFormat(Object  &);
    static int Input(Object&);
    static int SystemArgv(Object&);
    ///////////////////////////////
    int PD_SetParallelMode(Object&);
    int PD_GetParallelMode(Object&);
    int PD_GPUDeviceQ(Object&);
    int PD_SetCudaThreadsNumberPerBlock(Object&);
    int PD_GetCudaThreadsNumberPerBlock(Object&);
    int PD_SetCpuThreadsNumber(Object&);
    int PD_GetCpuThreadsNumber(Object&);
    ///////////////////////////////
    static int ShowHelpInfo(Object &);
    static int Clear(Object &);
    ///////////////////////////////
    static int AbsoluteTime(Object &);
    static int DateList(Object &);
    static int Timing(Object &);
    static int CpuTiming(Object &);
    static int Sleep(Object&);
    static int SimpleSimplify(Object&);
    ///////////////////////////////

    int PD_if(Object&);
    int PD_while(Object&);
    int PD_for(Object&);
    int PD_switch(Object&);
    int PD_import(Object&);
    int PD_var(Object&);
    int PD_def(Object&);
    int PD_FUNCTION$DEFINED$(Object&);
    int PD_continue(Object&);
    int PD_break(Object&);
    int PD_return(Object&);


    int PD_in(Object&);
    int PD_is(Object&);

    int PD_context(Object&);

    int PD_type(Object&);

    int PD_ArrowFunction(Object&);
    int PD_Unpack(Object&);
    int PD_UnpackDict(Object&);
    ///////////////////////////////
    ///////////////////////////////
    int PD_FetchMemoryToJson(Object&);
    ///////////////////////////////
    int PD_RunPageJS(Object&);
    int PD_RunKernelJS(Object&);
    ///////////////////////////////
  };
};
#endif
