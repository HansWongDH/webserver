#include "Parser.hpp"

ft::Parser::Parser(ifstream &file, string_type key, char **env) : info(key), servers(env)
{
	readfile(file);
	errorChecking();
}

ft::Parser::~Parser()
{

}

ft::Parser::value_vector ft::Parser::getServerInfo()
{
	return this->conf;
}

ft::Parser::servers_type	ft::Parser::getWebserv()
{
	return this->servers;
}

void	ft::Parser::readfile(ifstream &file)
{
	string	tmp;
	string key;
	string	value;
	value_type	store;
	while (getline(file, tmp))
	{
		stringstream ss(tmp);
		ss >> key >> value;
		if (!key.compare("server") && !value.compare("{"))
		{
			searchServer(file, store);
			this->conf.push_back(store);
			store.clear();
		}
		key.clear();
		value.clear();
	}
}

void	ft::Parser::searchServer(ifstream& file, value_type& store)
{
	string tmp;
	string key;
	string value;
	string bracket;
	vector<string> block;
	while (getline(file, tmp))
	{
		stringstream ss2(tmp);
		ss2 >> key;
		if (!key.compare("}"))
			break ;
		if (!key.compare("location"))
		{
			ss2 >> value;
			ss2 >> bracket;
			if (!bracket.compare("{"))
				searchLocation(file, store, value);
			value.clear();
			bracket.clear();
		}
		for (string_iterator it = info.begin(); it != info.end(); it++)
		{
			if(!key.compare(*it))
			{
				while (ss2 >> value)
					block.push_back(value);
				endoflineCheck(block);
				store.addConfig(key, block);
				block.clear();
			}
		}
		key.clear();
	}
}

void	ft::Parser::searchLocation(ifstream& file, value_type& store, string map_key)
{
	string	tmp;
	string 	key;
	string	value;
	vector<string> block;
	config_type	config;

	while(getline(file, tmp))
	{
		stringstream ss(tmp);
		ss >> key;
		if (!key.compare("}"))
			break ;
		while (ss >> value)
		{
			block.push_back(value);
		}
		endoflineCheck(block);
		config.insert(std::make_pair(key, block));
		block.clear();
		key.clear();
	}
	store.addLocation(map_key, config);
}

void	ft::Parser::checkPort(value_type config)
{
	string_type port = config.getConfigInfo("listen");
	if (port.size() < 1)
		throw std::out_of_range("Listen port not initalized");
	if (port.size() != 1)
		throw std::out_of_range("Multiple listen port");
	string test = *(port.begin());
	for (string::iterator it = test.begin(); it != test.end(); it++)
		if(!isdigit(*it))
			throw std::invalid_argument("listen port is not a digit");
}

void	ft::Parser::endoflineCheck(string_type& config)
{
	if (!config.empty())
	{
		string text = config.back();
		if (!text.empty())
		{
			config.back().pop_back();
		}
	}
}

void	ft::Parser::initalizeServer(int port, value_type block)
{
	server_type *server = new server_type(port, block);
	this->servers.insertServer(server->getFd(), server);
}

void	ft::Parser::errorChecking(void)
{
	for (value_iterator it = conf.begin(); it != conf.end(); it++)
	{
		it->printConfig();
		checkPort(*it);

		initalizeServer(it->getPortNo(), *it);
	}
}
