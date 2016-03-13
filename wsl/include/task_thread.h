#ifndef _WSL_TASK_THREAD_H_
#define _WSL_TASK_THREAD_H_

#include "simple_thread.h"
#include "blocking_queue.h"
namespace wsl
{
	
	class TaskIf;
	typedef blocking_queue<TaskIf* > TaskQueue;//can use non block task queue
	class TaskThread:
		public ThreadIf
	{
	public:
		TaskThread(TaskQueue&);
		~TaskThread();
		void run();
		virtual int thread_action()=0;
	public:
		TaskQueue &task_queue;
	};
}
#endif