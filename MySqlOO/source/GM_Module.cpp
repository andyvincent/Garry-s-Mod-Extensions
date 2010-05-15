
#include "GMLuaHeaders.h"

#include "CLASS_Database.h"
#include "CLASS_Query.h"

#define VERSION 5

LUA_FUNCTION(DatabaseConnect)
{
  /* Arguments:
     1 = host
     2 = username
     3 = password
     4 = database
     5 = port
     6 = unixSocket
     7 = clientFlag
  */

  ILuaInterface* gLua = Lua();
  
  int nNumParameters = gLua->Top();
  if (!LuaOO::checkArgument(gLua, 1, GLua::TYPE_STRING)) 
    return 0;
  if (!LuaOO::checkArgument(gLua, 2, GLua::TYPE_STRING)) 
    return 0;
  if (!LuaOO::checkArgument(gLua, 3, GLua::TYPE_STRING)) 
    return 0;

  const char* host = gLua->GetString(1);
  const char* user = gLua->GetString(2);
  const char* pass = gLua->GetString(3);
  const char* database = 0;
  unsigned int port = 0;
  const char* unixSocket = 0;
  unsigned int clientflag = 0;

  if (nNumParameters > 3)
  {
    if (!LuaOO::checkArgument(gLua, 4, GLua::TYPE_STRING)) 
      return 0;
    database = gLua->GetString(4);
  }
  if (nNumParameters > 4)
  {
    if (!LuaOO::checkArgument(gLua, 5, GLua::TYPE_NUMBER))
      return 0;
    port = (unsigned int)gLua->GetInteger(5);
  }
  if (nNumParameters > 5)  
  {
    if (!LuaOO::checkArgument(gLua, 6, GLua::TYPE_STRING)) 
      return 0;
    unixSocket = gLua->GetString(6);
  }
  if (nNumParameters > 6)  
  {
    if (!LuaOO::checkArgument(gLua, 7, GLua::TYPE_NUMBER)) 
      return 0;
    clientflag = (unsigned int)gLua->GetInteger(7);
  }

  Database* dbase = new Database(gLua);

  dbase->setHost(host);
  dbase->setUserPassword(user,pass);
  dbase->setDatabase(database);
  dbase->setPort(port);
  dbase->setUnixSocket(unixSocket);
  dbase->setClientFlag(clientflag);

  dbase->pushObject();
  return 1;
}

GMOD_FUNCTION(Init)
{
  ILuaInterface* gLua = Lua();

  LuaOO::instance()->registerPollingFunction(gLua, "MySqlOO::Poll");
  LuaOO::instance()->registerClasses(gLua);

  gLua->NewGlobalTable( "mysqloo" );
  ILuaObject* table = gLua->GetGlobal( "mysqloo" );

    table->SetMember("VERSION", (float)VERSION);
    table->SetMember("MYSQL_VERSION", (float)mysql_get_client_version());
    table->SetMember("MYSQL_INFO", mysql_get_client_info());

    table->SetMember("DATABASE_CONNECTED", (float)DATABASE_CONNECTED);
    table->SetMember("DATABASE_CONNECTING", (float)DATABASE_CONNECTING);
    table->SetMember("DATABASE_NOT_CONNECTED", (float)DATABASE_NOT_CONNECTED);
    table->SetMember("DATABASE_INTERNAL_ERROR", (float)DATABASE_INTERNAL_ERROR);

    table->SetMember("QUERY_NOT_RUNNING", (float)QUERY_NOT_RUNNING);
    table->SetMember("QUERY_RUNNING", (float)QUERY_RUNNING);
    table->SetMember("QUERY_READING_DATA", (float)QUERY_READING_DATA);
    table->SetMember("QUERY_COMPLETE", (float)QUERY_COMPLETE);
    table->SetMember("QUERY_ABORTED", (float)QUERY_ABORTED);

    table->SetMember("OPTION_NUMERIC_FIELDS", (float)OPTION_NUMERIC_FIELDS);
    table->SetMember("OPTION_NAMED_FIELDS", (float)OPTION_NAMED_FIELDS);
    table->SetMember("OPTION_INTERPRET_DATA", (float)OPTION_INTERPRET_DATA);
    table->SetMember("OPTION_CACHE", (float)OPTION_CACHE);

    table->SetMember("connect", DatabaseConnect);

    table->UnReference();

  return 0;
}

GMOD_FUNCTION(Shutdown)
{
  LuaOO::shutdown();
  return 0;
}

GMOD_MODULE( Init, Shutdown );

