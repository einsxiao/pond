#ifndef SuanZi_PythonModule_H
#define SuanZi_PythonModule_H

#include"Python.h"

namespace pond{
  class PythonModule: public Module{
  private:
    MatrixModule*matrixModule;
  public:
    PythonModule(); ~PythonModule();
    ////////////////////////////////////////
    ////////////////////////////////////////
    int             Initialize();
    int             Finalize();
    bool            IsInitialized();
    ////////////////////////////////////////
    PyObject       *PyMain;
    ///////////////////////////////////////
    int             PyObject2List(PyObject*,Object&);
    int             List2PyObject(Object&,PyObject*&);
    ///////////////////////////////////////
    int             Evaluate(std::string);
    int             Evaluate(Object&);
    ///////////////////////////////////////
    int             PutValue(Object&);
    int             PutValue(std::string name, Object&);
    int             GetValue(Object&);
    Object          GetValue(std::string name);
    ///////////////////////////////////////
    int 						Import(Object&);
    bool            AddPath(std::string path);
    int 						AddPath(Object&);
  };
  
};
#endif
