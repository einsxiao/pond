#ifndef POND_H_Arithmatics
#define POND_H_Arithmatics

namespace pond{

#define pow2(x) ((x)*(x))
#define sign(num) ((num)>0?1:((num)<0?-1:0)); 


  DeclareModule(Arithmatics){Constructors(Arithmatics);
  public:
    ///////////////////////////////
    static int Plus(Object&);
    static int Times(Object&);
    static int Power(Object&);
    static int Log(Object&);
    /* static int Complex(Object&); */
    ///////////////////////////////
    static int AddTo(Object&);
    static int SubtractFrom(Object&);
    static int TimesBy(Object&);
    static int DivideBy(Object&);
    static int PowerWith(Object&);
    ///////////////////////////////
    static int PreIncrement(Object&);
    static int PreDecrement(Object&);
    static int Increment(Object&);
    static int Decrement(Object&);
    ///////////////////////////////
    static int Greater(Object&);
    static int GreaterEqual(Object&);
    static int Less(Object&);
    static int LessEqual(Object&);
    static int Equal(Object&);
    static int UnEqual(Object&);
    ///////////////////////////////
    static int Not(Object&);
    static int And(Object&);
    static int Or(Object&);
    ///////////////////////////////
    static int Abs(Object&);
    static int Factorial(Object&);
    ///////////////////////////////
    static int SetRandomSeed(Object&);
    static int Random(Object&);
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


#endif