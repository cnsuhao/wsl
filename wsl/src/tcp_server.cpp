
#include "tcp_server.h"
using namespace  wsl;

TcpServer::TcpServer(EventLoop * _loop):
	m_loop(_loop)
{
	
}
TcpServer::~TcpServer()
{
}

int TcpServer::listen_port(int port)
{
	if (SOCKET_OK != m_socket.create_socket(port,NULL,AF_INET,400))
	{
		return -1;
	}
	return 0;
}
int TcpServer::start()
{
	if (!m_socket.is_valid())
	{
		return -1;
	}
	m_loop->createFileEvent(m_socket.get_fd(),EV_IO_READ,EV_IO_CB(this,TcpServer::accept_handler),NULL);
	return 0;
}

void TcpServer::accept_handler(int fd,int mask,void* data)
{
	printf("accept_handler fd %d\n",fd);
	return;
}




