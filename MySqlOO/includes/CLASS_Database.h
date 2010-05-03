#ifndef _CLASS_DATABASE_H_
#define _CLASS_DATABASE_H_

#include "MODULE_LuaOO.h"
#include "LIBRARY_MySql.h"
#include <vector>

class ConnectThread;
class Query;

enum
{
  DATABASE_CONNECTED = 0,
  DATABASE_CONNECTING,
  DATABASE_NOT_CONNECTED,
  DATABASE_INTERNAL_ERROR,
};

class Database :
  public LuaObjectBaseTemplate<Database, 0x781A>
{
  LUA_OBJECT
public:
  Database(ILuaInterface* luaInterface);
  virtual ~Database(void);

  virtual void poll();

  void setHost(const char* host);
  void setUserPassword(const char* user, const char* pass);
  void setDatabase(const char* database);
  void setPort(unsigned int port);
  void setUnixSocket(const char* socket);
  void setClientFlag(unsigned int port);

  int connect();
  int query();
  int escape();
  int abortAllQueries();
  int status();

  MYSQL* sqlHandle();
  void setRunning(Query*);
private:
  void checkQueries();

  MYSQL* m_sql;
  ConnectThread* m_connectionThread;
  std::vector<Query*> m_runningQueries;
};

#endif //_CLASS_DATABASE_H_
