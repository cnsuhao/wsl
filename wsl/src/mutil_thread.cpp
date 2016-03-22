#include "mutil_thread.h"

using namespace wsl;

MutilThread::MutilThread()
{

}
MutilThread::MutilThread(int threadnum)
	:DefaultRunnable(threadnum)
{
}
MutilThread::~MutilThread(){}

void MutilThread::run(MThread* pthread,void* args)
{
	int threadIndex = (int)((long)(args));
	printf("MutilThread::run [%d]\n",threadIndex);
	/*while(!m_stop)
	{
		SimpleTaskIf* task=NULL;
		task = taskQueue.pop();
		printf("taskQueue size [%u]\n",taskQueue.size());
		if(task)
		{
			task->execute(args);
			delete task;
			task = NULL;
		}
	}
	*/
	while(!m_stop)
	{
		printf("taskQueue size [%u]\n",staskQueue.size());
		shared_ptr<SimpleTaskIf> spTask;
		spTask = staskQueue.pop();
		spTask->execute(args);
	}
	return ;
}
void MutilThread::push(SimpleTaskIf* ptask)
{
	if (!ptask)
	{
		return ;
	}
	taskQueue.push(ptask);
}

void MutilThread::push(shared_ptr<SimpleTaskIf> spTask)
{
	staskQueue.push(spTask);
}