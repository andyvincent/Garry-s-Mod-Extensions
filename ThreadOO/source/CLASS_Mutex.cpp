
#include "CLASS_Mutex.h"

Mutex::Mutex()
{
#ifdef WIN32
  InitializeCriticalSection(&m_mutex);
#else
#error Unhandled Platform!
#endif
}

Mutex::~Mutex()
{
#ifdef WIN32
  DeleteCriticalSection(&m_mutex);
#else
#error Unhandled Platform!
#endif
}

void Mutex::lock()
{
#ifdef WIN32
  EnterCriticalSection(&m_mutex);
#else
#error Unhandled Platform!
#endif
}

void Mutex::unLock()
{
#ifdef WIN32
  LeaveCriticalSection(&m_mutex);
#else
#error Unhandled Platform!
#endif
}
