

#include <iomanip>
#include <sys/time.h>
#include <iostream>
#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <vector>

//#include "../include/blocking_queue.h"
#include "blocking_queue.h"
#include "simple_thread.h"

using namespace std;
using namespace wsl;

class Test
{
public:
	Test(){};
	~Test(){};
	void run();
};

void Test::run()
{
	blocking_queue<int> q;
	q.push(10);
	q.push(20);

	cout<<"size:  "<<q.size()<<endl;
	cout<<"pop "<< q.pop()<<endl;
	cout<<"size:  "<<q.size()<<endl;
	cout<<"pop "<< q.pop()<<endl;

}
struct  testTask
{
	testTask(int i ):data(i){}
	int data;
};
blocking_queue<testTask*> gqueue;
class PushThread:public Runnable
{
public:
	PushThread(){}
	~PushThread(){}
	void run(MThread* pthread,void *args)
	{
		int i=0;
		while (1)
		{
			testTask* tt = new testTask(i++);
			gqueue.push(tt);
			cout<<"PushThread size:  "<<gqueue.size()<<endl;
			cout<<"PushThread data:  "<<tt->data<<endl;
		}
	}
};

class PopThread:public Runnable
{
public:
	PopThread(){}
	~PopThread(){}
	void run(MThread* pthread,void *args)
	{
		while (1)
		{
			testTask* tt = gqueue.pop();
			cout<<"PopThread size:    "<<gqueue.size()<<endl;
			cout<<"PopThread data:   "<< tt->data<<endl;
			delete tt;
			tt=NULL;
		}
	}
};
void * push_thread_func(void* )
{
	int i=0;
	for (;;)
	{
		testTask* tt = new testTask(i++);
		gqueue.push(tt);
	}
	
}
void * pop_thread_func(void* )
{
	int i=0;
	for (;;)
	{
		testTask* tt = gqueue.pop();
		//printf("pthread id [%u],num [%d]\n",pthread_self(),tt->data);	
		delete tt;
		tt=NULL;
	}
}
int main()
{
//	Test tst;
	//tst.run();
	/*PushThread pushT;
	PopThread popT;
	MThread *pushThread = new MThread( );
	MThread *popThread = new MThread( );
	pushThread->start(&pushT,NULL);
	popThread->start(&popT,NULL);
	pthread_join(pushThread->get_thread(),NULL);
	pthread_join(popThread->get_thread(),NULL);
	*/
#define  THREADNUM 1
	int i;
	pthread_t push_pthread[THREADNUM]={0};
	for ( i =0 ; i<THREADNUM;i++)
	{
		if(pthread_create(&push_pthread[i], NULL, push_thread_func, NULL) != 0)
		{
			perror("push_thread_func pthread_create error");
			return false;
		}
	}
	pthread_t pop_pthread[THREADNUM]={0};
	for ( i =0 ; i<THREADNUM;i++)
	{
		if(pthread_create(&pop_pthread[i], NULL, pop_thread_func, NULL) != 0)
		{
			perror("push_thread_func pthread_create error");
			return false;
		}
	}
	for (i=0;i<THREADNUM;i++)
	{
		pthread_join(pop_pthread[i],NULL);
	}
	for (i=0;i<THREADNUM;i++)
	{
		pthread_join(push_pthread[i],NULL);
	}
	return 0;
}