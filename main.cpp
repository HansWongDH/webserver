#include "Parser/Parser.hpp"
#include <iostream>
#include <fstream>
#include <string>

int	main(void)
{
	std::ifstream file;
	std::string holder;
	file.open("example.conf");
	ft::Parser lol({"listen", "server_name", "root"});
	lol.readfile(file);

	vector<ft::ServerBlock> vet;
	vet = lol.getServer();
	for (std::vector<ft::ServerBlock>::iterator it = vet.begin(); it != vet.end(); it++)
	{
		// it->printConfig();
		it->printLocation();
	}

}