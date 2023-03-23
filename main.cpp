#include "Parser/Parser.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <poll.h>
#include "./Server/Server.hpp"


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
	struct pollfd fds[200];

	std::vector<ft::ServerBlock> vet;
	// vet = lol.getServerInfo();
	// // for (std::vector<ft::ServerBlock>::iterator it = vet.begin(); it != vet.end(); it++)
	// // {
	// // 	it->printConfig();
	// // 	std::cout << "---------------------------" << std::endl;
	// // 	it->printLocation();
	// // 	std::cout << "===========================" << std::endl;
	// // }

	std::vector<ft::Server> servers;

	typedef std::vector<ft::Server> serverblob;
	for (std::vector<ft::ServerBlock>::iterator it = vet.begin(); it != vet.end(); it++)
	{
		// Initialize server
		ft::Server server = ft::Server(it->getPortNo());
		servers.push_back(server);
	}

	for (int i = 0; i < servers.size(); i++)
	{
		fds[i].fd = servers[i].getSocket().getSocketfd();
		int poll_length = 1;
		if (poll(fds, poll_length, 100))
		{

		int	x = 0;
		while (x < poll_length)
		{
			if (fds[x].fd)
			{
				servers[i].bind_fd(servers[i].getSocket().accept_connection());
				fds[poll_length + 1].fd = servers[i].getFd();
				fds[poll_length + 1].events = POLLIN;
			}
			else
				return;
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

}