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
      形如int PD_XXX(Object&) 这样前缀为 PD_ , Object& 做唯一参数， 以及
      返回值为 int 的函数，将自动注册到pond中，并可在 pond 脚本中调用.
    *******************************************************************/
    int PD_welcome(Object &);
    int PD_vector_times(Object &);
  };
};

/****************************/
#endif
