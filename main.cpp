#include "Parser/Parser.hpp"
#include <iostream>
#include <fstream>
#include <string>

int	main(void)
{
	std::ifstream file;
	std::string holder;
	file.open("./config/example.conf");
	const char* temp[]	= {"listen", "server_name", "root"};
	std::vector<std::string> test (temp, temp + 3);
	ft::Parser lol(test);
	lol.readfile(file);

	vector<ft::ServerBlock> vet;
	vet = lol.getServer();
	for (std::vector<ft::ServerBlock>::iterator it = vet.begin(); it != vet.end(); it++)
	{
		// it->printConfig();
		it->printLocation();
	}

}