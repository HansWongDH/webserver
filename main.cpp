#include "./srcs/webserver/server/Server.hpp"
#include "./srcs/parser/Parser.hpp"
#include "./includes/colour.h"
#include <iostream>
#include <vector>
#include <poll.h>
#include <unistd.h>

using std::cout;
using std::endl;
using std::vector;

int main(int ac, char **av, char **env)
{	
	
	if (ac != 2)
	{
		throw std::invalid_argument("Too many arguments");
	}
	std::ifstream file;
	file.open(av[1]);
	if (!file.is_open())
		throw std::invalid_argument("Invalid config files");
	const char *temp[] = {"listen", "allow_methods", "root", "index", "404"};
	std::vector<std::string> test(temp, temp + 5);
	ft::Parser parsed(file, test, env);
	ft::Webserv	WebServer(parsed.getWebserv());
	// for (ft::Webserv::servers_iterator it = WebServer.begin(); it != WebServer.end(); it++)
	// 	 std::cout << i++ << std::endl;
	
	vector<struct pollfd> fds;
	char buf[BUFFER_SIZE];

	for (ft::Webserv::servers_iterator it = WebServer.begin(); it != WebServer.end(); it++)
	{
		struct pollfd tmp;
		// std::cout << "HERE === " << i++ << std::endl;
		it->second.getInfo().printConfig();
		tmp.fd = it->second.getFd();
		tmp.events = POLLIN;
		fds.push_back(tmp);
	}

	// for (vector<struct pollfd>::iterator it = fds.begin(); fds)

	int server_size = WebServer.server_size();
			bool connection = false;
	// cout << " fds size is " << fds.size()  << endl;
	while (1)
	{

		if (poll(&fds[0], fds.size(), 1000))
		{
			// cout << " fds revents is " <<  fds.data()->revents<< endl;
			for (int i = 0; i < fds.size(); i++)
			{
				if (fds[i].revents == 0)
					continue;
				if (i < server_size)
				{
					if (fds[i].revents & POLLIN)
					{
						struct pollfd tmp;
						tmp.fd = WebServer.findServer(fds[i].fd).getSocket().accept_connection();
						WebServer.insertClient(fds[i].fd, tmp.fd);
						tmp.events = POLLIN;
						cout << CYAN "[INFO] Incoming connection connected to FD: " << fds[i].fd << " with client FD: " << tmp.fd <<  RESET << endl;
						fds.push_back(tmp);
					}
				}
				else if (fds[i].revents != 0)
				{
					if (fds[i].revents & POLLIN)
					{
				
						int ret = read(fds[i].fd, buf, BUFFER_SIZE);
						if (ret)
						{
							WebServer.findClient(fds[i].fd).insertRequest(buf);
							cout << MAGENTA "[INFO] Client FD : " << fds[i].fd << " is in read mode." RESET << endl;
							cout << BLUE << buf << RESET << endl;
							// cout << WebServer.findClient(fds[i].fd).getRequest();
							fds[i].events = POLLOUT;
							// WebServer.findClient(fds[i].fd).getRequest().clear();
						}
						else
							connection = true;
					}
					else if (fds[i].revents & POLLOUT)
					{
						cout << MAGENTA "[INFO] Client FD : " << fds[i].fd << " is in send mode." RESET << endl;
					
						// std::cout << WebServer.findClient(fds[i].fd).getResponse()->returnResponse() << std::endl;
						// write(fds[i].fd, buf , BUFFER_SIZE);
						// std::cout << WebServer.findClient(fds[i].fd).getResponse() << std::endl;
						WebServer.findClient(fds[i].fd).getResponse()->returnResponse(fds[i].fd);
						if (WebServer.findClient(fds[i].fd).getResponse()->empty())
						{
							connection = true;
							fds[i].revents = POLLHUP;
						}
						// poll_length--;
					}
					if (fds[i].revents & POLLHUP || connection == true)
					{
						// std::cout << "HERE2" << std::endl;
						cout << RED "Deleteing client FD: " << fds[i].fd << " connnected to server FD: "
						<< WebServer.findServerfd(fds[i].fd) <<  RESET <<  endl;
						WebServer.eraseClient(fds[i].fd);
						fds.erase(fds.begin() + i);
						connection = false;
					}
				}
			}
		}
	}
}

