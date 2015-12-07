/************************************************************************/
/* filename:			 blocking_queue.h                                                                  */
/* function:			阻塞方式的队列                                      */
/* author :			wangchenglin                                                                  */
/* creattime :       2015/12/04 下午                              */
/* company :		YY                                                                               */
/************************************************************************/
#ifndef _WSL_BLOCKING_QUEUE_H_
#define  _WSL_BLOCKING_QUEUE_H_

#include <queue>
#include <sys/time.h>
#include "pthread_util.h"

using namespace std;
namespace wsl
{
	inline void get_timespec_to(struct timespec& to,size_t timeout)
	{
		struct timeval now;
		gettimeofday(&now,NULL);
		to.tv_sec = now.tv_sec + timeout / 1000;
		to.tv_nsec = now.tv_usec * 1000 + (timeout % 1000) * 1000 * 1000;
		if (to.tv_nsec >= 1000 * 1000 * 1000) {
			++to.tv_sec;
			to.tv_nsec -= 1000 * 1000 * 1000;
		}
	}
template<typename T,typename Sequence = std::deque<T> >
class blocking_queue
{
	typedef std::queue<T,Sequence> QueueT;
	QueueT m_queue;
	mutable Mutex m_mutex;
	mutable Condition m_cond;

public:
	typedef T value_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;
	typedef typename QueueT::size_type size_type;

	blocking_queue(){}

	template < typename FI >
	blocking_queue(FI first, FI last)
	{
		MutexLocker lock(m_mutex);
		for(; first != last; ++ first)
			m_queue.push(*first);
	}
	void push(const T& e)
	{
		{
			MutexLocker ml(m_mutex);
			m_queue.push(e);
		}
		m_cond.Signal();	
	}

	T pop()
	{
		MutexLocker ml(m_mutex);
		while(m_queue.empty())
		{
			m_cond.Wait(m_mutex);	
		}
		T t = m_queue.front();
		m_queue.pop();
		return t;
	}

	bool pop(T& t,size_t timeout)
	{
		if (timeout ==0)
		{
			MutexLocker locker(m_mutex);
			if (m_queue.empty()) return false;
			t = m_queue.front();
			m_queue.pop();
			return true;
		}
		else
		{
			struct timespec to;
			get_timespec_to(to, timeout);
			MutexLocker locker(m_mutex);
			bool r=true;
			while(m_queue.empty()&&r)
				r = m_cond.Wait(m_mutex,to);
			if (r == false)
			return false;
			t = m_queue.front();
			m_queue.pop();
		}
		return true;
	}

	template<class Op>
	bool pop_if(T& result,Op op)
	{
		MutexLocker locker(m_mutex);
		while (m_queue.empty())
			m_cond.Wait(m_mutex);
		result = m_queue.front();
		bool ret = op(result);
		if (ret)
		{
			m_queue.pop();
		}
		return ret;
	}
	/// pop a element from queue if op(result)
	template < typename Op >
	bool pop_if(T& result, size_type timeout, Op op)
	{
		if (timeout == 0) {
			MutexLocker lock(m_mutex);
			if (m_queue.empty())
				return false;
			result = m_queue.front();
			if(op(result))
				m_queue.pop();
		} else {
			struct timespec to;
			get_timespec_to(to, timeout);
		
			bool r = true;
			MutexLocker lock(m_mutex);
			while(m_queue.empty() && r)
				r = m_cond.Wait(m_mutex, to);
			if(!r)
				return false;
			result = m_queue.front();
			if(op(result))
				m_queue.pop();
		}
		return true;
	}
	void clear() 
	{
		MutexLocker locker(m_mutex);
		while(m_queue.empty() !=true)
			m_queue.pop();
	}
	template < typename Op >
	void clear(Op op)
	{
		MutexLocker lock(m_mutex);
		while(!m_queue.empty()){
			op(m_queue.front());
			m_queue.pop();
		}
	}
	bool empty() const
	{
		MutexLocker lock(m_mutex);
		return m_queue.empty();
	}
	size_type size() const
	{
		MutexLocker lock(m_mutex);
		return m_queue.size();
	}

};



/** priority blocking queue
 */
template <typename T, typename Sequence = std::vector<T>,
		 typename Compare  = std::less<typename Sequence::value_type> >
class priority_blocking_queue
{
	typedef	std::priority_queue<T, Sequence, Compare> QueueT;
	QueueT				m_queue;	///< queue
	mutable Mutex		m_mutex;	///< mutex to lock queue
	mutable Condition	m_cond;		///< condition for mutex
public:
	typedef T			value_type;
	typedef T*			pointer;
	typedef const T*	const_pointer;
	typedef T& 			reference;
	typedef const T&	const_reference;
	typedef typename QueueT::size_type 		size_type;

