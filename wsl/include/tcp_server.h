#ifndef _WSL_SERVER_H_
#define _WSL_SERVER_H_
#include "event_loop.h"
#include "wsl.h"
#include "socket.h"

namespace wsl
{
	class TcpServer:
		public Object
	{
	public:
		TcpServer(EventLoop * _loop);
		~TcpServer();
		int listen_port(int port);
		int start();
		int stop();
		void accept_handler(int fd,int mask,void* data);
	private:
		EventLoop *m_loop;
		Socket m_socket;
		Socket sockets[WSL_BINDADDR_MAX];
	};
}

#endif