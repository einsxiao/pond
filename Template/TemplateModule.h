/**********************************
Author          : MOD_AUTHOR
Version         : Template.MOD_VERSION  
R-Copyleft(r-ɔ) : MOD_DATE
**********************************/
#ifndef TemplateModule_POND_H
#define TemplateModule_POND_H
/****************************/

namespace pond{
  /*******************************************************************
    TemplateModule 为接口类 
  *******************************************************************/
  class TemplateModule:public Module{
  public:
    /******************************************************************/
    TemplateModule();
    ~TemplateModule();
    /*******************************************************************
      形如 int PD_XXX(Object&) 将自动注册，并可在 pond 脚本中调用.
    *******************************************************************/
    int PD_welcome(Object &);
    int PD_vector_times(Object &);
  };
};

/****************************/
#endif
