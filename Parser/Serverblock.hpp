#ifndef SERVERBLOCK_HPP
# define SERVERBLOCK_HPP

# include <map>
# include <iostream>
# include <vector>

using std::multimap;
using std::map;
using std::string;
using std::vector;

namespace ft{
	class ServerBlock{
		public:
		typedef string							key_type;
		typedef vector<string>					value_type;
		typedef map<string, value_type >		config_type;
		typedef	multimap<string, value_type >	location_type;
		

		void	addConfig(key_type key, value_type value)
		{
			config.insert(std::make_pair(key, value));
		}

		void	addLocation(key_type key, value_type value)
		{
			location.insert(std::make_pair(key, value));
		}

		location_type	getLocation(void) const
		{
			return this->location;
		}
		
		config_type	getConfig(void) const
		{
			return this->config;
		}
		private:
			config_type		config;
			location_type	location;
	};
}

#endif