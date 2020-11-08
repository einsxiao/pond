#ifndef POND_Class_H
#define POND_Class_H

namespace pond{
  class ClassModule:public Module{
  public:
    ClassModule(); ~ClassModule();
    int PD_class(Object&);
    int PD_CLASS$INSTANCE$(Object&);
    int PD_CLASS$DEFINED$(Object&);
    /* int PD_test(Object&); */
    /* int PD_test2(Object&); */
  };
};
#endif
