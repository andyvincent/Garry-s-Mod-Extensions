#include "CLASS_LuaObjectBase.h"

LuaObjectBase::LuaObjectBase(const LuaClassInfo& classInfo, ILuaInterface* luaInterface)
  : m_luaInterface(luaInterface)
  , m_classInfo(classInfo)
  , m_enableGC(true)
{
  LuaOO::instance()->allocated(this);
}

LuaObjectBase::~LuaObjectBase()
{
  LuaOO::instance()->released(this);
}

int LuaObjectBase::toString()
{
  if (!m_luaInterface)
    return 0;

  m_luaInterface->PushVA("[%s:%08X]", m_classInfo.className(), this );
	return 1;
}

bool LuaObjectBase::isValid(bool bError)
{
	return true;
}

void LuaObjectBase::poll()
{
}

void LuaObjectBase::enableGC(bool enable)
{
  m_enableGC = enable;
}

bool LuaObjectBase::canDelete()
{
  return m_enableGC;
}

void LuaObjectBase::pushObject()
{
  if (!m_luaInterface)
    return;

  ILuaObject* metaTable = m_luaInterface->GetMetaTable(m_classInfo.className(), m_classInfo.typeId());
  if (!metaTable)
    return;

  m_luaInterface->PushUserData(metaTable, this);

  metaTable->UnReference();
}

LuaObjectBase* LuaObjectBase::getFromObject(ILuaInterface* luaInterface, ILuaObject* object, bool error)
{
  // Make sure it's some user data!
	if (!object->isUserData())
	{
		if (error)
			luaInterface->LuaError( "Invalid object! (not userdata)\n");
		return 0;
	}

  // Check the object
	if (!checkValidity(luaInterface, object->GetType(), (LuaObjectBase*)object->GetUserData(), error))
		return 0;
	
  // Just a simple cast required
	return reinterpret_cast<LuaObjectBase*>(object->GetUserData());
}

LuaObjectBase* LuaObjectBase::getFromStack(ILuaInterface* luaInterface, int position, bool error)
{
  // Check the object
	if (!checkValidity(luaInterface, position, error))
		return 0;
	
  // Just a simple cast required
	return reinterpret_cast<LuaObjectBase*>(luaInterface->GetUserData(position));
}

bool LuaObjectBase::checkValidity(ILuaInterface* luaInterface, int type, LuaObjectBase* object, bool error)
{
  // Make sure we recognise the type number
  if (!LuaOO::instance()->typeRegistered(type))
	{
		if (error)
			luaInterface->LuaError("Invalid object! (unknown type)\n");
    return false;
	}

  // Check for a NULL pointer
	if (object == 0)
	{
		if (error)
			luaInterface->LuaError("Invalid object! (null)\n");
		return false;
	}

  // Make sure it's something we've allocated
	if (!LuaOO::instance()->hasAllocated(object))
	{
		if (error)
			luaInterface->LuaError("Invalid object! (unreferenced)\n");
		return false;
	}

  // Check the type (this should always be correct)
	if (object->m_classInfo.typeId() != type)
	{
		if (error)
			luaInterface->LuaError("Invalid object! (types are different)\n");
		return false;
	}

  // Check the interface pointer (this should always be correct)
	if (object->m_luaInterface != luaInterface)
	{
		if (error)
			luaInterface->LuaError("Invalid object! (interface pointer is different)\n");
		return false;
	}

	return true;
}

bool LuaObjectBase::checkValidity(ILuaInterface* luaInterface, int position, bool error)
{
// Make sure it's some user data!
	if (!luaInterface->isUserData(position))
	{
		if (error)
		{
			luaInterface->LuaError( "Invalid object! (not userdata)\n");
		}
		return false;
	}

// Acually do the check
	int type = luaInterface->GetType(position);
	LuaObjectBase* object = reinterpret_cast<LuaObjectBase*>( luaInterface->GetUserData(position) );

	return checkValidity(luaInterface, type, object, error);
}

bool LuaObjectBase::checkArgument(int stackPosition, int expectedType)
{
  return LuaOO::checkArgument(m_luaInterface, stackPosition, expectedType);
}

void LuaObjectBase::runCallback(const char* functionName, const char* sig, ...)
{
  // Find the callback function.
  std::map<std::string,int>::iterator index = m_userTable.find( std::string(functionName) );
  if (index == m_userTable.end())
    return;
  int callbackFunction = index->second;

  // Get ourselves as a callback object
  ILuaObject* callbackObject = getAsObject();
  if (!callbackObject)
    return;

  m_luaInterface->PushReference(callbackFunction);
  m_luaInterface->Push(callbackObject);

  int numArguments = 1;
  if (sig)
  {
    va_list arguments;
    va_start(arguments, sig);
    for (unsigned int i=0; i<strlen(sig); i++)
    {
      char option = sig[i];
      if (option == 'i')
      {
        int value = va_arg(arguments, int);
        m_luaInterface->PushLong(value);
        numArguments++;
      }
      else if (option == 'f')
      {
        float value = static_cast<float>(va_arg(arguments, double));
        m_luaInterface->Push(value);
        numArguments++;
      }
      else if (option == 'b')
      {
        bool value = va_arg(arguments, int) != 0;
        m_luaInterface->Push(value);
        numArguments++;
      }
      else if (option == 's')
      {
        char* value = va_arg(arguments, char*);
        m_luaInterface->Push(value);
        numArguments++;
      }
      else if (option == 'o')
      {
        ILuaObject* value = va_arg(arguments, ILuaObject*);
        m_luaInterface->Push(value);
        numArguments++;
      }
      else if (option == 'r')
      {
        int reference = va_arg(arguments, int);
        m_luaInterface->PushReference(reference);
        numArguments++;
      }
      else if (option == 'F')
      {
        CLuaFunction value = va_arg(arguments, CLuaFunction);
        m_luaInterface->Push(value);
        numArguments++;
      }
    }
    va_end(arguments);
  }

  m_luaInterface->Call(numArguments);

  callbackObject->UnReference();
}

