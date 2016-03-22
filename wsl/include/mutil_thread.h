#ifndef _WSL_MUTIL_THREAD_H_
#define _WSL_MUTIL_THREAD_H_
#include "default_runnable.h"
#include "blocking_queue.h"
#include "bound_blocking_queue.h"
#include <tr1/memory>
using namespace std::tr1;

namespace wsl
{
	class SimpleTaskIf
	{
	public:
		virtual ~SimpleTaskIf(){} ;
		virtual void execute(void* args) =0 ;
	};

	class MutilThread : public DefaultRunnable
	{
	public:
		MutilThread();
		MutilThread(int thread_num);
		~MutilThread();
	public:
		void push(SimpleTaskIf* ptask);
		void push(shared_ptr<SimpleTaskIf> spTask);
		void run(MThread* pthread,void* args);
	private:
		//blocking_queue<SimpleTaskIf* > taskQueue;
		BoundedBlockingQueue<SimpleTaskIf*> taskQueue;
		BoundedBlockingQueue<shared_ptr<SimpleTaskIf> > staskQueue;

	};
}
#endif