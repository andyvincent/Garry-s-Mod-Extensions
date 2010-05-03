#include "CLASS_Thread.h"
#include "CLASS_MutexLocker.h"

#ifdef WIN32
DWORD WINAPI Thread::ThreadProc(void* p)
{
  Thread* thread = reinterpret_cast<Thread*>(p);
  if (!thread)
    return 0;
  int result = thread->run();
  thread->done();
  return result;
}
#else
#error Unhandled Platform!
#endif

Thread::Thread(void)
{
  m_running = false;
#ifdef WIN32
  m_thread = 0;
  m_threadID = 0;
#else
#error Unhandled Platform!
#endif
}

Thread::~Thread(void)
{
  wait();
}

bool Thread::start()
{
  if (isRunning())
    return false;

  m_running = true;

#ifdef WIN32
  m_thread = CreateThread(0, 0, &ThreadProc, this, CREATE_SUSPENDED, &m_threadID );
  if (m_thread == 0 || m_thread == INVALID_HANDLE_VALUE)
    return false;

  ResumeThread(m_thread);
#else
#error Unhandled Platform!
#endif

  return true;
}

void Thread::done()
{
#ifdef WIN32
  m_thread = 0;
  m_threadID = 0;
#else
#error Unhandled Platform!
#endif
  m_running = false;
}

void Thread::wait()
{
  if (!isRunning())
    return;
  
  while (true)
  {
#ifdef WIN32
    DWORD exitCode = 0;
    GetExitCodeThread(m_thread, &exitCode);
    if (exitCode == STILL_ACTIVE)
	    continue;
    Sleep(100);
#else
#error Unhandled Platform!
#endif
  }
}

void Thread::postEvent(int eventID, void* data)
{
  EventData ed = {0};
  ed.id = eventID;
  ed.data = data;

  MutexLocker lock(m_eventList);
  m_events.push_back(ed);
}

void Thread::getEvents(std::vector<EventData>& events)
{
  MutexLocker lock(m_eventList);
  events = m_events;
  m_events.clear();
}

bool Thread::getEvent(EventData& event)
{
  MutexLocker lock(m_eventList);
  if (m_events.empty())
    return false;

  event = *m_events.begin();
  m_events.erase( m_events.begin() );
  return true;
}
