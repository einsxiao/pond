/**********************************
Author          : phpp@ovo.ltd
Version         : test_phpp.0.1.0  
R-Copyleft(r-ɔ) : 12/22/2020 - *
**********************************/

namespace pond{
  class test_phppModule:public Module{
      public:
      /*************************************************
        Template 模块中须定义 TemplateModule 类作为接口类                                                                            
       *************************************************/
      test_phppModule();       ~test_phppModule();
      /***********************************************************
        形如int PD_XXX(Object&) 这样前缀为 PD_ , Object& 做唯一参数， 
        以及返回值为 int 的函数，将自动注册到pond中，并可在 pond 脚本
        中调用. 
       **********************************************************/
      int PD_f(Object&);
  };
};
