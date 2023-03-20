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
namespace 	ft{
	class Parser {
		public:
			typedef ft::ServerBlock		value_type;
			typedef	multimap<string, ft::ServerBlock>	server_type;
			typedef	server_type::iterator	iterator;

			Parser()
			{

			};

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
				value_type	info;
				vector<string> block;
				while (getline(file, tmp))
				{
					if (findString(tmp, "server"))
					{
						while (getline(file, tmp))
						{
							if (findString(tmp, "listen"))
							{
								std::stringstream test(tmp);
								string	key;
								string	value;
								test >> key;
								test >> value;
								std::cout << key << "====" << value << std::endl;
								block.push_back(value);
								info.addConfig(key, block);
							}

						}
					}
				}
			}
			// virtual int error_handling(void) = 0;
			void	addServerblock(string key,	value_type value)
			{
				server_info.insert(std::make_pair(key, value));
			}
			std::pair<iterator, iterator>	getServer(const string& key)
			{
				return server_info.equal_range(key);
			}

		private:
			multimap<string, ft::ServerBlock> server_info;
	};
}
#endif