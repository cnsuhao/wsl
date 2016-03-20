#ifndef _WSL_DEFAULT_RUNNABLE_
#define _WSL_DEFAULT_RUNNABLE_
#include "simple_thread.h"
namespace wsl
{
	class DefaultRunnable:public Runnable
	{
	public:
		DefaultRunnable(int threadNum =1);
		virtual ~DefaultRunnable();
		void set_thread_num(int num);
		/** 
		* create %_threadCount threads
		* @return started thread count;
		*/
		int start();

		/**
		 * stop
		 */
		void stop();

		/**
		 * wait
		 */
		void wait();
	protected:
		int thread_num;
		MThread* aThread;
		bool m_stop;
	};
}

#endif