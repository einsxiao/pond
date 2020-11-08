#ifndef AlgebraicModule_Eins_H
#define AlgebraicModule_Eins_H

namespace pond{
  class AlgebraicModule:public Module{
  public:
    AlgebraicModule();
    ~AlgebraicModule();
    /////////////////////////////////
    int SimpleSimplify(Object&);
  private:
    Object SSRules;
  };
};


#endif

