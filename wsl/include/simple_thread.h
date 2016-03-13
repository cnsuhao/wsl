/*	
	Ïß³Ì·â×°
	how to use:
	class MyClass:public Runnable
	{
		void run(){};
	}
	MyClass *pmyClass = new MyClass;
	MThread *pthread= new MThread(pmyClass);
	pthread->start();
 */

#ifndef _WSL_SIMPLE_THREAD_H_
#define _WSL_SIMPLE_THREAD_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

class Runnable
{
public:
	Runnable();
	virtual ~Runnable();

	virtual void run() = 0;

};

class MThread
{
public:
	MThread(Runnable* pRunnable);
	~MThread();

	bool start();
	void stop();
	void set_stop(){
			m_stop=true;
	}
	bool is_stop(){
		return m_stop;
	}
	
	pthread_t get_thread(){return m_pthread;}
private:
	Runnable* m_pRunnable;
	pthread_t m_pthread;
	bool      m_stop;         ///< tell the thread to stop, reserved for future extension

	bool      alive_;        ///< if the thread is still alive?
	
	bool      release_;      ///< tell the thread to release thread class
};


class ThreadIf
{
public:
	ThreadIf();
	virtual ~ThreadIf();

	int start();
	void cancle();
	
  /**
   * Make a flag to tell the thread stop running
   */
  inline void set_stop()
  {
    m_stop = true;
  }

  /**
   * Check if the thread should stop running
   */
  inline bool stop() const
  {
    return m_stop;
  }

  /*
   *  Make a flag to tell the thread release thread class
   */
  inline void set_release()
  {
    m_release = true;
  }

  /*
   *  thr thread detach self
   */
  inline void detach()
  {
    pthread_detach(m_pthread);
  }

	pthread_t get_thread(){return m_pthread;}
	void join();
private:
	/// disable some constructors and assignment operators
	ThreadIf(const ThreadIf& thr);
	ThreadIf& operator = (const ThreadIf& thr);

	/// thread start routine
	static void* thread_proc(void* arg);
protected:
	  /**
   * Override this virtual function to implement your own logic
   */
  virtual void run() = 0;
private:
	pthread_t m_pthread;
	bool      m_stop;         ///< tell the thread to stop, reserved for future extension
	bool      m_alive;        ///< if the thread is still alive?
	bool      m_release;      ///< tell the thread to release thread class
};

class ThreadMutex
{
public:
	ThreadMutex();
	~ThreadMutex();
	pthread_mutex_t* GetPMutex();
private:
	pthread_mutex_t m_pthread_mutex;
};

class ThreadLock
{
public:
	ThreadLock(ThreadMutex* pThreadMutex);
	~ThreadLock();
private:
	pthread_mutex_t* mp_pthread_mutex;
};



#endif