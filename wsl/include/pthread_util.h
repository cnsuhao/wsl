/* ************************************************************************
 *       Filename:  pthread_util.h
 *       Description: 封装一些pthread的类
 *       Version:  1.0
 *       Created:  2015年12月04日 17时49分29秒
 *       Revision:  NONE
 *       Compiler:  g++
 *       Author:  wochenglin@qq.com
 *       Company:  YY
* ************************************************************************/
#ifndef _WSL_PTHREAD_UTIL_H_
#define _WSL_PTHREAD_UTIL_H_

#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include "functor.h"
#include "thread_util.h"

#if !defined(linux) && (defined(__linux) || defined(__linux__))
#define  linux
#endif

namespace wsl
{
	class Mutex
	{
	private:
		pthread_mutex_t m_mutex;

		Mutex(Mutex&);
		Mutex& operator= (const Mutex&);
	public:
		Mutex(){
			::pthread_mutex_init(&m_mutex,NULL);
		}
		~Mutex(){
			::pthread_mutex_destroy(&m_mutex);
		}
		bool Lock(){ return 0 == ::pthread_mutex_lock(&m_mutex);}
		bool Unlock(){ return 0 == ::pthread_mutex_unlock(&m_mutex);}
		bool TryLock(){return 0 ==::pthread_mutex_trylock(&m_mutex);}

		pthread_mutex_t* get_mutex()
		{
			return &m_mutex;
		}
#ifdef _POSIX_TIMEOUTS
#	if _POSIX_TIMEOUTS >= 0
		bool Lock(const struct timespec& abstime)	{ return 0 == ::pthread_mutex_timedlock(&m_mutex, &abstime); }
#	endif
#endif

	};
	typedef wsl::Locker<Mutex> MutexLocker;


	class Condition
	{
		pthread_cond_t m_cond;
		Condition(Condition&);
		Condition& operator=(Condition&);
	public:
		Condition() {::pthread_cond_init(&m_cond,NULL);}
		~Condition(){
			::pthread_cond_destroy(&m_cond);
		}

		bool Signal() {return 0 ==::pthread_cond_signal(&m_cond);}
		bool Broadcast() {return 0 ==::pthread_cond_broadcast(&m_cond);}
		bool Wait(Mutex& m){return 0 ==::pthread_cond_wait(&m_cond,m.get_mutex());}
		bool Wait(Mutex& m,const struct timespec&  abstime)
		{
			int r;
			do{ 
				r = ::pthread_cond_timedwait(&m_cond, m.get_mutex(), &abstime);
			}while(r != 0 && r != ETIMEDOUT && r != EINVAL);
			return r == 0;

		}
	};

	#if !defined(linux) || (defined __USE_UNIX98 || defined __USE_XOPEN2K)
class ReadWriteLocker
{
private:
	pthread_rwlock_t m_sect;

	ReadWriteLocker(const ReadWriteLocker&);
	ReadWriteLocker& operator=(const ReadWriteLocker&);
public:
	ReadWriteLocker()			{ ::pthread_rwlock_init(&m_sect, NULL); }
	~ReadWriteLocker()			{ ::pthread_rwlock_destroy(&m_sect); }
	bool ReadLock()				{ return 0 == ::pthread_rwlock_rdlock(&m_sect); }
	bool WriteLock()			{ return 0 == ::pthread_rwlock_wrlock(&m_sect); }
	bool TryReadLock()			{ return 0 == ::pthread_rwlock_tryrdlock(&m_sect); }
	bool TryWriteLock()			{ return 0 == ::pthread_rwlock_trywrlock(&m_sect); }
	bool Unlock()				{ return 0 == ::pthread_rwlock_unlock(&m_sect); }
#ifdef _POSIX_TIMEOUTS
#	if _POSIX_TIMEOUTS >= 0
	bool ReadLock(const struct timespec& abstime)	{ return 0 == ::pthread_rwlock_timedrdlock(&m_sect, &abstime); }
	bool WriteLock(const struct timespec& abstime)	{ return 0 == ::pthread_rwlock_timedwrlock(&m_sect, &abstime); }
#	endif
#endif
};

typedef wsl::ReadWriteLockerRL<ReadWriteLocker>	ReadLocker;
typedef wsl::ReadWriteLockerWL<ReadWriteLocker>	WriteLocker;

#endif
#if (defined(__USE_XOPEN2K) || !defined(linux)) && !defined(__OpenBSD__)

class SpinLock
{
private:
	pthread_spinlock_t m_sect;
	SpinLock(SpinLock&);
	SpinLock& operator=(const SpinLock& );
public:
	explicit SpinLock(int pshared = PTHREAD_PROCESS_PRIVATE)			{ ::pthread_spin_init(&m_sect, pshared); }
	~SpinLock()					{ ::pthread_spin_destroy(&m_sect); }
	bool Lock()					{ return 0 == ::pthread_spin_lock(&m_sect); }
	bool TryLock()				{ return 0 == ::pthread_spin_trylock(&m_sect); }
	bool Unlock()				{ return 0 == ::pthread_spin_unlock(&m_sect); }
};
typedef wsl::Locker<SpinLock> SpinLocker;

class Barrier
{
private:
	pthread_barrier_t m_sect;
	
	Barrier(const Barrier&);
	Barrier& operator=(const Barrier&);
public:
	Barrier(unsigned int count)	{ ::pthread_barrier_init(&m_sect, NULL, count); }
	~Barrier()						{ ::pthread_barrier_destroy(&m_sect); }
	int Wait()						{ return ::pthread_barrier_wait(&m_sect); }

};

	
#endif

class Semaphore
{
	sem_t	m_sem;

	Semaphore(const Semaphore&);
	Semaphore& operator=(const Semaphore&);
public:
	explicit Semaphore(int pshared = 0, unsigned int value = 0)	{ ::sem_init(&m_sem, pshared, value); }
	~Semaphore()								{ ::sem_destroy(&m_sem); }

	bool Post()									{ return 0 == ::sem_post(&m_sem); }
	void Wait()									{ ::sem_wait(&m_sem); }
#if _POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600
	bool Wait(const struct timespec& abstime)	{ return 0 == ::sem_timedwait(&m_sem, &abstime); }
#endif
	bool TryWait()								{ return 0 == ::sem_trywait(&m_sem); }
	int GetValue()
	{
		int val = 0;
		::sem_getvalue(&m_sem, &val);
		return val;
	}
};
}
#endif

