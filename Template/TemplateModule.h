/**********************************
Author          : MOD_AUTHOR
**********************************/
#ifndef TemplateModule_POND_H
#define TemplateModule_POND_H

namespace pond{
  class TemplateModule:public Module{
  public:
    TemplateModule();
    ~TemplateModule();

    int PD_vector_times(Object &);
  };
};

#endif
