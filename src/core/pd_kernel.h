#ifndef Kernel_Xiao_H
#define Kernel_Xiao_H

namespace pond{

  class Kernel{
  public:
    Object       superList; //super Argv of lists readin
    Object       inputList; //historical Argv of lists readin
    ImportList  *importList;
    Evaluation  *evaluation;
    bool         interactively;
    bool         noprint       = false;
    bool         pmark         = false;
    ///////////////////////////////
    Kernel(std::istream *istr_in,bool interactively,int argc,char*argv[],std::string rcfile, bool pmark=false, bool pimport=false);
    ~Kernel();
    //////////////////////////////////////////////
    int   Phrasing();
    //////////////////////////////////////////////
  }; 

};

#endif
