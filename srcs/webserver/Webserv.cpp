#include "Webserv.hpp"

ft::Webserv::Webserv() : servers(), clients() {}
ft::Webserv::~Webserv(){}
ft::Webserv::Webserv(const Webserv& other) : servers(other.servers), clients(other.clients)
{
}

void	ft::Webserv::insertServer(int server_fd, server_type server)
{
	servers.insert(std::make_pair(server_fd, server));
}

/**
 * @brief Get number of servers
 * 
 * @return int 
 */
int		ft::Webserv::server_size()
{
	return servers.size();
}

/**
 * @brief Get number of clients
 * 
 * @return int 
 */
int		ft::Webserv::client_size()
{
	return clients.size();
}

ft::Webserv::servers_iterator	ft::Webserv::begin()
{
	return this->servers.begin();
}

ft::Webserv::servers_iterator 	ft::Webserv::end()
{
	return this->servers.end();
}

ft::Socket	ft::Webserv::serverSocket(servers_iterator it)
{
	return it->second.getSocket();
}

ft::Server	ft::Webserv::findServer(int	server_fd)
{
	return (this->servers.find(server_fd))->second;
}

int	ft::Webserv::findServerfd(int client_fd)
{
	return 	clients.find(client_fd)->second.server_fd;
}


void	ft::Webserv::insertClient(int server_id, int client_fd)
{
	ft::Client tmp(server_id, findServer(server_id).getInfo());
	
	clients.insert(std::make_pair(client_fd, tmp));
}

void	ft::Webserv::eraseClient(int client_fd)
{
	clients.erase(client_fd);
	close(client_fd);
}

ft::Client	ft::Webserv::findClient(int client_fd)
{
	return clients.find(client_fd)->second;
}
