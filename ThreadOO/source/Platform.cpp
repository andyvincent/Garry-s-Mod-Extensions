#include "Platform.h"

#include "CLASS_Thread.h"
/////////////////////////////////////////////////////////////////////////////////
// THREAD FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////

/*!
  \brief Thread function
  \param p Thread data
  \return Thread exit code.
*/
DWORD WINAPI _threadFunction(void* p)
{
  Thread* thread = reinterpret_cast<Thread*>(p);
  if (!thread)
    return 0;
  int result = thread->Run();
  thread->Done();
  return result;
}

/*!
  \brief Create a thread for the thread class.
  \param obj Class to run
  \param threadID Return thread id
  \return Thread handle.
*/
THREAD _createThread(Thread* obj, THREAD_ID& threadId)
{
  return CreateThread(0, 0, &_threadFunction, obj, CREATE_SUSPENDED, &threadId );
}

/*!
  \brief Start a thread running.
  \param thread Thread handle
*/
void _startThread(THREAD thread)
{
  ResumeThread(thread);
}

/*!
  \brief Check if a thread is still running
  \param thread Thread handle
  \return True or False if the thread is running.
*/
bool _isThreadRunning(THREAD thread)
{
	DWORD dwExit = 0;
	GetExitCodeThread(thread, &dwExit);
	return (dwExit == STILL_ACTIVE);
}

void _pause(unsigned long time)
{
  ::Sleep(time);
}
#endif
