#ifndef LOCATION_BLOCK_HPP
#define LOCATION_BLOCK_HPP

#include <vector>
#include <map>
#include <iostream>

using std::map;
using std::vector;
using std::string;
namespace ft {
	class locationBlock
	{

	public:
		typedef string							key_type;
		typedef vector<string>					value_type;
		typedef	map<key_type, value_type >		location_type;
		locationBlock() {};
		~locationBlock() {};

		void	insertConfig(key_type key, value_type value)
		{
			location.insert(std::make_pair(key, value));
		}
	private:
		location_type	location;
	};
}

#endif
// vector<serverblock>
// serverblock = map<string, vector<string> config and multimap<string, locationblock>
// location block = map<string, vector<string>

// vector<serverblock>
// serverblock = map<string, vector<string> config and multimap<string, map<string, vector<string>>
