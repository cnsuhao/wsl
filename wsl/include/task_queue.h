#ifndef _WSL_TASK_THREAD_H_
#define _WSL_TASK_THREAD_H_

#include "simple_thread.h"
#include "blocking_queue.h"

namespace wsl
{
	class TaskThread:
		public Runnable
	{
	public:
		TaskThread();
		~TaskThread();
		void run();
	public:
		//std::queue<Task> & mTaskQueue;
		blocking_queue<TaskIf> task_queue;
	};
}
#endif