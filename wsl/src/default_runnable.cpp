#include "default_runnable.h"

using namespace  wsl;



DefaultRunnable::DefaultRunnable(int _num)
:thread_num(_num),aThread(NULL),m_stop(false)
{

}
DefaultRunnable::~DefaultRunnable()
{
	if (aThread)
	{
		delete[] aThread;
		aThread=NULL;
	}
}
void DefaultRunnable::set_thread_num(int num)
{
	if (aThread != NULL) {
		//(ERROR, "已经在运行了不能设置线程数");
		return;
	}
	thread_num = num;
}
int DefaultRunnable::start()
{
	if (aThread != NULL || thread_num < 1) {
		return 0;
	}
	aThread = new MThread[thread_num];
	if (NULL == aThread)
	{
		return 0;
	}
	int i = 0;
	for (; i<thread_num; i++)
	{
		if (!aThread[i].start(this, (void*)((long)i)))
		{
			return i;
		}
	}
	return i;
}

void DefaultRunnable::stop(){
	m_stop = false;
}

	
void DefaultRunnable::wait(){
	if (aThread != NULL)
	{
		for (int i=0; i<thread_num; i++)
		{
			aThread[i].join();
		}
	}
}