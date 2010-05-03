
#include "CLASS_Event.h"

Event::Event()
{
#ifdef WIN32
  m_event = CreateEvent(0, TRUE, FALSE, 0);
#else
#error Unhandled Platform!
#endif
}

Event::~Event()
{
#ifdef WIN32
  CloseHandle(m_event);
#else
#error Unhandled Platform!
#endif
}

void Event::signal()
{
#ifdef WIN32
  SetEvent(m_event);
#else
#error Unhandled Platform!
#endif
}

void Event::clear()
{
#ifdef WIN32
  ResetEvent(m_event);
#else
#error Unhandled Platform!
#endif
}

bool Event::wait()
{
#ifdef WIN32
  return (WaitForSingleObject(m_event, INFINITE) == WAIT_OBJECT_0);
#else
#error Unhandled Platform!
#endif
}

bool Event::poll()
{
#ifdef WIN32
  return (WaitForSingleObject(m_event, 0) == WAIT_OBJECT_0);
#else
#error Unhandled Platform!
#endif
}

