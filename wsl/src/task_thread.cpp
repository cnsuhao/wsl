#include "task_thread.h"
#include "task_interface.h"
using namespace wsl;

TaskThread::TaskThread(TaskQueue& btq)
	:task_queue(btq)
{
}
TaskThread::~TaskThread()
{
}
void TaskThread::run()
{
	while(!this->stop())
	{
		if (this->thread_action() == -1)
		{
			this->set_stop();
		}
	}
	return;
}