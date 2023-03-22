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
			typedef	vector<string>		key_type;
			typedef	key_type::iterator	key_iterator;
			typedef ft::ServerBlock		value_type;
			typedef ft::ServerBlock::config_type	config_type;
			typedef	vector<value_type>	server_type;
			typedef	server_type::iterator	iterator;
		
		private:
			vector<string>	info;
			server_type		server;
		
		public:
			Parser(key_type key) : info(key)
			{

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
						searchServer(file, store);
						this->server.push_back(store);
					}
				}
			}
			
			void	searchServer(ifstream& file, value_type& store)
			{
				string tmp;
				string key;
				string value;
				vector<string> block;
				while (getline(file, tmp))
				{
					// std::cout<< "temp === " << tmp << std::endl;
					stringstream ss2(tmp);
					ss2 >> key;
					if (!key.compare("}"))
						break ;
					if (!key.compare("location"))
					{
						ss2 >> value;
						searchLocation(file, store, value);
						value.clear();
					}
					for (key_iterator it = info.begin(); it != info.end(); it++)
					{
						if(!key.compare(*it))
						{
							while (ss2 >> value)
								block.push_back(value);
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
					config.insert(std::make_pair(key, block));
					block.clear();
					key.clear();
				}
				store.addLocation(map_key, config);
			}

			server_type	getServer()
			{
				return this->server;
			}
	};
}
#endif