#include "mutil_thread.h"
#include <boost/shared_ptr.hpp>
#include <tr1/memory>
using namespace std::tr1;
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
		int threadIndex = (int)((long)(args));
		printf("name:%s,number:%u,threadid :%u,pid: %u\n",__FUNCTION__,this->get_mem(),(unsigned int)pthread_self(),threadIndex);
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
	MutilThread mThread(8);
	mThread.start();
	unsigned int i=0;
	//for (i = 0 ; i<mWriteCount;i++)
	for(;;)
	{
		i++;
		//TestTask * task = new TestTask(i);
		shared_ptr<TestTask> spt(new TestTask(i));
		mThread.push(spt);
	}
	printf("1111111111");
	mThread.wait();
	return 0;
}