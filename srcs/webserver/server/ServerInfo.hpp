#ifndef SERVERINFO_HPP
#define SERVERINFO_HPP

# include <map>
# include <iostream>
# include <vector>
# include "Socket.hpp"

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
		typedef map<string, value_type>			config_type;
		typedef	multimap<string, config_type>	location_type;
		typedef	config_type::iterator			config_iterator;
		typedef	location_type::iterator			location_iterator;
		typedef value_type::iterator			iterator;
		
		ServerInfo();
		~ServerInfo();
		ServerInfo(const ServerInfo& other);

		void			addConfig(key_type key, value_type value);
		void			insertConfig(const config_type& other);
		config_type		createConfig(key_type key, value_type value);
		void			addLocation(key_type key, config_type config);
		location_type	getLocation(void) const;
		void			printConfig(void);
		void			printing(config_type& con);
		value_type		getConfigInfo(const key_type& key);
		value_type		getinfo(config_type& conf, const key_type& key);
		value_type		getLocationKey(void);
		value_type		getLocationInfo(const key_type &loc, const key_type& key);
		int				getPortNo(void);
		location_type	getLocation(void);
		void			clear();
		void			printLocation();
		config_type		getConfig(void);
		
		private:
			config_type		config;
			location_type	location;
	};	
}

#endif
