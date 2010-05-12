
#include "GMLuaModule.h"
#include "CLASS_Test.h"

LUA_FUNCTION(Create)
{
  ILuaInterface* gLua = Lua();
  
  Test* obj = new Test(gLua);
  obj->pushObject();
  return 1;
}

int Init()
{
  ILuaInterface* gLua = Lua();

  LuaOO::instance()->registerPollingFunction(gLua, "TestOO::Poll");
  LuaOO::instance()->registerClasses(gLua);

  gLua->NewGlobalTable( "testoo" );
  ILuaObject* table = gLua->GetGlobal( "testoo" );
    table->SetMember("create", Create);
    table->UnReference();

  return 0;
}

int Shutdown()
{
  LuaOO::shutdown();
  return 0;
}

GMOD_MODULE( Init, Shutdown );
