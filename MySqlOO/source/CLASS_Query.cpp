#include "CLASS_Query.h"
#include "CLASS_QueryThread.h"
#include "CLASS_Database.h"
#include "CLASS_DataRow.h"

Query::Query(Database* dbase, ILuaInterface* pLua)
  : LuaObjectBaseTemplate<Query>(pLua)
  , m_database(dbase)
{
  m_options = OPTION_NAMED_FIELDS | OPTION_INTERPRET_DATA | OPTION_CACHE;
  m_status = QUERY_NOT_RUNNING;
  m_queryThread = new QueryThread( m_database );
}

Query::~Query(void)
{
  reset();
  delete m_queryThread;
  m_queryThread = 0;
}

BEGIN_BINDING(Query)
	BIND_FUNCTION(start)
	BIND_FUNCTION(isRunning)
	BIND_FUNCTION(getData)
	BIND_FUNCTION(abort)
	BIND_FUNCTION(lastInsert)
  BIND_FUNCTION(status)
  BIND_FUNCTION(affectedRows)
  BIND_FUNCTION(setOption)
END_BINDING()

bool Query::canDelete()
{
  if (m_queryThread->isRunning())
    return false;
  if (m_queryThread->hasEvents())
    return false;
  return LuaObjectBaseTemplate<Query>::canDelete();
}

void Query::setQuery(const char* query)
{
  m_queryThread->setQuery(query);
}

void Query::reset()
{
  if(m_queryThread->checkAbort())
    m_queryThread->abort();

  m_queryThread->wait();

  std::vector<Thread::EventData> events;
  m_queryThread->getEvents(events);

  for( std::vector<Thread::EventData>::iterator eventIterator = events.begin();
       eventIterator != events.end();
       ++eventIterator)
  {
    if (eventIterator->id == QueryThread::QUERY_DATA)
    {
      DataRow* row = reinterpret_cast<DataRow*>(eventIterator->data);
      delete row;
    }
  }

  m_columns.clear();
  for( std::vector<DataRow*>::iterator rowIterator = m_allRows.begin();
       rowIterator != m_allRows.end();
       ++rowIterator)
  {
    DataRow* row = reinterpret_cast<DataRow*>(*rowIterator);
    delete row;
  }
  m_allRows.clear();
}

int Query::start()
{
  reset();
  m_database->setRunning(this);
  m_queryThread->start();
  m_status = QUERY_RUNNING;
  return 0;
}

bool Query::threadRunning()
{
  if (!m_queryThread)
    return false;

  return m_queryThread->isRunning();
}

int Query::isRunning()
{
  if (!m_queryThread)
    return 0;
  
  m_luaInterface->Push( m_queryThread->isRunning() );
  return 1;
}

int Query::lastInsert()
{
  if (!m_queryThread)
    return 0;

  m_luaInterface->PushLong( m_queryThread->lastInsertId() );
  return 1;
}

int Query::affectedRows()
{
  if (!m_queryThread)
    return 0;

  m_luaInterface->PushLong( m_queryThread->affectedRows() );
  return 1;
}

int Query::setOption()
{
  if (!checkArgument(2, GLua::TYPE_NUMBER))
    return 0;
  
  bool set = true;
  int option = m_luaInterface->GetInteger(2);
  if (option != OPTION_NUMERIC_FIELDS && 
      option != OPTION_NAMED_FIELDS && 
      option != OPTION_INTERPRET_DATA &&
      option != OPTION_CACHE)
  {
    m_luaInterface->LuaError("Invalid option", 2);
    return 0;
  }

  if (m_luaInterface->Top() >= 3)
  {
    if (!checkArgument(3, GLua::TYPE_BOOL))
      return 0;
    set = m_luaInterface->GetBool(3);
  }
  
  if (set)
  {
    m_options |= option;
  }
  else
  {
    m_options &= ~option;
  }
  return 0;
}

