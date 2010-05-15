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

#if GMOD_VERSION == 1
#define DebugMessage  Msg
#else
#define DebugMessage  m_luaInterface->Msg
#endif

int Test::normalTest()
{
  DebugMessage("Testing..Testing...1...2...3...\n");
  return 0;
}

int Test::callbackTest()
{
  DebugMessage("Running callback...\n");
  runCallback("onCallback", "si", "test", (int)m_currentTime);
  DebugMessage("...Done\n");
  return 0;
}

void Test::poll()
{
  time_t nowTime = time(0);
  if (m_currentTime != nowTime)
  {
    DebugMessage("Poll ... %d\n", nowTime);
    m_currentTime = nowTime;
  }
}