	priority_blocking_queue()
	{}

	template < typename FI >
	priority_blocking_queue(FI first, FI last)
	{
		MutexLocker lock(m_mutex);
		for(; first != last; ++ first)
			m_queue.push(*first);
	}

	/** push a element to queue
	 *  \param e		the element to push
	 */
	void push(const T& e)
	{
		{
			MutexLocker lock(m_mutex);
			m_queue.push(e);
		}
		m_cond.Signal();
	}

	/// pop a element from queue, wait when empty
	T pop()
	{
		MutexLocker lock(m_mutex);
		while(m_queue.empty())
			m_cond.Wait(m_mutex);
		T t = m_queue.top();
		m_queue.pop();
		return t;
	}

	/** pop a element with timeout
	 *  \param t		element to pop
	 *  \param timeout	timeout in millisecond
	 *  \return			if element is poped, return true; otherwise timeout, return false
	 */
    bool pop(T& t, size_type timeout)
	{
		if (timeout == 0) {
			MutexLocker lock(m_mutex);
			if (m_queue.empty())
				return false;
			t = m_queue.top();
			m_queue.pop();
		} else {
			struct timespec to;
			get_timespec_to(to, timeout);
			
			bool r = true;
			MutexLocker lock(m_mutex);
			while(m_queue.empty() && r)
				r = m_cond.Wait(m_mutex, to);
			if(!r)
				return false;
			t = m_queue.top();
			m_queue.pop();
		}
		return true;
	}

	/// pop a element from queue if op(result)
	template < typename Op >
	bool pop_if(T& result, Op op)
	{
		MutexLocker lock(m_mutex);
		while(m_queue.empty())
			m_cond.Wait(m_mutex);
		result = m_queue.top();
		bool b = op(result);
		if(b)
			m_queue.pop();
		return b;
	}

	/// pop a element from queue if op(result)
	template < typename Op >
	bool pop_if(T& result, size_type timeout, Op op)
	{
		if (timeout == 0) {
			MutexLocker lock(m_mutex);
			if (m_queue.empty())
				return false;
			result = m_queue.top();
			if(op(result))
				m_queue.pop();
		} else {
			struct timespec to;
			get_timespec_to(to, timeout);
			
			bool r = true;
			MutexLocker lock(m_mutex);
			while(m_queue.empty() && r)
				r = m_cond.Wait(m_mutex, to);
			if(!r)
				return false;
			result = m_queue.top();
			if(op(result))
				m_queue.pop();
		}
		return true;
	}

	/// clear all
	void clear()
	{
		MutexLocker lock(m_mutex);
		while(!m_queue.empty())
			m_queue.pop();
	}

	template < typename Op >
	void clear(Op op)
	{
		MutexLocker lock(m_mutex);
		while(!m_queue.empty()){
			op(m_queue.top());
			m_queue.pop();
		}
	}

	/// is empty?
	bool empty() const
	{
		MutexLocker lock(m_mutex);
		return m_queue.empty();
	}

	/// get size of queue
	size_type size() const
	{
		MutexLocker lock(m_mutex);
		return m_queue.size();
	}
};
}
#endif
