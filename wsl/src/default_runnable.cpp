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
		//(ERROR, "�Ѿ��������˲��������߳���");
		return;
	}
	thread_num = num;
}
int start()
{
	return 0;
}

void stop(){

}

	
void wait(){

}