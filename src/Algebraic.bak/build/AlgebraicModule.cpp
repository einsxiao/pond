#define ModuleName Algebraic
#define ModuleName Algebraic
#include<pond.h>
#include"AlgebraicModule.h"
using namespace pond;

ModuleConstruction{
  RegisterFunction("SimpleSimplify",(MemberFunction)(&AlgebraicModule::SimpleSimplify),this );
}

ModuleDestruction{
};