int Query::getData()
{
	ILuaObject* table = m_luaInterface->GetNewTable();
  
  float rowNumber = 1;
  for( std::vector<DataRow*>::iterator it = m_allRows.begin();
       it != m_allRows.end();
       ++it)
  {
		ILuaObject* rowObject = rowToLua( *it );
		table->SetMember(rowNumber, rowObject);
    rowObject->UnReference();
    
    rowNumber++;
  }

  table->Push();
  table->UnReference();

  return 1;
}

int Query::status()
{
  m_luaInterface->PushLong(m_status);
  return 1;
}

int Query::abort()
{
  if (!m_queryThread)
    return 0;
  m_queryThread->abort();
  return 0;
}

void Query::dataToLua(const std::string* data, ILuaObject* row, unsigned int column)
{
  if (column < m_columns.size())
  {
    int type = m_columnTypes[column];

    if (type == QueryThread::STRING || (!testOption(OPTION_INTERPRET_DATA)))
    {
      if (testOption(OPTION_NUMERIC_FIELDS))
        row->SetMember((float)column+1, data->c_str() );
      if (testOption(OPTION_NAMED_FIELDS))
        row->SetMember(m_columns[column].c_str(), data->c_str() );
    }
    else if (type == QueryThread::INTEGER)
    {
      int value = atoi( data->c_str() );
      if (testOption(OPTION_NUMERIC_FIELDS))
        row->SetMember((float)column+1, (float)value );
      if (testOption(OPTION_NAMED_FIELDS))
        row->SetMember(m_columns[column].c_str(), (float)value );
    }
    else if (type == QueryThread::FLOATING_POINT)
    {
      double value = atof( data->c_str() );
      if (testOption(OPTION_NUMERIC_FIELDS))
        row->SetMember((float)column+1, (float)value );
      if (testOption(OPTION_NAMED_FIELDS))
        row->SetMember(m_columns[column].c_str(), (float)value );
    }
  }
  else
  {
    if (testOption(OPTION_NUMERIC_FIELDS))
      row->SetMember((float)column+1, data->c_str() );
  }
}

ILuaObject* Query::rowToLua(DataRow* row)
{
  ILuaObject* rowObject = m_luaInterface->GetNewTable();
  
  unsigned int column = 0;
  for( DataRow::iterator it = row->begin();
       it != row->end();
       ++it)
  {
	  if (*it)
	  {
      dataToLua(*it, rowObject, column);
	  }

    column++;
  }

  return rowObject;
}

void Query::poll()
{
  if (!m_queryThread)
    return;

  std::vector<Thread::EventData> events;
  m_queryThread->getEvents(events);

  for( std::vector<Thread::EventData>::iterator it = events.begin();
       it != events.end();
       ++it)
  {
    switch (it->id)
    {
    case QueryThread::QUERY_ABORTED:
      runCallback("onAborted");
      m_status = QUERY_ABORTED;
      break;

    case QueryThread::QUERY_ERROR:
      if (m_queryThread->checkAbort())
        continue;
      m_status = QUERY_COMPLETE;
      runCallback("onError", "s", m_queryThread->error().c_str());
      break;

    case QueryThread::QUERY_SUCCESS:
    case QueryThread::QUERY_SUCCESS_NO_DATA:
      if (m_queryThread->checkAbort())
        continue;
      m_status = QUERY_COMPLETE;
      runCallback("onSuccess");
      break;

    case QueryThread::QUERY_COLUMNS:
      if (m_queryThread->checkAbort())
        continue;
      m_columns = m_queryThread->columns();
      m_columnTypes = m_queryThread->columnTypes();
      break;

    case QueryThread::QUERY_DATA:
      {
        m_status = QUERY_READING_DATA;
        DataRow* row = reinterpret_cast<DataRow*>(it->data);
        if (!row)
          break;

        if (testOption(OPTION_CACHE))
          m_allRows.push_back(row);

        if (m_queryThread->checkAbort())
          continue;

        ILuaObject* dataChunk = rowToLua(row);
        runCallback("onData", "o", dataChunk);
        dataChunk->UnReference();

        if (!testOption(OPTION_CACHE))
          delete row;
      }
    }
  }
}
