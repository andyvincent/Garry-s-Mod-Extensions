#include "CLASS_Thread.h"
#include "CLASS_MutexLocker.h"

#ifdef WIN32
DWORD WINAPI Thread::threadProc(void* p)
{
  Thread* thread = reinterpret_cast<Thread*>(p);
  if (!thread)
    return 0;
  int result = 0;
  if (thread->init())
  {
    result = thread->run();
    thread->exit();
  }
  thread->done();
  return result;
}
#elif LINUX
void* Thread::threadProc(void* p)
{
  Thread* thread = reinterpret_cast<Thread*>(p);
  if (!thread)
    return 0;
  int result = 0;
  if (thread->init())
  {
    result = thread->run();
    thread->exit();
  }
  thread->done();
  return static_cast<void*>(result);
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
#elif LINUX
  m_thread = 0;
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
  m_thread = CreateThread(0, 0, &threadProc, this, CREATE_SUSPENDED, &m_threadID );
  if (m_thread == 0 || m_thread == INVALID_HANDLE_VALUE)
    return false;

  ResumeThread(m_thread);
#elif LINUX
  int error = pthread_create(&m_thread, NULL, &threadProc, this);
  if (error != 0)
    return false;
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
  
#ifdef WIN32
  WaitForSingleObject(m_thread, INFINITE);
#elif LINUX
  pthread_join(&m_thread, NULL);
#else
#error Unhandled Platform!
#endif
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

bool Thread::init()
{
  return true;
}

void Thread::exit()
{
}
