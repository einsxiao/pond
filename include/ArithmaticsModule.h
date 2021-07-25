
namespace pond{

#define pow2(x) ((x)*(x))
#define sign(num) ((num)>0?1:((num)<0?-1:0)); 


  class ArithmaticsModule:public Module{
  public:
    ArithmaticsModule(); ~ArithmaticsModule();
    ///////////////////////////////
    int PD_Plus(Object&);
    int PD_Times(Object&);
    int PD_Power(Object&);
    int PD_Log(Object&);
    /* static int Complex(Object&); */
    ///////////////////////////////
    int PD_AddTo(Object&);
    int PD_SubtractFrom(Object&);
    int PD_TimesBy(Object&);
    int PD_DivideBy(Object&);
    int PD_PowerWith(Object&);
    ///////////////////////////////
    int PD_PreIncrement(Object&);
    int PD_PreDecrement(Object&);
    int PD_Increment(Object&);
    int PD_Decrement(Object&);
    ///////////////////////////////
    int PD_Greater(Object&);
    int PD_GreaterEqual(Object&);
    int PD_Less(Object&);
    int PD_LessEqual(Object&);
    int PD_Equal(Object&);
    int PD_UnEqual(Object&);
    ///////////////////////////////
    int PD_Not(Object&);
    int PD_And(Object&);
    int PD_Or(Object&);
    ///////////////////////////////
    int PD_Abs(Object&);
    int PD_Factorial(Object&);
    ///////////////////////////////
    int PD_SetRandomSeed(Object&);
    int PD_Random(Object&);
    ///////////////////////////////
    int PD_Sin(Object&);
    int PD_Cos(Object&);
    int PD_Tan(Object&);
    int PD_Ln(Object&);
    int PD_Mod(Object&);
    int PD_Floor(Object&);
    int PD_Round(Object&);
    int PD_Ceil(Object&);
    int PD_Sqrt(Object&);
    int PD_Max(Object&);
    int PD_Min(Object&);
    int PD_RealQ(Object&);
    int PD_IntegerQ(Object&);
    int PD_EvenQ(Object&);
    int PD_OddQ(Object&);
  };
};

