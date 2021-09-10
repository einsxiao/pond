/**********************************
Author          : AUTHOR
Version         : Template.VERSION  
R-Copyleft(r-ɔ) : DATE
**********************************/
#ifndef TemplateModule_POND_H
#define TemplateModule_POND_H
/****************************/

namespace pond{
  /*******************************************************************
    Template 模块中须定义 TemplateModule 类作为接口类 
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
    int PD_hello(Object &);
    int PD_matX2(Object &);
  };
};

/****************************/
#endif
