#ifndef _WSL_SIMPLE_BLOCKING_QUEUE_H_
#define _WSL_SIMPLE_BLOCKING_QUEUE_H_

#include <queue>
#include <vector>
#include <sys/time.h>

#include <pthread.h>
#include <semaphore.h>
#include <errno.h>

using namespace std;
namespace wsl
{
#define  QUEUE_SIZE 2000
	template <typename T>
	class SimpleBlockingQueue 
	{ 
	public: 
		SimpleBlockingQueue (int size) : maxSize(size) 
		{ 
			pthread_mutex_init(&_lock, NULL); 
			pthread_cond_init(&_rcond, NULL);
			_array.reserve(maxSize);
		} 
		SimpleBlockingQueue () : maxSize(QUEUE_SIZE) 
		{ 
			pthread_mutex_init(&_lock, NULL); 
			pthread_cond_init(&_rcond, NULL);
			_array.reserve(maxSize);
		} 
		~SimpleBlockingQueue ( ) 
		{ 
			pthread_mutex_destroy(&_lock);
			pthread_cond_destroy(&_rcond);
		} 
		void push(const T& data);
		T pop( ); 
		size_t size(){return _array.size();}
	private: 
		vector<T> _array; // or T* _array if you so prefer
		int maxSize;
		pthread_mutex_t _lock;
		pthread_cond_t _rcond;
	};
	template <typename T>
	void SimpleBlockingQueue <T>::push(const T& value ) 
	{ 
		pthread_mutex_lock(&_lock);
		_array.push_back(value);
		pthread_cond_signal(&_rcond);
		pthread_mutex_unlock(&_lock);
	}
	template <typename T>
	T SimpleBlockingQueue<T>::pop( ) 
	{ 
		pthread_mutex_lock(&_lock);
		while(_array.empty( )) 
		{ 
			pthread_cond_wait(&_rcond, &_lock) ;
			continue;
		}
		T _temp = _array.front( );
		_array.erase( _array.begin( ));
		pthread_mutex_unlock(&_lock);
		return _temp;
	}
}
#endif