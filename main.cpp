#include "Parser/Parser.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include "./Server/Server.hpp"

int	main(void)
{
	std::ifstream file;
	std::string holder;
	file.open("./config/example.conf");
	const char* temp[]	= {"listen", "server_name", "root"};
	std::vector<std::string> test (temp, temp + 3);
	ft::Parser lol(file, test);
	// lol.readfile(file);

	vector<ft::ServerBlock> vet;
	vet = lol.getServerInfo();
	for (std::vector<ft::ServerBlock>::iterator it = vet.begin(); it != vet.end(); it++)
	{
		it->printConfig();
		std::cout << "---------------------------" << std::endl;
		it->printLocation();
		std::cout << "===========================" << std::endl;
	}

	// Initialize server
	ft::Server server;

	// Run and await connections
	server.run();
}