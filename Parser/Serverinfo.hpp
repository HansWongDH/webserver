#ifndef SERVERINFO_HPP
#define SERVERINFO_HPP

# include <map>
# include <iostream>
# include <vector>
# include "../Server/Socket.hpp"

#include <sstream>

using std::multimap;
using std::map;
using std::string;
using std::vector;
using std::stringstream;

namespace ft{
	class ServerInfo{
		public:
		typedef string							key_type;
		typedef vector<string>					value_type;
		typedef map<string, value_type >		config_type;
		typedef	multimap<string, config_type>		location_type;
		typedef	config_type::iterator				config_iterator;
		typedef	location_type::iterator				location_iterator;
		typedef value_type::iterator				iterator;
		
		ServerInfo()
		{
		
		}
		~ServerInfo()
		{
			
		}

		ServerInfo(const ServerInfo& other)
		{
			this->config = other.config;
			this->location = other.location;
		}

		void	addConfig(key_type key, value_type value)
		{
			config.insert(std::make_pair(key, value));
		}

		void	insertConfig(const config_type& other)
		{
			this->config = other;
		}

		config_type	createConfig(key_type key, value_type value)
		{
			config_type tmp;
			tmp.insert(std::make_pair(key, value));
			return tmp;
		}

		void	addLocation(key_type key, config_type config)
		{
			location.insert(std::make_pair(key, config));
		}

		location_type	getLocation(void) const
		{
			return this->location;
		}
		
		void	printConfig(void)
		{
			printing(this->config);
		}
		void	printing(config_type& con)
		{
			config_type::iterator it = con.begin();
			for (; it != con.end(); it++)
			{ 
				std::cout<< "key = " << it->first << std::endl;
				for (value_type::iterator vt = it->second.begin(); vt != it->second.end(); vt++)
					std::cout<< "value = " << *vt << std::endl;
			}
		}

		value_type	getConfigInfo(const key_type& key)
		{
			return (getinfo(this->config, key));
		}

		value_type	getinfo(config_type& conf, const key_type& key)
		{
			config_iterator it;
			it = conf.find(key);
			if (it == conf.end())
				throw std::out_of_range("Key not found");
			return it->second;
		}


		value_type	getLocationKey(void)
		{
			value_type	keyvector;
			for (location_iterator it = location.begin(); it != location.end(); it++)
				keyvector.push_back(it->first);
			return keyvector;
		}
		
		value_type	getLocationInfo(const key_type &loc, const key_type& key)
		{
			location_iterator it;
			it = location.find(loc);
			if (it == location.end())
				throw std::out_of_range("Key not found");
			return (getinfo(it->second, key));
		}

		int	getPortNo(void)
		{
			value_type lol = getConfigInfo("listen");
			stringstream ss(lol.front());
			int	port;
			ss >> port;
			return (port);
		}

		location_type	getLocation(void)
		{
			return this->location;
		}

		void	clear()
		{
			config.clear();
			location.clear();
		}
		void	printLocation()
		{
			location_type::iterator it = location.begin();
			for (; it != location.end(); it++)
			{
				std::cout <<"map key === " << it->first << " | map value |" <<  std::endl;
				printing(it->second);
			}

		}

		// void	updateConfig(config_type con)
		// {
		// 	this->config = con;
		// }


		// void	updateConfigz(config_type ori, config_type update)
		// {
		// 	ori = update;
		// }

		// void	updateLocation(location_type loc)
		// {
		// 	this->location = loc;
		// 	for (location_iterator it = loc.begin(); it != loc.end(); it++)
		// }

		config_type	getConfig(void)
		{
			return this->config;
		}
		private:
			config_type		config; map <string, vector<string>>
			location_type	location;  map<string,
	};	
}

#endif


