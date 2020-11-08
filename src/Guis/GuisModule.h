#ifndef GuisModule_POND_H
#define GuisModule_POND_H

namespace pond{

  
  class GuisModule:public Module
  {
  public:
    GuisModule(); ~GuisModule();
    int PD_Notepad(Object& );
    int PD_Runmod(Object& );
  };
};
#endif
