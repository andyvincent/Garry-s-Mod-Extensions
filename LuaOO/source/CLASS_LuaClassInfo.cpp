#include "CLASS_LuaClassInfo.h"

LuaClassInfo::LuaClassInfo(const char* className, int typeId, std::vector<LuaBoundFunction*> functionList)
  : m_className(className)
  , m_typeId(typeId)
  , m_functionList(functionList)
{
  LuaOO::instance()->addClass(this);
}