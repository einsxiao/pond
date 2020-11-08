#include"pond.h"
#include"MatrixModule.h"
#include"PythonModule.h"
// #include"pd_oldmacro.h"

using namespace std;
using namespace pond;

__DEBUG_MAIN__("input.pd");

PythonModule::PythonModule():Module(MODULE_NAME){
  matrixModule=(MatrixModule*) EvaKernel->ModulePtr("Matrix");
  PyMain = NULL;
  {
    RegisterFunction("Python",(MemberFunction)(&PythonModule::Evaluate), this);
    RegisterFunction("PyEvaluate",(MemberFunction)(&PythonModule::Evaluate), this);
  }
  {
    RegisterFunction("PyImport",(MemberFunction)(&PythonModule::Import), this);
  }
  {
    RegisterFunction("PyPutValue",(MemberFunction)(&PythonModule::PutValue), this);
    RegisterFunction("PyGetValue",(MemberFunction)(&PythonModule::GetValue), this);
  }
  {
    RegisterFunction("PyAddPath",(MemberFunction)(&PythonModule::AddPath), this);
  }
}

PythonModule::~PythonModule(){
  Finalize();
};

int PythonModule::Initialize(){
  if ( not IsInitialized() ){
    Py_Initialize();
    PyMain = PyImport_AddModule("__main__");
  }
  if ( not IsInitialized() )
    ThrowError("Python","Initialize","Can not Initialize Python Environment.");
  return 0;
}

int PythonModule::Finalize(){
  if ( Py_IsInitialized() )
    Py_Finalize();
  PyMain = NULL;
  return 0;
}

bool PythonModule::IsInitialized(){
  return Py_IsInitialized() and PyMain != NULL;
}

int PythonModule::Evaluate( string expr ){
  Initialize();
  PyRun_SimpleString( expr.c_str() );
  return 0;
}

int PythonModule::Evaluate(Object &ARGV){
  CheckShouldEqual(1);
  CheckShouldBeString(1);
  Initialize();
  PyRun_SimpleString(ARGV[1].Key());
  ReturnNull;
}

int PythonModule::PutValue(string name,Object&obj){
  Initialize();
  PyObject *v=NULL;
  List2PyObject( obj, v );
  PyObject_SetAttrString(PyMain,name.c_str(),v);
  Py_DECREF(v);
  return 0;
}


int PythonModule::PutValue(Object&ARGV){
  CheckShouldEqual(2);
  CheckShouldBeString(1);
  Initialize();
  PyObject *v=NULL;
  List2PyObject( ARGV[2], v );
  PyObject_SetAttrString(PyMain,ARGV[1].Key(),v);
  Py_DECREF(v);
  ReturnNull;
}

Object PythonModule::GetValue(string name){
  Initialize();
  PyObject *v = PyObject_GetAttrString(PyMain, name.c_str() );
  Object res;
  PyObject2List( v, res );
  Py_DECREF(v);
  return res;
}

int PythonModule::GetValue(Object&ARGV){
  CheckShouldEqual(1);
  CheckShouldBeString(1);
  Initialize();
  PyObject *v = PyObject_GetAttrString(PyMain, ARGV[1].Key() );
  Object res;
  PyObject2List( v, res );
  Py_DECREF(v);
  ARGV = res;
  ReturnNormal;
}


int PythonModule::Import(Object&ARGV){
  CheckShouldEqual(1);
  CheckShouldBeString(1);
  Initialize();
  PyImport_ImportModule( ARGV[1].Key() );
  ReturnNull;
}

