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
		typedef	multimap<string, config_type>		location_type;
		

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

		config_type	getConfig(void)
		{
			return this->config;
		}
		private:
			config_type		config;
			location_type	location;
	};
}

#endif