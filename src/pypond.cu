/*
  Author: Eins, Xiao
  Content: Pond shared lib for Python
  CopyRight: CDE
*/
#include<pond.h>
#include"unistd.h"

#include"Python.h"

using namespace std;
using namespace pond;

PyObject *EvaluateString(PyObject *self, PyObject *args ) {
  static Kernel kernel(&cin, true, 0, NULL, "");
  //kernel.pdetail = pdetail;
  const char *code;
  bool     pdetail = false;
  if ( !PyArg_ParseTuple(args,"sb", &code, &pdetail) ){
    return NULL;
  }

  Object result;
  kernel.evaluation->EvaluateString( code, result );

  PyObject *res = Py_BuildValue( "s", result.Key() );
  return res;

}

static PyMethodDef pond_methods[] = {
  {"evaluate", EvaluateString, METH_VARARGS, "" },
};

static struct PyModuleDef pondmodule = {
  PyModuleDef_HEAD_INIT,
  "pond",
  "pond doc string",
  -1,
  pond_methods
};

PyMODINIT_FUNC PyInit_pond(void){
  return PyModule_Create(&pondmodule);
}