int PythonModule::PyObject2List(PyObject*pyo,Object &list){
  if ( pyo == NULL or pyo == Py_None ){list.SetNull(); return 1;}
  else if ( PyLong_Check(pyo) ){list.SetNumber( PyLong_AsLong(pyo) ); return 1;}
  else if ( PyBool_Check(pyo) ){if ( pyo == Py_True ){list.SetSymbol( SYMID_OF_True );}else{list.SetSymbol( SYMID_OF_False );} return 1;}
  else if ( PyLong_Check(pyo) ){list.SetNumber( PyLong_AsLong(pyo) ); return 1;}
  else if ( PyFloat_Check(pyo) ){list.SetNumber( PyFloat_AsDouble(pyo) ); return 1;}
  else if ( PyComplex_Check(pyo) ){list.SetNumber( complex(PyComplex_RealAsDouble(pyo), PyComplex_ImagAsDouble(pyo)) ); return 1;}
  else if ( PyByteArray_Check(pyo) ){list.SetString( PyByteArray_AsString(pyo) ); return 1;}
  else if ( PyUnicode_Check(pyo) ){
    Py_ssize_t size;
    list.SetString( PyUnicode_AsUTF8AndSize(pyo,&size) );
    return 1;
  }else if ( PyTuple_Check(pyo) ){
    u_int size = PyTuple_Size( pyo );
    list.SetList();
    for ( u_int i=0;i<size;i++){
      list.PushBackNull();
      PyObject2List( PyTuple_GetItem( pyo, i), list[i+1] );
    }
    return 1;
  }else if( PyList_Check(pyo) ){
    u_int size = PyList_Size( pyo );
    list.SetList();
    for ( u_int i=0;i<size;i++){
      list.PushBackNull();
      PyObject2List( PyList_GetItem( pyo, i), list[i+1] );
    }
    return 1;
  }else if( PyDict_Check(pyo) ){
    u_int size = PyDict_Size( pyo );
    PyObject * items = PyDict_Items( pyo );
    list.SetList(SYMID_OF_Dict);
    for ( u_int i=0;i<size;i++){
      PyObject*item = PyList_GetItem( items, i );
      list.PushBackNull();
      Object key,value;
      PyObject2List( PyList_GetItem( item,0),key );
      PyObject2List( PyList_GetItem( item,1),value );
      list.Back().SetPairRef( key, value );
    }
    Py_DECREF( items );
    return 1;
  }

  list.SetSymbol("$UNKNOWN$Type$PyObject$");
  return 0;
}

int PythonModule::List2PyObject(Object &list,PyObject*&pyo){
  if ( list.NullQ() ){
    pyo = Py_None;
    Py_INCREF( pyo );
    return 1;
  }
  switch ( list.type() ){
  case ObjectType::Number:{
    pyo = PyFloat_FromDouble( list.Number() );
    return 1;
  }
  case ObjectType::String:{
    pyo = PyUnicode_FromString( list.Key() );
    return 1;
  }
  case ObjectType::Symbol:{
    ThrowError("List2PyObject","Un-determined Symbol is not allowed to transfer to Python as a value.");
  }
  case ObjectType::List:{
    if ( list[0].SymbolQ( SYMID_OF_Complex ) ){
      pyo = PyComplex_FromDoubles( list[1].Number(), list[2].Number() );
      return 1;
    }
    if ( list[0].SymbolQ( SYMID_OF_List ) ){
      pyo = PyList_New(0);
      for ( u_int i = 1;i<=list.Size(); i++ ){
        PyObject *item;
        List2PyObject( list[i], item );
        PyList_Append( pyo, item );
        Py_DECREF( item );
      }
      return 1;
    }
    if ( list[0].SymbolQ( SYMID_OF_Dict ) ){
      pyo = PyDict_New();
      for( u_int i=1; i<=list.Size(); i++ ){
        PyObject *key,*value;
        if ( not list[i].ListQ() or list[i].Size() < 2 ) continue;
        List2PyObject( list[i][1], key );
        List2PyObject( list[i][2], value );
        PyDict_SetItem( pyo, key, value );
      }
      return 1;
    }
  }
  }
  return 0;
}

bool PythonModule::AddPath( string path ){
  string cmd = "import sys\nsys.path.append(\""+path+"\")";
  Evaluate( cmd );
  return 0;
}

int PythonModule::AddPath(Object&ARGV){
  CheckShouldEqual(1);
  CheckShouldBeString(1);
  AddPath( ARGV[1].Key() );
  ReturnNull;
}

