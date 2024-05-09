/**********************************
Author          : MOD_AUTHOR
**********************************/
#ifndef TestModule_POND_H
#define TestModule_POND_H

namespace pond{
  class TestModule:public Module{
  public:
    TestModule();
    ~TestModule();

    int PD_vector_times(Object &);
  };
};

#endif
