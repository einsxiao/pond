#include"pd_headers.h"
using namespace std;
using namespace pond;

DebugOutput __DebugOutput::out;


void pond::Error::trace()const{
#ifdef DEBUG
#endif
}

const string pond::Error::swhat()const throw(){
  //print backtrace information
  // dout<<"pond error throwed with what calculated"<<endl;
  // dout<<"module = "<<module<<endl;
  // dout<<"function = "<<function<<endl;
  // dout<<"message = "<<message<<endl;
  string res;
  if ( module != "" ){
    res = module+"::"+function+"::Error: "+message;
  }else{
    if ( function != "" ){
      res = function+"::Error: "+message;
    }else{
      res = "Error: "+message;
    }
  }
  // dout<<"what calculated :"<< res <<endl;
  return res;
}

const char *pond::Error::what()const throw(){
  return swhat().c_str();
}

