#ifndef	PARSER_HPP
#define PARSER_HPP
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <string>
#include <sstream>
#include "Serverblock.hpp"

using std::string;
using std::multimap;
using std::ifstream;
using std::stringstream;
namespace 	ft{
	class Parser {
		public:
			typedef	ft::ServerBlock::value_type key_type;;
			typedef	key_type::iterator	key_iterator;
			typedef ft::ServerBlock		value_type;
			typedef ft::ServerBlock::config_type	config_type;
			typedef	vector<value_type>	server_type;
			typedef	server_type::iterator	iterator;
		
		private:
			vector<string>	info;
			server_type		server;
		
		public:
			Parser(ifstream &file, key_type key) : info(key)
			{
				readfile(file);
				errorChecking();
			}

			~Parser()
			{

			}

			int	findString(string str, string find)
			{
				int	index;
				if ((index = str.find(find)))
				{
					if (index == 0)
					{
						if (isspace(str[index + find.length() + 1]) || str[index + find.length() + 1] == '\n')
							return 1;
					}
					else
					{
						if (isspace(str[index - 1 ]))
							return 1;
					}
				}
				return 0;
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
						// std::cout << "HERE " << std::endl;
						searchServer(file, store);
						this->server.push_back(store);
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
					for (key_iterator it = info.begin(); it != info.end(); it++)
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
					// std::cout << "location tmp is === " << tmp << std::endl;
					stringstream ss(tmp);
					ss >> key;
					if (!key.compare("}"))
						break ;
					// std::cout << "key === " << key << "i is === " << i <<  std::endl;
					while (ss >> value)
					{
						// std::cout << "value === " << value << std::endl;
						block.push_back(value);
					}
					endoflineCheck(block);
					config.insert(std::make_pair(key, block));
					block.clear();
					key.clear();
				}
				store.addLocation(map_key, config);
			}

			server_type	getServerInfo()
			{
				return this->server;
			}
			void	checkPort(value_type config)
			{
				key_type port = config.getConfigInfo("listen");
				if (port.size() < 1)
					throw std::out_of_range("Listen port not initalized");
				if (port.size() != 1)
					throw std::out_of_range("Multiple listen port");
				string test = *(port.begin());
				// std::cout << "port text" << test << std::endl;
				for (string::iterator it = test.begin(); it != test.end(); it++)
					if(!isdigit(*it))
						throw std::invalid_argument("listen port is not a digit");
			}

			void	endoflineCheck(key_type& config)
			{
				if (!config.empty())
				{
					string text = config.back();
					if (!text.empty())
					{
						// std::cout << " STRING IS === " << text << std::endl;
						// if (text.back() != ';')
						// 	throw std::invalid_argument("end of line missing : ';'");
						// else
							config.back().pop_back();
					}
				}
			}
			
			// void	locationChecking(value_type server)
			// {
			// 	key_type vec = server.getLocationKey();
			// 	for (key_iterator it = vec.begin(); it != vec.end(); it++)
			// 	{
			// 		if (it->front() != '/')
			// 			throw std::invalid_argument("invalid location");
			// 	}
			// }

			void	errorChecking(void)
			{
				for (iterator it = server.begin(); it != server.end(); it++)
				{
					checkPort(*it);
					// locationChecking(*it);
				}
			}

	};
}
#endif