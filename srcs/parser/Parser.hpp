#ifndef	PARSER_HPP
#define PARSER_HPP
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <string>
#include <sstream>
#include "../webserv/server/ServerInfo.hpp"
#include "../webserv/Webserv.hpp"

using std::string;
using std::multimap;
using std::ifstream;
using std::stringstream;

namespace ft
{
	class Parser {
		public:
			typedef ft::ServerInfo					value_type;
			typedef	ft::ServerInfo::value_type		string_type;
			typedef	string_type::iterator			string_iterator;
			typedef ft::Server						server_type;
			typedef ft::ServerInfo::config_type		config_type;
			typedef	vector<value_type>				value_vector;
			typedef	value_vector::iterator			value_iterator;
			typedef ft::Webserv						servers_type;
		
			Parser(ifstream &file, string_type key) : info(key)
			{
				readfile(file);
				errorChecking();
			}

			~Parser()
			{

			}

			value_vector getServerInfo()
			{
				return this->conf;
			}

			servers_type	getWebserv() const
			{
				return this->servers;
			}

			void	readfile(ifstream &file)
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
			
			void	searchServer(ifstream& file, value_type& store)
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

			void	searchLocation(ifstream& file, value_type& store, string map_key)
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

			void	checkPort(value_type config)
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

			void	endoflineCheck(string_type& config)
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

			void	initalizeServer(int port, value_type block)
			{
				server_type server(port, block);
				this->servers.insertServer(server.getFd(), server);
			}

			void	errorChecking(void)
			{
				for (value_iterator it = conf.begin(); it != conf.end(); it++)
				{
					checkPort(*it);
					initalizeServer(it->getPortNo(), *it);
				}
			}
			
			private:
				vector<string>	info;
				value_vector	conf;
				servers_type	servers;
	};
}
#endif