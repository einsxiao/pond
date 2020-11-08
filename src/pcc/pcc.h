#ifndef __PDCC__
#define __PDCC__

#include<iostream>
#include<vector>
#include<string>
#include<regex>

#include"pond.h"

using namespace std;
using namespace pond;
#include"pcc_cmdline.h"
////////////////////////////////
//#include"syntax_launch_kernel.h"
#include"pcc_syntax.h"
////////////////////////////////
#include"pcc_engine.h"



class cc_err:public Error{
public:
  int lN,cN;
  string file,message;
  cc_err(string _file,int _lN,int _cN,string _mes){
    file = _file;
    lN = _lN;
    cN = _cN;
    message = _mes;
  }
  virtual const string info()const throw(){
    if ( file != "" ){
      return (file+":"+ToString(lN+1)+":"+ToString(cN+1)+": error: "+message);
    }else{
      return ("pcc:: error: "+message);
    }
  }
};

inline void warning(string mes){ cerr<<"pcc:  warning:  "<<mes<<endl; }
#define warningAtPos(idx,mes) cerr<<(filePath+":"+ToString((idx).row+1)+":"+ToString((idx).column+1)+": warning: "+(mes)+"\n")
#define warnErrorAtPos(idx,mes) cerr<<(filePath+":"+ToString((idx).row+1)+":"+ToString((idx).column+1)+": error: "+(mes)+"\n")

#define throwCCError(mes) ({throw cc_err(filePath,(idx).row,(idx).column,mes);})
#define throwCCErrorAtPos(idx,mes) ({throw cc_err(filePath,(idx).row,(idx).column,mes);})



#endif
