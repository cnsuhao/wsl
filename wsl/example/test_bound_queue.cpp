

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
#include "bound_blocking_queue.h"
#include "simple_thread.h"

using namespace std;
using namespace wsl;

//
//
//struct  testTask
//{
//	testTask(int i ):data(i){}
//	int data;
//};
//BoundedBlockingQueue<testTask*> gqueue(1000);
//class PushThread:public Runnable
//{
//public:
//	PushThread(){}
//	~PushThread(){}
//	void run()
//	{
//		int i=0;
//		while (1)
//		{
//			testTask* tt = new testTask(i++);
//			gqueue.push(tt);
//			//cout<<"PushThread size:  "<<gqueue.size()<<endl;
//			cout<<"PushThread data:  "<<tt->data<<endl;
//		}
//	}
//};
//
//class PopThread:public Runnable
//{
//public:
//	PopThread(){}
//	~PopThread(){}
//	void run()
//	{
//		while (1)
//		{
//			testTask* tt = gqueue.pop();
//			//cout<<"PopThread size:    "<<gqueue.size()<<endl;
//			cout<<"PopThread data:   "<< tt->data<<endl;
//			delete tt;
//			tt=NULL;
//		}
//	}
//};
//
int main()
{
//	PushThread pushT;
//	PopThread popT;
//	MThread *pushThread = new MThread(&pushT );
//	MThread *popThread = new MThread(&popT );
//	pushThread->start();
//	popThread->start();
//	pthread_join(pushThread->get_thread(),NULL);
//	pthread_join(popThread->get_thread(),NULL);
	return 0 ;
}