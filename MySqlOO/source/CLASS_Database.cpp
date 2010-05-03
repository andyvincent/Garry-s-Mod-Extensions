#include "CLASS_Database.h"
#include "CLASS_ConnectThread.h"
#include "CLASS_Query.h"

Database::Database(ILuaInterface* luaInterface)
  : LuaObjectBaseTemplate<Database>(luaInterface)
{
  m_sql = mysql_init(0);
  m_connectionThread = new ConnectThread(m_sql);
}

Database::~Database(void)
{
  abortAllQueries();

  m_connectionThread->wait();
  delete m_connectionThread;
  m_connectionThread = 0;

  mysql_close(m_sql);
  m_sql = 0;
}

BEGIN_BINDING(Database)
	BIND_FUNCTION( connect )
	BIND_FUNCTION( query )
	BIND_FUNCTION( escape )
	BIND_FUNCTION( abortAllQueries )
	BIND_FUNCTION( status )
END_BINDING()

MYSQL* Database::sqlHandle()
{
  return m_sql;
}

void Database::setRunning(Query* query)
{
  m_runningQueries.push_back(query);
}

int Database::query()
{
  if (!checkArgument(2, GLua::TYPE_STRING))
    return 0;

	const char* query = m_luaInterface->GetString(2);
  if (!query)
    return 0;

  Query* queryObject = new Query(this, m_luaInterface);
  queryObject->setQuery(query);
  queryObject->pushObject();
  return 1;
}

void Database::poll()
{
  checkQueries();

  if (!m_connectionThread)
    return;

  Thread::EventData event;
  if (!m_connectionThread->getEvent(event))
    return;

  if (event.id == ConnectThread::CONNECTION_FINISHED)
  {
    if (m_connectionThread->wasSuccessful())
      runCallback("onConnected");
    else
      runCallback("onConnectionFailed", "s", m_connectionThread->error().c_str() );
  }
}

int Database::connect()
{
  if (m_connectionThread->isRunning())
    return 0;

  m_connectionThread->start();
  return 0;
}

int Database::escape()
{
  if (!checkArgument(2, GLua::TYPE_STRING))
    return 0;

	const char* sQuery = m_luaInterface->GetString(2);
  size_t nQueryLength = strlen(sQuery);

	char* sEscapedQuery = (char*)malloc( (nQueryLength * 2) + 4 );

	if (sEscapedQuery == 0)
		return 0;

	mysql_real_escape_string(m_sql, sEscapedQuery, sQuery, (unsigned long)nQueryLength );

  m_luaInterface->Push( sEscapedQuery );

  free(sEscapedQuery);
  return 1;
}

int Database::status()
{
  // Internal error!
  if (!m_connectionThread)
  {
    m_luaInterface->PushLong( DATABASE_INTERNAL_ERROR );
    return 1;
  }
  
  // Is still connecting
  if (m_connectionThread->isRunning())
  {
    m_luaInterface->PushLong( DATABASE_CONNECTING );
    return 1;
  }
  
  // Has connected, wasn't successful
  if (!m_connectionThread->wasSuccessful())
  {
    m_luaInterface->PushLong( DATABASE_NOT_CONNECTED );
    return 1;
  }

  if (mysql_ping(m_sql) == 0)
  {
    m_luaInterface->PushLong( DATABASE_CONNECTED );
  }
  else
  {
    m_luaInterface->PushLong( DATABASE_NOT_CONNECTED );
  }
  return 1;
}

void Database::checkQueries()
{
  for( std::vector<Query*>::iterator it = m_runningQueries.begin();
       it != m_runningQueries.end(); )
  {
    if (!(*it)->threadRunning())
    {
      it = m_runningQueries.erase(it);
    }
    else
    {
      it++;
    }
  }
}

int Database::abortAllQueries()
{
  for( std::vector<Query*>::iterator it = m_runningQueries.begin();
       it != m_runningQueries.end();
       ++it)
  {
    (*it)->abort();
  }

  while (!m_runningQueries.empty())
  {
    checkQueries();
    ::SleepEx(100, TRUE);
  }

  return 0;
}

void Database::setHost(const char* host)
{
  if (m_connectionThread->isRunning())
    return;
  m_connectionThread->setHost(host);
}

void Database::setUserPassword(const char* user, const char* pass)
{
  if (m_connectionThread->isRunning())
    return;
  m_connectionThread->setUserPassword(user,pass);
}

void Database::setDatabase(const char* database)
{
  if (m_connectionThread->isRunning())
    return;
  m_connectionThread->setDatabase(database);
}

void Database::setPort(unsigned int port)
{
  if (m_connectionThread->isRunning())
    return;
  m_connectionThread->setPort(port);
}

void Database::setUnixSocket(const char* socket)
{
  if (m_connectionThread->isRunning())
    return;
  m_connectionThread->setUnixSocket(socket);
}

void Database::setClientFlag(unsigned int flag)
{
  if (m_connectionThread->isRunning())
    return;
  m_connectionThread->setClientFlag(flag);
}