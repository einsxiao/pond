#ifndef POND_H_Class
#define POND_H_Class
#ifndef POND_Class_H
#define POND_Class_H

namespace pond{
  DeclareModule(Class){Constructors(Class);
  public:
    int PD_class(Object&);
    int PD_CLASS$INSTANCE$(Object&);
    int PD_CLASS$DEFINED$(Object&);
    /* int PD_test(Object&); */
    /* int PD_test2(Object&); */
  };
};
#endif

#endif