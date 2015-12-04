
//////////////////////////////////////////////////////////////////////////
//
//     封装了用于同步的工具
//
//////////////////////////////////////////////////////////////////////////

#ifndef _WSL_THREAD_UTIL_H_
#define _WSL_THREAD_UTIL_H_


#include <vector>
#include "functor.h"

namespace wsl
{
//指定具体锁，必须有lock/unlock
template <typename T>
class Locker
{
private:
	T& m_obj;

	Locker(const Locker&);
	Locker& operator= (const Locker&);
public:

	Locker(T& obj):m_obj(obj)
	{
		m_obj.lock();
	}
	~Locker()
	{
		m_obj.lock();
	}
};

template <typename ReadWriteLockerT>
class ReadWriteLockerRL
{
	ReadWriteLockerT& m_cs;
	ReadWriteLockerRL(const ReadWriteLockerRL&);
	ReadWriteLockerRL& operator=(const ReadWriteLockerRL&);
public:
	ReadWriteLockerRL(ReadWriteLockerT& cs)
		: m_cs(cs)
	{
		m_cs.ReadLock();
	}

	~ReadWriteLockerRL()
	{
		m_cs.Unlock();
	}

};
template < typename ReadWriteLockerT >
class ReadWriteLockerWL
{
	ReadWriteLockerT& m_cs;

	ReadWriteLockerWL(const ReadWriteLockerWL&);
	ReadWriteLockerWL& operator=(const ReadWriteLockerWL&);
public:
	ReadWriteLockerWL(ReadWriteLockerT& cs)
		: m_cs(cs)
	{
		m_cs.WriteLock();
	}

	~ReadWriteLockerWL()
	{
		m_cs.Unlock();
	}
};

struct NullLocker
{
	bool Lock(){return true;}
	bool Unlock(){return true;}
	bool ReadLock(){return true;}
	bool WriteLock(){return true;}
};

template<typename T,typename LockT,typename HashT = wsl::hash<T> ,typename SequenceT = std::vector<LockT*> >
class LockPool
{
	HashT _hash;
	SequenceT _locks;
	bool _own;

	typedef typename SequenceT::iterator SIT;
public:
	typedef typename SequenceT::size_type size_type;
	explicit LockPool (const SequenceT& locks,bool own = false)
		:_locks(locks)
		,_own(own)
	{}
	explicit LockPool(size_type n)
		:_own(true)
	{
		_locks.resize(n);
		for (SIT i = _locks.begin();i !=_locks.end();++i)
		{
			*i = new LockT();
		}
	}
	~LockPool()
	{
		if (_own)
		{
			for (SIT i = _locks.begin();i !=_locks.end();++i)
			{
				delete *i;
			}
		}
	}


	HashT Hasher() const {return _hash;}

	void Lock(const T& v)
	{
		return LockH(_hash(v))
	}
	
	void UnLockH(const T& v)
	{
		return UnLockH(_hash(v));
	}

	void LockH(size_type hash)
	{
		size_type h = hash%_locks.size();
		_locks[h]->Lock();
	}
	void UnLockH(size_type hash)
	{
		size_type h = hash%_locks.size();
		_locks[h]->Unlock();
	}

};

}


#endif