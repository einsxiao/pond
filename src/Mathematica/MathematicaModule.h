#ifndef MathematicaModule_POND_H
#define MathematicaModule_POND_H

namespace pond{
  class MathematicaModule_vars;
  class MathematicaModule:public Module{
    DeclareModule(Mathematica);
  private:
    void  *env;
    void  *link;
  public:
    bool   On();
    bool   Open();
    bool   Close();
    int    PutObject(Object&);
    int    PutMatrix(string ,Matrix&);
    string Evaluate(string);
    int    PutValue(string,Object);
    Object GetValue(string);
    DeclareFunction(Mathematica);
    DeclareFunction(MLPutValue);
    DeclareFunction(MLGetValue);
    DeclareFunction(MLPutMatrix);
    DeclareFunction(MLGetMatrix);
  };
};
#endif
