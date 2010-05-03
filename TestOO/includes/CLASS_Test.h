#ifndef _CLASS_TEST_H_
#define _CLASS_TEST_H_

#include "CLASS_LuaObjectBase.h"

class Test : public LuaObjectBaseTemplate<Test, 7467>
{
  LUA_OBJECT
public:
  Test(ILuaInterface* li);

  int normalTest();
  int callbackTest();
  void poll();

private:
  time_t m_currentTime;
};

#endif _CLASS_TEST_H_
