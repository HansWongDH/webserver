#include "Parser/Parser.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <poll.h>
#include "./Server/Server.hpp"
#include "./Server/Socket.hpp"


// void run(ft::Server	server)
// {
// 	const char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 18\n\nChicken pie world!";

// 	printf("\n+++++++ Waiting for new connection ++++++++\n\n");
	

// 	printf("Accepted\n");
// 	char buffer[30000] = {0};
// 	read(server.getFd() , buffer, 30000);
// 	printf("%s\n",buffer );
// 	write(server.getFd() , hello , strlen(hello));
// 	printf("------------------Hello message sent-------------------\n");
// }

int	main(void)
{
	std::ifstream file;
	std::string holder;
	file.open("./config/example.conf");
	const char* temp[]	= {"listen", "server_name", "root"};
	std::vector<std::string> test (temp, temp + 3);
	ft::Parser lol(file, test);
	std::vector<struct pollfd> fds;

	std::vector<ft::ServerBlock> vet;
	std::vector<ft::Server> Servers;
	vet = lol.getServerInfo();

	
	for (std::vector<ft::ServerBlock>::iterator it = vet.begin(); it != vet.end(); it++)
	{
		ft::Server server = ft::Server(it->getPortNo(), *it);
		Servers.push_back(server);
		
		// it->printConfig();
		// std::cout << "---------------------------" << std::endl;
		// it->printLocation();
		// std::cout << "===========================" << std::endl;
			// std::cout << "current i is " << i << std::endl;	
	}



	for (std::vector<ft::Server>::iterator it = Servers.begin(); it != Servers.end(); it++)
	{
		struct pollfd tmp;
		tmp.fd = it->getSocket().getSocketfd();
		tmp.events = POLLIN;
		fds.push_back(tmp);

		
	}

	// for (std::vector<ft::Server>::iterator it = Servers.begin(); it != Servers.end(); it++)
	// {
		
	//  	std::cout << "my fd is === "<< it->getSocket().getSocketfd() << std::endl;
	

		
	// }

	for (std::vector<struct pollfd>::iterator it = fds.begin(); it != fds.end(); it++)
	{
		std::cout << "event is === " <<it->revents << std::endl;
	}
	// std::vector<ft::Server> servers;

	// // typedef std::vector<ft::Server> serverblob;
	// for (std::vector<ft::ServerBlock>::iterator it = vet.begin(); it != vet.end(); it++)
	// {
	// 	// Initialize server
	// 	ft::Server server = ft::Server(it->getPortNo());
	// 	std::cout <<it->getPortNo() << std::endl;
	// 	servers.push_back(server);
	// }
	// for (int i = 0; i < servers.size(); i++)
	// {
		
	// 	fds[i].fd = servers[i].getSocket().getSocketfd();
	// 	fds[i].events = POLLIN;
	// }
	char buf[3000];

	// fds[0].fd = lol.getSocketfd();
	// fds[0].events = POLLIN;
	// int poll_length = 1;
	// bool haha = false;
	while (1)
	{
		int	server_size = Servers.size();
		if (poll(fds.data(), fds.size(), 1000))
		{
		
			for(int i = 0; i < fds.size(); i++)
			{
					// std::cout << "event is === " <<fds[i].revents << std::endl;
			if (i < server_size)
			{
				if(fds[i].revents == 0)
        			continue;
				if (fds[i].revents & POLLIN)
				{
					
					struct pollfd tmp;
					tmp.fd = Servers[i].getSocket().accept_connection();
					tmp.events = POLLIN;
					fds.push_back(tmp);
				}
			}
			else if (fds[i].revents != 0)
				{
					// if (fds[i].fd)
					// {
					// if (haha == true)
					// 	{
					// 		std::cout << "hajhahaahahahahah" << std::endl;
					// 		fds[i].fd = -1;
					// 		poll_length--;
					// 		haha = false;
					// 		break;
					// 	}
						if (fds[i].revents & POLLIN)
						{
							std::cout<< "connected" << std::endl;
							read(fds[i].fd, buf, 3000);
							if (buf)
								fds[i].events = POLLOUT;
						}
						else if (fds[i].revents & POLLOUT)
						{
							std::cout << "SENDING" << std::endl;
							write(fds[i].fd, "HAHAHAHAHAHAHAAHAH", 19);
							fds[i].events = POLLIN;
							// poll_length--;
						}
				
					}
					}
				// }
			
			}

		}
	}



	// while (1)
	// {
	// 	for (serverblob::iterator it = servers.begin(); it != servers.end(); it++)
	// 		run(*it);
	// }



	// Need to do a while loop like this, but poll must be inside if not it cannot accept anything
	// while(1)
	// {
	// 	for (unsigned int i = 0; i < servers.size(); i++)
	// 	{
	// 		// Run and await connections
	// 		try {
	// 			servers[i].run();
	// 		} catch (std::exception ex) {
	// 			// std::cout << ex.what() << std::endl;
	// 		}
	// 	}
	// }

