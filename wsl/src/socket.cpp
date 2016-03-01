#include "socket.h"

using namespace wsl;




int Socket::listen(struct sockaddr *sa, int len)
{
	if(::bind(m_fd, sa, len) == -1) {
		return SOCKET_ERR;
	}

	if(::listen(m_fd, 511) == -1) {
		return SOCKET_ERR;
	}

	return SOCKET_OK;
}

int Socket::accept(sockaddr *addr, int *len)
{
	int conn;
	for(;;){
		conn = ::accept(m_fd, addr, (socklen_t*)len);
		if (conn == -1) {
			if (errno == EINTR){
				continue;
			}else {
				return SOCKET_ERR;
			}
		}
		break;
	}

	return conn;
}

int Socket::connect_tcp(const char *host, short port, int isNonBlock)
{
	if(create(AF_INET) == SOCKET_ERR){
		return SOCKET_ERR;
	}

	if(isNonBlock && set_nonblock() == SOCKET_ERR){
		close();
		return SOCKET_ERR;
	}

	struct sockaddr_in s_add;
	bzero(&s_add, sizeof(struct sockaddr_in));
	s_add.sin_family = AF_INET;
	s_add.sin_addr.s_addr = inet_addr(host);
	s_add.sin_port = htons(port);
	if(connect(m_fd, (struct sockaddr *)(&s_add), sizeof(struct sockaddr)) == -1){
		if(errno != EINPROGRESS){
			close();
			m_error = errno;
			return SOCKET_ERR;
		}
	}

	return SOCKET_OK;
}

int Socket::connect_unix(const char *path, int isNonBlock)
{
	struct sockaddr_un sa;

	if(create(AF_LOCAL) == SOCKET_ERR){
		return SOCKET_ERR;
	}

	if(isNonBlock && set_nonblock() == SOCKET_ERR){
		close();
		return SOCKET_ERR;
	}

	sa.sun_family = AF_LOCAL;
	strncpy(sa.sun_path, path, sizeof(sa.sun_path)-1);
	if(connect(m_fd, (struct sockaddr*)&sa, sizeof(sa)) == -1) {
		if(errno != EINPROGRESS){
			close();
			m_error = errno;
			return SOCKET_ERR;
		}
	}

	return SOCKET_OK;
}
int Socket::set_reuseaddr()
{
	int yes = 1;
	/* Make sure connection-intensive things like the redis benckmark
	* will be able to close/open sockets a zillion of times */
	if (setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
		return SOCKET_ERR;
	}
	return SOCKET_OK;
}

int Socket::set_nonblock()
{
	int flags;

	/* Set the socket non-blocking.
	 * Note that fcntl(2) for F_GETFL and F_SETFL can't be
	 * interrupted by a signal. */
	if ((flags = fcntl(m_fd, F_GETFL)) == -1) {
		return SOCKET_ERR;
	}
	if (fcntl(m_fd, F_SETFL, flags | O_NONBLOCK) == -1) {
		return SOCKET_ERR;
	}

	return SOCKET_OK;
}


int Socket::set_nondelay()
{
	int val = 1;

	if (setsockopt(m_fd, IPPROTO_TCP, TCP_NODELAY, &val, sizeof(val)) == -1)
	{
		return SOCKET_ERR;
	}

	return SOCKET_OK;
}

int Socket::set_keepalive()
{
	int val = 1;

	if (setsockopt(m_fd, SOL_SOCKET, SO_KEEPALIVE, &val, sizeof(val)) == -1)
	{
		return SOCKET_ERR;
	}

	return SOCKET_OK;
}


int Socket::read(char *buffer, int len)
{
	int nread = ::read(m_fd, buffer, len);
	if(nread == -1){
		if(errno == EAGAIN){
			return 0;
		}
		m_error = errno;
		return SOCKET_ERR;
	}else if(nread == 0){
		m_error = 0;
		return SOCKET_ERR;
	}

	return nread;
}

int Socket::write(const char *buffer, int len)
{
	int nwrote = ::write(m_fd, buffer, len);
	if(nwrote == -1){
		if(errno == EAGAIN){
			return 0;
		}

		m_error = errno;
		return SOCKET_ERR;
	}else if(nwrote == 0){
		m_error = 0;
		return SOCKET_ERR;
	}

	return nwrote;
}


int Socket::create(int domain)
{
	m_fd = socket(domain, SOCK_STREAM, 0);
	if(m_fd == -1){
		return SOCKET_ERR;
	}

	if(set_reuseaddr() == SOCKET_ERR){
		close();
		return SOCKET_ERR;
	}

	return SOCKET_OK;
}

int Socket::close()
{
	::close(m_fd);
	return SOCKET_OK;
}