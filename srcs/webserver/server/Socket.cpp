#include "Socket.hpp"

ft::Socket::~Socket() {
	// close(socket_fd);
}

/**
 * @brief Construct a new Socket object
 * Default's to port 3000 and 20 backlog connections
 */
ft::Socket::Socket()
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

	setup(domain, service, protocol, port, interface, backlog);
}

/**
 * @brief Construct a new Socket object
 * 
 * @param port Port to listen on
 */
ft::Socket::Socket(int port)
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
	int interface = INADDR_ANY;

	setup(domain, service, protocol, port, interface, backlog);
}

/**
 * @brief Construct a new Socket object
 * 
 * @param port Port to listen on
 * @param backlog Optional. Defaults to 20
 */
ft::Socket::Socket(int port, int backlog = 20)
{
	/**
	 * @brief defining address structure
	 * The htons() function translates a short integer from host byte order to network byte order. 
	 * The htonl() function translates a long integer from host byte order to network byte order.
	 */
	int domain = AF_INET;
	int service = SOCK_STREAM;
	int protocol = 0;
	int interface = INADDR_ANY;

	setup(domain, service, protocol, port, interface, backlog);
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
ft::Socket::Socket(int domain, int service, int protocol, int port, u_long interface, int backlog = 20)
{
	setup(domain, service, protocol, port, interface, backlog);
}

/* Getter*/
int	ft::Socket::getConnection(void)
{
	return this->connection;
}

int	ft::Socket::getFd(void)
{
	return this->socket_fd;
}

struct sockaddr_in	ft::Socket::getAddress(void)
{
	return this->address;
}

/**
 * @brief Accept a connection
 * 
 * @return int The client's FD
 */
int ft::Socket::accept_connection()
{
	int client_fd;
	socklen_t addr_len = sizeof(address);

	client_fd = accept(socket_fd, (struct sockaddr *)&address, &addr_len);

	if (client_fd < 0)
		throw std::runtime_error("Failed to accept connection");

	int flags = fcntl(client_fd, F_GETFL, 0);
	if (flags < 0)
	{
		throw std::runtime_error("Failed to get client socket flags");
	}

	if (fcntl(client_fd, F_SETFL, flags | O_NONBLOCK) < 0)
	{
		throw std::runtime_error("Failed to set socket to non-blocking mode");
	}

	return client_fd;
}

void ft::Socket::setup(int domain, int service, int protocol, int port, u_long interface, int backlog)
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

	std::cout << "\033[96m[INFO]	Initializing server..." << std::endl;

	if ((socket_fd = socket(domain, service, protocol)) < 0)
	{
		std::cerr << "\033[31m[ERROR]	\033[96msocket:	\033[0m" << strerror(errno) << std::endl;
		throw std::runtime_error("Failed to create socket");
	}

	std::cout << "\033[96m[INFO]	FD: " << socket_fd << std::endl;

	int opt = 1;
	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		std::cerr << "\033[31m[ERROR]	\033[96msetsockopt:	\033[0m" << strerror(errno) << std::endl;
		throw std::runtime_error("Failed to set socket options");
	}

	if (bind(socket_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		std::cerr << "\033[31m[ERROR]	\033[96mbind:	\033[0m" << strerror(errno) << std::endl;
		std::cerr << "\033[31mport:	" << port << "\033[0m" << std::endl;
		throw std::runtime_error("Failed to bind socket");
	}

	if (listen(socket_fd, backlog) < 0)
	{
		std::cerr << "\033[31m[ERROR]	\033[96mlisten:	\033[0m" << strerror(errno) << std::endl;
		throw std::runtime_error("Failed to listen on socket");
	}

	std::cout << "\033[92m[ OK ]	Ready! Listening on port " << port << "\033[0m" << std::endl;
}