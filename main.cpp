#include "Parser/Parser.hpp"
#include <iostream>
#include <fstream>
#include <string>

int	main(void)
{
	std::ifstream file;
	std::string holder;
	file.open("example.conf");
	ft::Parser lol;
	lol.readfile(file);

}