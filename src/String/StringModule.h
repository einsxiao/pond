
namespace pond{
  class StringModule:public Module{
  public:
    StringModule(); ~StringModule();
    int PD_StringSplit(Object& );
    int PD_RandomString(Object& );
    int PD_StringJoin(Object& );
    /* int PD_RunJS( Object& ); */
    /* int PD_RunJSInt( Object& ); */
    /* int PD_RunJSString( Object& ); */
  };
};

