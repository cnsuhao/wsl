#include "mutil_thread.h"
using namespace wsl;
class TestTask:public SimpleTaskIf
{
public:
	~TestTask(){}
	TestTask(int i )
	{
		iMember = i ;
	}
	unsigned int get_mem(){return iMember;}
protected:
	void execute(void* args)
	{
	//	unsigned int pthreadId = *(unsigned int * )(args);
		printf("name:%s,number:%u,threadid :%u\n",__FUNCTION__,this->get_mem(),(unsigned int)pthread_self());
	}

private:
	unsigned int iMember;
};

int main(int argc, char *argv[])
{
	int mWriteCount=100000;
	if (argc>1) {
		mWriteCount = atoi(argv[1]);
	}
	MutilThread mThread(4);
	mThread.start();
	unsigned int i=0;
	//for (i = 0 ; i<mWriteCount;i++)
	for(;;)
	{
		i++;
		TestTask * task = new TestTask(i);
		mThread.push(task);
	}
	printf("1111111111");
	mThread.wait();
	return 0;
}