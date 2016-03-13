#include "simple_thread.h"
#include <assert.h>
#include <exception>
void* ThreadFun(void* lParam)
{
	Runnable* pRunnable = (Runnable*)lParam;
	pRunnable->run();
	return 0;
}

Runnable::Runnable()
{
}

Runnable::~Runnable()
{
}

MThread::MThread(Runnable* pRunnable)
:m_pRunnable(pRunnable), m_pthread(0),m_stop(false)
{
}

MThread::~MThread()
{
	if(m_pRunnable)
	{
		delete m_pRunnable;
		m_pRunnable = 0;
	}
}

bool MThread::start()
{
	if(0 == m_pRunnable)
	{
		return false;
	}

	if(pthread_create(&m_pthread, NULL, ThreadFun, (void*)m_pRunnable) != 0)
	{
		perror("pthread_create");
		return false;
	}
	return true;
}

void MThread::stop()
{
	pthread_cancel(m_pthread);
}

ThreadMutex::ThreadMutex()
{
	pthread_mutex_init(&m_pthread_mutex, 0);
}

ThreadMutex::~ThreadMutex()
{
	pthread_mutex_destroy(&m_pthread_mutex);
}

pthread_mutex_t* ThreadMutex::GetPMutex()
{
	return &m_pthread_mutex;
}

ThreadLock::ThreadLock(ThreadMutex* pThreadMutex)
:mp_pthread_mutex(pThreadMutex->GetPMutex())
{
	pthread_mutex_lock(mp_pthread_mutex);
}

ThreadLock::~ThreadLock()
{
	pthread_mutex_unlock(mp_pthread_mutex);
}

ThreadIf::ThreadIf():
m_pthread(0),
m_stop(false),
m_alive(false)
{
}

ThreadIf::~ThreadIf()
{
}

int ThreadIf::start()
{
	// if create successful, will return 0
	int err = pthread_create(&m_pthread, NULL,
		ThreadIf::thread_proc, (void*)(this));
	return err;
}

void ThreadIf::cancle()
{
	return ;
}

void* ThreadIf::thread_proc(void* arg)
{
	assert(arg);
	ThreadIf* self = (ThreadIf*)(arg);
	self->m_alive = true;
	try
	{
		self->run();
	}
	catch(...)
	{
		return (void*)-1;
	}
	self->m_alive = false;
	if (self->m_release)
	{
		delete self;
	}
	return (void*)0;
}

void ThreadIf::join()
{
	pthread_join(m_pthread, NULL);
	m_pthread = 0;
	m_alive = false;
}