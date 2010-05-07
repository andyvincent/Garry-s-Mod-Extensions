#include "CLASS_Test.h"

#include <time.h>

Test::Test(ILuaInterface* li)
  : LuaObjectBaseTemplate<Test>(li)
{
  m_currentTime = time(0);
}

BEGIN_BINDING(Test)
  BIND_FUNCTION("normalTest",Test::normalTest)
  BIND_FUNCTION("callbackTest",Test::callbackTest)
END_BINDING()

int Test::normalTest()
{
  m_luaInterface->Msg("Testing..Testing...1...2...3...\n");
  return 0;
}

int Test::callbackTest()
{
  m_luaInterface->Msg("Running callback...\n");
  runCallback("onCallback", "si", "test", (int)m_currentTime);
  m_luaInterface->Msg("...Done\n");
  return 0;
}

void Test::poll()
{
  time_t nowTime = time(0);
  if (m_currentTime != nowTime)
  {
    m_luaInterface->Msg("Poll ... %d\n", nowTime);
    m_currentTime = nowTime;
  }
}
