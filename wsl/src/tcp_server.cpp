
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
	if (SOCKET_OK != m_socket.create(port))
	{
		return -1;
	}
	
	return 0;
}




