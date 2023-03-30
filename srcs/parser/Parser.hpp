#ifndef	PARSER_HPP
#define PARSER_HPP
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <string>
#include <sstream>
#include "../webserver/server/ServerInfo.hpp"
#include "../webserver/Webserv.hpp"

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
		
			Parser(ifstream &file, string_type key);
			~Parser();

			value_vector	getServerInfo();
			servers_type	getWebserv();
			void			readfile(ifstream &file);
			void			searchServer(ifstream& file, value_type& store);
			void			searchLocation(ifstream& file, value_type& store, string map_key);
			void			checkPort(value_type config);
			void			endoflineCheck(string_type& config);
			void			initalizeServer(int port, value_type block);
			void			errorChecking(void);
			void			printerror(void) {
				
			}
			private:
				vector<string>	info;
				value_vector	conf;
				servers_type	servers;
	};
}
#endif