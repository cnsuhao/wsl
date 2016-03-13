#include "../include/http_curl_client.h"
#include <pthread.h>
#include <string>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <iostream>
using namespace std;
using namespace wsl;

#define POSTURL    "http://www.xiami.com/member/login"  
#define POSTFIELDS "email=myemail@163.com&password=mypassword&autologin=1&submit=µÇ Â¼&type="  
void * test_http(void* argv)
{
	HttpClient *hc = (HttpClient*)argv;
	while(1)
	{
		string resp;
		int ret = hc->http_post(POSTURL,POSTFIELDS,resp);
		cout<<"ret :"<<ret <<"resp: "<<resp<<endl;
		sleep(1);
	}
	return NULL;
}
int main()
{
	pthread_t pthId;
	HttpClient hp;
	pthread_create(&pthId,NULL,test_http,(void*)&hp);
	pthread_join(pthId,NULL);
}