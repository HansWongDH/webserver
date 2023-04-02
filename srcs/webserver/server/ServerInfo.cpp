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

ft::ServerInfo&	ft::ServerInfo::operator=(const ServerInfo& other)
{
	this->config = other.config;
	this->location = other.location;
	return *this;
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
	if (this->config.empty())
		std::cout << "THIS IS EMPTY U FUCKER" << std::endl;
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

ft::ServerInfo::value_type	ft::ServerInfo::getinfo(const config_type& conf, const key_type& key)
{
	if (conf.find(key) == conf.end())
		throw std::out_of_range("Key not found");
	return conf.find(key)->second;
}

int		ft::ServerInfo::getLocationCount(const key_type& key)
{
	return location.count(key);
}

int		ft::ServerInfo::getConfigCount(const key_type& key)
{
	return config.count(key);
}
ft::ServerInfo::config_type	ft::ServerInfo::getLocationKey(const key_type& key)
{
	if (location.find(key) == location.end())
	{
		throw std::out_of_range("Key not found");
	}
	return location.find(key)->second;
}

ft::ServerInfo::value_type	ft::ServerInfo::getLocationInfo(const key_type &loc, const key_type& key)
{
	return (getinfo(getLocationKey(loc), key));
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

