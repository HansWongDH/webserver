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
				else
				{
					if (fds[i].revents & POLLIN)
					{
					
						char *buf = (char *)calloc(sizeof(char), BUFFER_SIZE + 1);
						int ret;
						ret = recv(fds[i].fd, buf, BUFFER_SIZE, 0);
						// buf[ret] = 0;
						cout << "return value of recv :" << ret << MAGENTA "[INFO] Client FD : " << fds[i].fd << " is in read mode." RESET << endl;
						if (ret > 0)
						{
						
							std::cout << "SIZE OF BUFFER = " << string(buf).length() << std::endl;
							// string tmp = buf;
							// cout << buf << endl;
							// tmp.append(buf);
							if (WebServer.findClient(fds[i].fd).getRequest()->getcontentLength() == -1)
							{
								// std::cout << RED "Here 1" RESET<< endl;m
								WebServer.findClient(fds[i].fd).insertHeader(buf,ret);
								if (WebServer.findClient(fds[i].fd).getRequest()->findCarriage() == true)
								{
									WebServer.findClient(fds[i].fd).parseHeader(ret);
									std::cout << RED "Here 2" RESET<< endl;
								}
							}
							else
							{
								std::cout << RED "Here 3" RESET<< endl;
								WebServer.findClient(fds[i].fd).insertBody(buf,ret);
							}
							std::cout << "Content length == " << WebServer.findClient(fds[i].fd).getRequest()->getcontentLength() << std::endl;
							if (WebServer.findClient(fds[i].fd).getRequest()->getcontentLength() == WebServer.findClient(fds[i].fd).getRequest()->getRawbytes())
							{
								WebServer.findClient(fds[i].fd).parseRespond();
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
				
						WebServer.findClient(fds[i].fd).getResponse()->returnResponse(fds[i].fd);
						if (WebServer.findClient(fds[i].fd).getResponse()->empty())
						{
							connection = true;
						}
						// poll_length--;
					}
					if (fds[i].revents & POLLHUP || connection == true)
					{
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

