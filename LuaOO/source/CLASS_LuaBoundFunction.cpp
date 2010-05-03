#include "CLASS_LuaBoundFunction.h"

LuaBoundFunction::LuaBoundFunction(const char* name, CLuaFunction fptr, bool meta)
  : m_functionName(name)
  , m_function(fptr)
  , m_isMetaFunction(meta)
{
}

