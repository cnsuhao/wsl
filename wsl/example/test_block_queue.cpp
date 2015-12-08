

#include <iomanip>
#include <sys/time.h>
#include <iostream>
#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <vector>

#include "../include/blocking_queue.h"


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
int main()
{
	Test tst;
	tst.run();
	return 0;
}