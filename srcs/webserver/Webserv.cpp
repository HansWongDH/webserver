#include "Webserv.hpp"

ft::Webserv::Webserv() : servers(), clients() {}
ft::Webserv::~Webserv(){
	for (servers_iterator it = servers.begin(); it != servers.end(); it++)
		delete it->second;
}
ft::Webserv::Webserv(char **env) : servers(), clients() {
	this->envs = charToMap(env);
}
ft::Webserv::Webserv( const Webserv& other) : servers(other.servers), clients(other.clients), envs(other.envs)
{
}

void	ft::Webserv::initializePollstruct(void)
{
	for (servers_iterator it = servers.begin(); it != servers.end(); it++)
	{
		struct pollfd tmp;
		tmp.fd = it->second->getFd();
		tmp.events = POLLIN;
		fds.push_back(tmp);
	}
}
void	ft::Webserv::insertServer(int server_fd, server_type *server)
{
	servers.insert(std::make_pair(server_fd, server));
}

/**
 * @brief Get number of servers
 * 
 * @return int 
 */

/**
 * @brief Get number of clients
 * 
 * @return int 
 */


void	ft::Webserv::insertClient(int server_id, int client_fd)
{
	ft::Client *tmp = new ft::Client(server_id, servers[server_id]->getInfoAddress(), envs);
	
	clients.insert(std::make_pair(client_fd, tmp));
}

void	ft::Webserv::eraseClient(int client_fd)
{
	delete clients[client_fd];
	clients.erase(client_fd);
	close(client_fd);
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

void	ft::Webserv::runServer(int timeout)
{
	bool	connection = false;
	while (1)
	{

		if (poll(&fds[0], fds.size(), timeout))
		{
			for (size_t i = 0; i < fds.size(); i++)
			{
				if (fds[i].revents == 0)
					continue;
				if (i < servers.size())
				{
					if (fds[i].revents & POLLIN)
					{
						struct pollfd tmp;
						tmp.fd = servers[fds[i].fd]->getSocket().accept_connection();
						insertClient(fds[i].fd, tmp.fd);
						tmp.events = POLLIN;
						cout << CYAN "[INFO] Incoming connection connected to FD: " << fds[i].fd << " with client FD: " << tmp.fd <<  RESET << endl;
						fds.push_back(tmp);
					}
				}
				else
				{
					if (fds[i].revents & POLLIN)
					{
					
						char *buf = (char *)calloc(sizeof(char), BUFFER_SIZE + 1);
						int ret;
						ret = recv(fds[i].fd, buf, BUFFER_SIZE, 0);
						std::cout << BLUE << buf << RESET << std::endl;
						cout << MAGENTA "[INFO] Client FD : " << fds[i].fd << " is in read mode." RESET << endl;
						if (ret > 0)
						{
							if (clients[fds[i].fd]->getRequest()->getcontentLength() == -1)
								clients[fds[i].fd]->insertHeader(buf,ret);
							else
								clients[fds[i].fd]->insertBody(buf,ret);
							// std::cout << "Content length == " << clients[fds[i].fd]->getRequest()->getcontentLength() << "raw_bytes ===" << clients[fds[i].fd]->getRequest()->getRawbytes() << std::endl;
							if (clients[fds[i].fd]->getRequest()->getcontentLength() == clients[fds[i].fd]->getRequest()->getRawbytes())
							{
								clients[fds[i].fd]->parseRespond();
								fds[i].events = POLLOUT;
							}
							
						}
						else
							 connection = true;
						free(buf);
					}
					else if (fds[i].revents & POLLOUT)
					{
						cout << MAGENTA "[INFO] Client FD : " << fds[i].fd << " is in send mode." RESET << endl;
				
						if (clients[fds[i].fd]->getResponse()->returnResponse(fds[i].fd) <= 0)
							connection = true;
					}
					if (fds[i].revents & POLLHUP || connection == true)
					{
						cout << RED "[INFO] Deleteing client FD: " << fds[i].fd << " connnected to server FD: "
						<< clients[fds[i].fd]->server_fd <<  RESET <<  endl;
						eraseClient(fds[i].fd);
						fds.erase(fds.begin() + i);
						connection = false;
					}
				}
			}
		}
	}
}