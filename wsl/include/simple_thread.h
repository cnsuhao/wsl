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
	
	pthread_t get_thread(){return m_pthread;}
private:
	Runnable* m_pRunnable;
	pthread_t m_pthread;

	bool      alive_;        ///< if the thread is still alive?
	bool      stop_;         ///< tell the thread to stop, reserved for future extension
	bool      release_;      ///< tell the thread to release thread class
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