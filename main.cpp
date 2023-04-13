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
using std::string;

int main(int ac, char **av, char **env)
{	
	if (ac != 2)
	{
		throw std::invalid_argument("Too many arguments");
	}
	string filename (av[1]);
	if (filename.substr(filename.find(".")).compare(".conf"))
	{
		throw std::invalid_argument("Invalid file type");
	}
	std::ifstream file;
	file.open(av[1]);
	if (!file.is_open())
		throw std::invalid_argument("Invalid config files");
	const char *temp[] = {"listen", "allow_methods", "root", "index", "404", "client_max_body_size"};
	std::vector<std::string> test(temp, temp + 6);
	ft::Parser parsed(file, test, env);
	ft::Webserv	WebServer(parsed.getWebserv());
	WebServer.initializePollstruct();
	WebServer.runServer(1000);
	
}

