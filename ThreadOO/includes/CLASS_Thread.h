#ifndef _CLASS_THREAD_H_
#define _CLASS_THREAD_H_

#include "PlatformSpecific.h"
#include <vector>

#include "CLASS_Mutex.h"
 
/*!
  \brief Thread class
*/
class Thread
{
public:
  /*!
    \brief Generic event notification
  */
  typedef struct
  {
    unsigned int id;
    void* data;
  } EventData;

  /*!
    \brief Constructor
  */
  Thread(void);

  /*!
    \brief Destructor
  */
  virtual ~Thread(void);

  /*!
    \brief Start the thread
  */
  virtual bool start();

  /*!
    \brief Wait for the thread to complete
  */
  virtual void wait();
  
  /*!
    \brief Called before the thread is run
  */
  virtual bool init();

  /*!
    \brief Activity to perform
  */
  virtual int run() = 0;

  /*!
    \brief Called after the thread has run
  */
  virtual void exit();

  /*!
    \brief Check if the thread is still running
  */
  inline bool isRunning() { return m_running; }

  /*!
    \brief Get the full list of posted events.
  */
  void getEvents(std::vector<EventData>& events);

  /*!
    \brief Get a single event.
  */
  bool getEvent(EventData& event);

  /*!
    \brief Post an event to the list
  */
  void postEvent(int eventID, void* data = 0);

private:
  /*!
    \brief Clear any variables when the thread finishes
  */
  void done();
  bool m_running;

#ifdef WIN32
  HANDLE m_thread;
  DWORD m_threadID;
  /*!
    \brief Thread entry point (windows)
  */
  static DWORD WINAPI ThreadProc(void* p);
#else
#error Unhandled Platform!
#endif

  Mutex m_eventList;
  std::vector<EventData> m_events;
};

#endif //_CLASS_THREAD_H_
