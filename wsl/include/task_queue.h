#ifndef _WSL_TASK_THREAD_H_
#define _WSL_TASK_THREAD_H_

#include "simple_thread.h"

namespace wsl
{
	class Task;
	class TaskThread:
		public Runnable
	{
	public:
		TaskThread();
		~TaskThread();
		void run();
	public:
		std::queue<Task> & mTaskQueue;
	};
}
#endif