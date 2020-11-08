#include"pd_headers.h"
using namespace std;
using namespace pond;

///////////////////////////////////////////////////////////
Module::Module(string name){
  this->moduleName = name;
  EvaKernel->InsertModule(moduleName,this);
  EvaKernel->SetAttribute(moduleName.c_str(),AttributeType::Protected);
};

Module::~Module(){
};

string Module::GetModuleName(){return moduleName;}

//string Module::GetModuleDirectory(){return moduleDirectory;}

//int Module::SetModuleDirectory(string dir){ moduleDirectory = dir; return 0; }

#include<string.h>
int Module::RegisterFunction(const char* funcName, MemberFunction func,const Module*mod,const char*descri){
  Object temp; temp.SetSymbol( funcName );
  EvaRecord *rec = EvaKernel->GetOrNewEvaRecord(temp);
  SetAttribute(rec->attributes,AttributeType::Protected);
  rec->staticFunction = NULL;
  rec->module = (Module*)mod;
  rec->memberFunction = func;
  rec->description = descri;
  return 0;
}

int Module::RegisterFunction(const char*funcName, StaticFunction func,const Module*mod,const char*descri){
  Object temp; temp.SetSymbol( funcName );
  EvaRecord *rec = EvaKernel->GetOrNewEvaRecord(temp);
  SetAttribute(rec->attributes,AttributeType::Protected);
  rec->staticFunction = func;
  rec->module = NULL;
  rec->memberFunction = NULL;
  rec->description = descri;
  return 0;
}

int Module::AddAttribute(const char*funcName, const char* attribute){
  EvaKernel->SetAttribute(funcName,pond::String2AttributeType(attribute) );
  return 0;
}

int Module::AddAttribute(const char*funcName, const AttributeType attribute){
  EvaKernel->SetAttribute(funcName,attribute );
  return 0;
}

int Module::SetModuleDescription(string des){
  moduleDescreption = des;
  return 0;
}

string Module::ModuleDescription(){
  return moduleDescreption;
}



