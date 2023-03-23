#ifndef SOCKET_HPP
#define SOCKET_HPP
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdexcept>
#include <iostream>

namespace ft
{
	class Socket {
		public:
			~Socket() {}

			/**
			 * @brief Construct a new Socket object
			 * Default's to port 3000 and 20 backlog connections
			 */
			Socket()
			{
				/**
				 * @brief defining address structure
				 * The htons() function translates a short integer from host byte order to network byte order. 
				 * The htonl() function translates a long integer from host byte order to network byte order.
				 */
				int domain = AF_INET;
				int service = SOCK_STREAM;
				int protocol = 0;
				int backlog = 20;
				int port = 3000;
				int interface = INADDR_ANY;

				address.sin_family = domain;
				address.sin_port = htons(port);
				address.sin_addr.s_addr = htonl(interface);
				memset(address.sin_zero, '\0', sizeof address.sin_zero);

				if ((socket_fd = socket(domain, service, protocol)) < 0)
					throw std::runtime_error("Failed to create socket");

				if (bind(socket_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
					throw std::runtime_error("Failed to bind socket");

				if (listen(socket_fd, backlog) < 0)
					throw std::runtime_error("Failed to listen on socket");
			}
			
			/**
			 * @brief Construct a new Socket object
			 * 
			 * @param domain 
			 * AF_INET (IP) | AF_INET6 (IPv6) | AF_UNIX (local channel, similar to pipes) | AF_ISO (ISO protocols) | AF_NS (Xerox Network Systems protocols)
			 * @param service 
			 * SOCK_STREAM (virtual circuit service)| SOCK_DGRAM (datagram service)| SOCK_RAW (direct IP service)
			 * @param protocol 
			 *  protocol is a standard set of rules for transferring data, such as UDP/IP and TCP/IP. An application program can specify a protocol only if more than one protocol is supported for this particular socket type in this domain. Each socket can have a specific protocol associated with it.
			 * PF_UNIX(Local communication) | PF_INET (Internet (TCP/IP)) | PF_NDD (The operating system NDD)
			 */
			Socket(int domain, int service, int protocol, int port, u_long interface, int backlog = 20)
			{
				/**
				 * @brief defining address structure
				 * The htons() function translates a short integer from host byte order to network byte order. 
				 * The htonl() function translates a long integer from host byte order to network byte order.
				 */
				address.sin_family = domain;
				address.sin_port = htons(port);
				address.sin_addr.s_addr = htonl(interface);
				memset(address.sin_zero, '\0', sizeof address.sin_zero);

				if ((socket_fd = socket(domain, service, protocol)) < 0)
					throw std::runtime_error("Failed to create socket");

				if (bind(socket_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
					throw std::runtime_error("Failed to bind socket");

				if (listen(socket_fd, backlog) < 0)
					throw std::runtime_error("Failed to listen on socket");
			}

			/* Getter*/
			int	getConnection(void)
			{
				return this->connection;
			}

			int	getSocketfd(void)
			{
				return this->socket_fd;
			}

			struct sockaddr_in	getAddress(void)
			{
				return this->address;
			}

			/**
			 * @brief Accept a connection
			 * 
			 * @return int The client's FD
			 */
			int accept_connection()
			{
				int client_fd;
				socklen_t addr_len = sizeof(address);

				client_fd = accept(socket_fd, (struct sockaddr *)&address, &addr_len);
				if (client_fd < 0)
					throw std::runtime_error("Failed to accept connection");

				return client_fd;
			}

		private:
			int	socket_fd;
			int connection;
			struct sockaddr_in	address;
	};
} // namespace ft


#endif