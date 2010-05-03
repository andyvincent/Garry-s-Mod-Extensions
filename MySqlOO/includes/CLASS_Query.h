#ifndef _CLASS_QUERY_H_
#define _CLASS_QUERY_H_

#include "CLASS_LuaObjectBase.h"
#include "LIBRARY_MySql.h"

class QueryThread;
class Database;
class DataRow;

enum
{
  QUERY_NOT_RUNNING = 0,
  QUERY_RUNNING,
  QUERY_READING_DATA,
  QUERY_COMPLETE,
  QUERY_ABORTED,
};

enum
{
  OPTION_NUMERIC_FIELDS = 1,
  OPTION_NAMED_FIELDS   = 2,
  OPTION_INTERPRET_DATA = 4,
  OPTION_CACHE          = 8,
};

class Query :
  public LuaObjectBaseTemplate<Query,0x43D2>
{
  LUA_OBJECT
public:
  Query(Database* dbase, ILuaInterface* luaInterface);
  virtual ~Query(void);

  void setQuery(const char* query);

  virtual void poll();

  bool threadRunning();

  int start();
  int isRunning();
  int getData();
  int abort();
  int lastInsert();
  int affectedRows();
  int status();
  int setOption();
private:
  inline bool testOption(int option)
  {
    return (m_options & option) == option;
  }

  Database* m_database;
  QueryThread* m_queryThread;
  std::vector<std::string> m_columns;
  std::vector<int> m_columnTypes;
  std::vector<DataRow*> m_allRows;
  int m_status;
  int m_options;

  void reset();

  void dataToLua(const std::string* data, ILuaObject* row, unsigned int column);
  ILuaObject* rowToLua(DataRow* row);
};

#endif //_CLASS_QUERY_H_
