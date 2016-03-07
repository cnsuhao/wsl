#ifndef _WSL_SOCKET_H_
#define _WSL_SOCKET_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
namespace wsl
{
#ifndef INVALID_SOCKET
#define INVALID_SOCKET -1
#endif

#define SOCKET_ERR		-1
#define SOCKET_OK		0

	class Socket
	{
	public:
		Socket():m_fd(INVALID_SOCKET), m_error(0){}
		Socket(int fd):m_fd(fd){}
		~Socket(){}
	public:
		int get_fd(){ return m_fd; }
		bool is_valid(){ return m_fd > 0;}
		int get_error(){ return m_error; }
		int create(int domain);
		int close();
		int listen(struct sockaddr *sa, int len);
		int listen( struct sockaddr *sa, socklen_t len, int backlog);
		int accept(sockaddr *addr, int *len);
		int connect_tcp(const char *host, short port, int isNonBlock = 0);
		int connect_unix(const char *path, int isNonBlock = 0);
		int read(char *buffer, int len);
		int write(const char *buffer, int len);
		int set_reuseaddr();
		int set_nonblock();
		int set_nondelay();
		int set_keepalive();
		int create_socket(int port, char *bindaddr, int af, int backlog);
		int v6_only();
	private:
		int m_fd;
		int m_error;
	};
}

#endif