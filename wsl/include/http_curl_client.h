#ifndef _WSL_HTTP_CLIENT_H_
#define _WSL_HTTP_CLIENT_H_
#include <curl/curl.h>
#include <string>
//#include "../dep/json/json.h"
using namespace std;

namespace wsl
{
        class HttpClient
        {
		public:
			static HttpClient& get_instance()
			{
				static HttpClient hclient;
				return hclient;
			}
			static int write_data_callback(void* buffer,size_t size,size_t nmemb,void *stream);
		public:
			HttpClient();
			~HttpClient();
			
			/**
			*@para url �������,�����Url��ַ,��:http://www.baidu.com
			*@para para �������,ʹ�����¸�ʽpara&para1&para2
			*@resp �������,���ص�����
			*@return 0: CURLE_OK, -1 fail other see https://curl.haxx.se/libcurl/c/libcurl-errors.html
			*/
			int http_post(const string url,string param,string& resp);
			/** 
			* @brief HTTP GET���� 
			* @param strUrl �������,�����Url��ַ,��:http://www.baidu.com 
			 * @param strResponse �������,���ص����� 
			* @return �����Ƿ�Post�ɹ� 
			*/
			int http_get(const std::string & strUrl, std::string & strResponse);
        };
}
#endif
