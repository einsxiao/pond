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
    Template 模块中必需定义 TemplateModule 类作为接口类 
  *******************************************************************/
  class TemplateModule:public Module{
  private :
  public  :
    /******************************************************************/
    TemplateModule(); ~TemplateModule();
    /*******************************************************************
      形如PD_hello 这样输入变量为 Object&，返回值为 int，            
      函数名前缀为 PD_ 的函数，将自动注册到pond中，可在pond中自动调用
    *******************************************************************/
    int     PD_hello(Object&argv);
    int     PD_hello2(Object&argv);
  };
};

/****************************/
#endif



