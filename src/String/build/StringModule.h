#ifndef POND_H_String
#define POND_H_String

namespace pond{
  DeclareModule(String){Constructors(String);
  public:
    DeclareFunction( StringSplit );
    DeclareFunction( RandomString );
    DeclareFunction( StringJoin );
    int PD_RunJS( Object & );
    int PD_RunJSInt( Object & );
    int PD_RunJSString( Object & );
  };
};


#endif