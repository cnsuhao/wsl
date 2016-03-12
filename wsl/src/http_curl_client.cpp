#include "http_curl_client.h"
using namespace wsl;


HttpClient::HttpClient()
{

}

HttpClient::~HttpClient()
{

}
int HttpClient::write_data_callback(void* buffer,size_t size,size_t nmemb,void *stream)
{
	std::string* str = dynamic_cast<std::string*>((std::string *)stream);  
	if( NULL == str || NULL == buffer )  
	{  
		return -1;  
	}
	size_t buff_len  = size* nmemb;
	str->append((char*)buffer,buff_len);
	return buff_len;
}

int HttpClient::http_post(string url,string param,string resp)
{
	CURL *curl =NULL;
	CURLcode res;
	curl = curl_easy_init();
	if (!curl)
	{
		fprintf(stderr,"curl init failed\n");
		return -1;
	}
	curl_easy_setopt(curl,CURLOPT_URL,url.c_str()); //url地址
	curl_easy_setopt(curl,CURLOPT_POSTFIELDS,param.c_str()); //post参数
	curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,&(HttpClient::write_data_callback)); //对返回的数据进行操作的函数地址
	curl_easy_setopt(curl,CURLOPT_WRITEDATA,(void*)&resp); //这是write_data_callback的第四个参数值
	curl_easy_setopt(curl,CURLOPT_POST,1); //设置问非0表示本次操作为post

	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);  
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);  
	res = curl_easy_perform(curl);  
	curl_easy_cleanup(curl);  
	return res;
}

int HttpClient::http_get(const std::string & strUrl, std::string & strResponse)
{
	return 0;
}