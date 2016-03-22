#ifndef _WSL_BOUND_BLOCKING_QUEUE_H_
#define _WSL_BOUND_BLOCKING_QUEUE_H_

#include <queue>
#include <vector>
#include <sys/time.h>
#include "pthread_util.h"
using namespace std;
namespace wsl
{
#define  BOUND_QUEUE_SIZE 2000
	template <typename T>
	class BoundedBlockingQueue 
	{ 
	public: 
		BoundedBlockingQueue (int size) : maxSize(size) 
		{ 
			pthread_mutex_init(&_lock, NULL); 
			pthread_cond_init(&_rcond, NULL);
			pthread_cond_init(&_wcond, NULL);
			_array.reserve(maxSize);
		} 
		BoundedBlockingQueue () : maxSize(BOUND_QUEUE_SIZE) 
		{ 
			pthread_mutex_init(&_lock, NULL); 
			pthread_cond_init(&_rcond, NULL);
			pthread_cond_init(&_wcond, NULL);
			_array.reserve(maxSize);
		} 
		~BoundedBlockingQueue ( ) 
		{ 
			pthread_mutex_destroy(&_lock);
			pthread_cond_destroy(&_rcond);
			pthread_cond_destroy(&_wcond);
		} 
		void push(const T& data);
		T pop( ); 
		size_t size(){return _array.size();}
	private: 
		vector<T> _array; // or T* _array if you so prefer
		int maxSize;
		pthread_mutex_t _lock;
		pthread_cond_t _rcond, _wcond;
	};
	template <typename T>
	void BoundedBlockingQueue <T>::push(const T& value ) 
	{ 
		//if (_array.size( ) < maxSize)
		{
			pthread_mutex_lock(&_lock);
			const bool was_empty = _array.empty( );
			while (_array.size( ) == maxSize) 
			{ 
	 			pthread_cond_wait(&_wcond, &_lock);
	 		} 
			
			_array.push_back(value);
			pthread_mutex_unlock(&_lock);
			if (was_empty) 
				pthread_cond_broadcast(&_rcond);
		}
	}
	template <typename T>
	T BoundedBlockingQueue<T>::pop( ) 
	{ 
		pthread_mutex_lock(&_lock);
		const bool was_full = (_array.size( ) == maxSize);
		while(_array.empty( )) 
		{ 
			pthread_cond_wait(&_rcond, &_lock) ;
		}
		T _temp = _array.front( );
		_array.erase( _array.begin( ));
		pthread_mutex_unlock(&_lock);
		if (was_full)
			pthread_cond_broadcast(&_wcond);
		return _temp;
	}
}
#endif