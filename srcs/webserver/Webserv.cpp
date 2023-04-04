#include "Webserv.hpp"

ft::Webserv::Webserv() : servers(), clients() {}
ft::Webserv::~Webserv(){}
ft::Webserv::Webserv(char **env) : servers(), clients() {
	// std::cout << RED "HERE" RESET << std::endl;
	this->envs = charToMap(env);
}
ft::Webserv::Webserv( const Webserv& other) : servers(other.servers), clients(other.clients), envs(other.envs)
{
	// for (servers_const_iterator it = other.servers.begin(); it != other.servers.end(); it++)
	// 	it->second.getInfo().printConfig();



}

void	ft::Webserv::insertServer(int server_fd, server_type server)
{
	// ft::Server *serv = new ft::Server(server);
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
	ft::Client tmp(server_id, findServer(server_id).getInfo(), envs);
	
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

ft::Webserv::env_map	ft::Webserv::charToMap(char **env)
{
	ft::Webserv::env_map envs;

	for (int i = 0; env[i] != NULL; i++)
	{
		string tmp(env[i]);
		// std::cout << tmp.substr(0, tmp.find_first_of('=')) << " === " << tmp.substr(tmp.find_first_of('=') + 1) << std::endl;
		envs.insert(std::make_pair(tmp.substr(0, tmp.find_first_of('=')), tmp.substr(tmp.find_first_of('=') + 1)));
	}
	return envs;
}




void	ft::Webserv::printEnv(void)
{
	// std::cout << envs.empty() << std::endl;
	for (map<string, string>::iterator it = envs.begin(); it != envs.end(); it++)
		std::cout << it->first << " = " << it->second << std::endl;
}