#ifndef SOCKET_HPP
#define SOCKET_HPP
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdexcept>
#include <iostream>
#include <fcntl.h>
#include <cstring>
#include <unistd.h>

namespace ft
{
	class Socket {
		public:
			~Socket();
			Socket();
			Socket(int port, int backlog = 20);
			Socket(int domain, int service, int protocol, int port, u_long interface, int backlog = 20);
			
			int	getConnection(void);
			int	getFd(void);
			struct sockaddr_in	getAddress(void);
			int accept_connection();

		private:
			int	socket_fd;
			int connection;
			struct sockaddr_in	address;

			void setup(int domain, int service, int protocol, int port, u_long interface, int backlog);
	};
}

#endif