ILuaObject* LuaObjectBase::getAsObject()
{
// Push the object on to the stack
	pushObject();
	
// Get it as an ILuaObject
	ILuaObject* object = m_luaInterface->GetObject();

// Pop it off the stack & return
	m_luaInterface->Pop();
	return object;
}

CLuaFunction LuaObjectBase::lookupFunction(const char* name)
{
  const std::vector<LuaBoundFunction*>& bindings = m_classInfo.functionList();
  for(std::vector<LuaBoundFunction*>::const_iterator it = bindings.begin();
    it != bindings.end();
    ++it)
  {
    if ((*it)->isMetaFunction())
      continue;
    if (strcmp( (*it)->functionName(), name ) == 0)
      return (*it)->function();
  }
  return 0;
}

LUA_OBJECT_FUNCTION(LuaObjectBase::toStringWrapper)
{
	LuaObjectBase* object = getFromStack( Lua(), 1, false);
	if (!object)
  {
    Lua()->Push("[NULL Object]");
		return 1;
  }

	return object->toString();
}

LUA_OBJECT_FUNCTION(LuaObjectBase::indexWrapper)
{
	LuaObjectBase* object = getFromStack(Lua(), 1, false);
	if (!object)
  {
    const char* keyName = Lua()->GetString(2);
    if (strcmp(keyName, "isValid") == 0)
    {
      Lua()->Push( isValidWrapper );
      return 1;
    }
		return 0;
  }

  if (Lua()->GetType(2) == GLua::TYPE_STRING)
  {
    const char* keyName = Lua()->GetString(2);

    CLuaFunction function = object->lookupFunction(keyName);
    if (function)
    {
      Lua()->Push( function );
      return 1;
    }

    std::string key = std::string( keyName );
    std::map<std::string,int>::iterator index = object->m_userTable.find(key);
    if (index != object->m_userTable.end())
    {
      Lua()->PushReference( index->second );
      return 1;
    }
  }

  return 0;
}

LUA_OBJECT_FUNCTION(LuaObjectBase::newIndexWrapper)
{
	LuaObjectBase* object = getFromStack(Lua(), 1, false);
	if (!object)
		return 0;

  if (Lua()->GetType(2) == GLua::TYPE_STRING)
  {
    const char* keyName = Lua()->GetString(2);

    CLuaFunction function = object->lookupFunction(keyName);
    if (function)
    {
      Lua()->LuaError("Attempt to override meta function", 2);
      return 0;
    }

    std::string key = std::string( Lua()->GetString(2) );
    std::map<std::string,int>::iterator index = object->m_userTable.find(key);
    if (index != object->m_userTable.end())
    {
      Lua()->FreeReference(index->second);
      object->m_userTable.erase(index);
    }

    int ref = Lua()->GetReference(3);
    if (ref)
    {
      object->m_userTable[key] = ref;
    }
  }

  return 0;
}

LUA_OBJECT_FUNCTION(LuaObjectBase::enableGCWrapper)
{
	LuaObjectBase* object = getFromStack(Lua(), 1, false);
	if (!object)
	{
		return 0;
	}
  bool enable = true;
  if (Lua()->Top() >= 2)
  {
    if (!LuaOO::checkArgument(Lua(), 2, GLua::TYPE_BOOL))
      return 0;
    enable = Lua()->GetBool(2);
  }
	object->enableGC(enable);
	return 0;  
}

LUA_OBJECT_FUNCTION(LuaObjectBase::isValidWrapper)
{
	LuaObjectBase* object = getFromStack(Lua(), 1, false);
	if (!object)
	{
		Lua()->Push( false );
		return 1;
	}
	Lua()->Push( object->isValid(false) );
	return 1;
}

LUA_OBJECT_FUNCTION(LuaObjectBase::pollWrapper)
{
	LuaObjectBase* object = getFromStack(Lua(), 1, false);
	if (!object)
		return 0;
  object->poll();
	return 0;
}

LUA_OBJECT_FUNCTION(LuaObjectBase::deleteWrapper)
{
	LuaObjectBase* object = getFromStack(Lua(), 1, false);
	if (!object)
		return 0;
  delete object;
	return 0;
}

LUA_OBJECT_FUNCTION(LuaObjectBase::gcDeleteWrapper)
{
	LuaObjectBase* object = getFromStack(Lua(), 1, false);
	if (!object)
		return 0;
  if (!object->canDelete())
    return 0;
  delete object;
	return 0;
}

LUA_OBJECT_FUNCTION(LuaObjectBase::failureFunction)
{
  return 0;
}
