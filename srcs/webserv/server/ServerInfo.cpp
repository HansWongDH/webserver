# include "ServerInfo.hpp"

ft::ServerInfo::ServerInfo()
{

}

ft::ServerInfo::~ServerInfo()
{
	
}

ft::ServerInfo::ServerInfo(const ServerInfo& other)
{
	this->config = other.config;
	this->location = other.location;
}

void	ft::ServerInfo::addConfig(key_type key, value_type value)
{
	config.insert(std::make_pair(key, value));
}

void	ft::ServerInfo::insertConfig(const config_type& other)
{
	this->config = other;
}

ft::ServerInfo::config_type	ft::ServerInfo::createConfig(key_type key, value_type value)
{
	config_type tmp;
	tmp.insert(std::make_pair(key, value));
	return tmp;
}

void	ft::ServerInfo::addLocation(key_type key, config_type config)
{
	location.insert(std::make_pair(key, config));
}

ft::ServerInfo::location_type	ft::ServerInfo::getLocation(void) const
{
	return this->location;
}

void	ft::ServerInfo::printConfig(void)
{
	printing(this->config);
}

void	ft::ServerInfo::printing(config_type& con)
{
	config_type::iterator it = con.begin();
	for (; it != con.end(); it++)
	{ 
		std::cout<< "key = " << it->first << std::endl;
		for (value_type::iterator vt = it->second.begin(); vt != it->second.end(); vt++)
			std::cout<< "value = " << *vt << std::endl;
	}
}

ft::ServerInfo::value_type	ft::ServerInfo::getConfigInfo(const key_type& key)
{
	return (getinfo(this->config, key));
}

ft::ServerInfo::value_type	ft::ServerInfo::getinfo(config_type& conf, const key_type& key)
{
	config_iterator it;
	it = conf.find(key);
	if (it == conf.end())
		throw std::out_of_range("Key not found");
	return it->second;
}

ft::ServerInfo::value_type	ft::ServerInfo::getLocationKey(void)
{
	value_type	keyvector;
	for (location_iterator it = location.begin(); it != location.end(); it++)
		keyvector.push_back(it->first);
	return keyvector;
}

ft::ServerInfo::value_type	ft::ServerInfo::getLocationInfo(const key_type &loc, const key_type& key)
{
	location_iterator it;
	it = location.find(loc);
	if (it == location.end())
	{
		std::cout << "here 1" << std::endl;
		throw std::out_of_range("Key not found");
	}
	return (getinfo(it->second, key));
}

int	ft::ServerInfo::getPortNo(void)
{
	value_type lol = getConfigInfo("listen");
	stringstream ss(lol.front());
	int	port;
	ss >> port;
	return (port);
}

ft::ServerInfo::location_type	ft::ServerInfo::getLocation(void)
{
	return this->location;
}

void	ft::ServerInfo::clear()
{
	config.clear();
	location.clear();
}

void	ft::ServerInfo::printLocation()
{
	location_type::iterator it = location.begin();
	for (; it != location.end(); it++)
	{
		std::cout <<"map key === " << it->first << " | map value |" <<  std::endl;
		printing(it->second);
	}
}

ft::ServerInfo::config_type	ft::ServerInfo::getConfig(void)
{
	return this->config;
}

