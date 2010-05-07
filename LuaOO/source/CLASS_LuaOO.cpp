#include "CLASS_LuaOO.h"
#include "CLASS_LuaClassInfo.h"
#include "CLASS_LuaObjectBase.h"
#include <sstream>

LuaOO::LuaOO(void)
{
}

LuaOO::~LuaOO(void)
{
}

LuaOO* LuaOO::s_instance = 0;

LuaOO* LuaOO::instance()
{
  if (!s_instance)
    s_instance = new LuaOO;
  return s_instance;
}

void LuaOO::shutdown()
{
  if (!s_instance)
    return;

  s_instance->deallocateAll();
  delete s_instance;
  s_instance = 0;
}

void LuaOO::registerClass(ILuaInterface* gLua, LuaClassInfo* classInfo)
{
  if (isRegistered(classInfo))
    return;

// Make the meta table
	ILuaObject* pMeta = gLua->GetMetaTable( classInfo->className(), classInfo->typeId() );
  pMeta->SetMember("__gc", LuaObjectBase::gcDeleteWrapper );
  pMeta->SetMember("__tostring", LuaObjectBase::toStringWrapper);
	pMeta->SetMember("__index", LuaObjectBase::indexWrapper );
  pMeta->SetMember("__newindex", LuaObjectBase::newIndexWrapper );

// Cleanup
  pMeta->UnReference();

// Register the type with our list
	registerType( classInfo );
}

void LuaOO::registerClasses(ILuaInterface* gLua)
{
  for(std::vector<LuaClassInfo*>::iterator it = m_classes.begin();
      it != m_classes.end();
      ++it)
  {
    registerClass(gLua, *it);
  }
}

void LuaOO::addClass(LuaClassInfo* ci)
{
  m_classes.push_back(ci);
}

bool LuaOO::isRegistered(LuaClassInfo* classInfo)
{
  return typeRegistered(classInfo->typeId());
}

void LuaOO::registerType(LuaClassInfo* classInfo)
{
  m_registered.insert(classInfo->typeId());
}

bool LuaOO::typeRegistered(int typeId)
{
  std::set<int>::iterator it = m_registered.find(typeId);
  if (it == m_registered.end())
    return false;
  return true;
}

void LuaOO::allocated(LuaObjectBase* object)
{
  m_allocated.insert(object);
}

bool LuaOO::hasAllocated(LuaObjectBase* object)
{
  std::set<LuaObjectBase*>::iterator it = m_allocated.find(object);
  if (it == m_allocated.end())
    return false;
  return true;
}

void LuaOO::released(LuaObjectBase* object)
{
  std::set<LuaObjectBase*>::iterator it = m_allocated.find(object);
  if (it == m_allocated.end())
    return;
  m_allocated.erase(it);
}

void LuaOO::deallocateAll()
{
  while (!m_allocated.empty())
  {
    delete (*m_allocated.begin());
  }
}

bool LuaOO::checkArgument(ILuaInterface* luaInterface, int stackPosition, int expectedType)
{
  if (!luaInterface)
    return false;

  int passedType = luaInterface->GetType(stackPosition);
  if (passedType == expectedType)
    return true;

  const char* passedTypeName = luaInterface->GetTypeName(passedType);
  const char* expectedTypeName = luaInterface->GetTypeName(expectedType);
  
  if (passedType == GLua::TYPE_NIL)
    passedTypeName = "no value";
  else if (passedTypeName == 0)
    passedTypeName = "<unknown type>";

  if (expectedTypeName == 0)
    expectedTypeName = "<unknown type>";

  std::stringstream errorText;
  errorText << expectedTypeName << " expected, got " << passedTypeName << "\n";
  luaInterface->LuaError(errorText.str().c_str(), stackPosition);
  return false;
}

LUA_OBJECT_FUNCTION(LuaOO::pollingFunction)
{
  if (!s_instance)
    return 0;

  std::set<LuaObjectBase*> copy(s_instance->m_allocated);
  for(std::set<LuaObjectBase*>::iterator it = copy.begin();
      it != copy.end();
      ++it)
  {
    (*it)->poll();
  }
  return 0;
}

void LuaOO::registerPollingFunction(ILuaInterface* luaInterface, const char* name)
{
  ILuaObject* hookModule = luaInterface->GetGlobal("hook");
	ILuaObject* addFunction = hookModule->GetMember("Add");

  addFunction->Push();
  luaInterface->Push("Think");
  luaInterface->Push(name);
  luaInterface->Push(pollingFunction);

  luaInterface->Call(3);

  addFunction->UnReference();
  hookModule->UnReference();
}