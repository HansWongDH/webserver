#ifndef SERVER_HPP
# define SERVER_HPP

# include <unistd.h>
# include <map>
# include <vector>
# include <string>
# include "Socket.hpp"
# include "../client/Client.hpp"

using std::map;
using std::vector;
using std::string;

namespace ft
{
    class Server
    {
        public:
            typedef vector<string>					value_type;
            typedef map<string, value_type>         config_type;
            typedef	multimap<string, config_type>	location_type;
            typedef	config_type::iterator			config_iterator;
            typedef	location_type::iterator			location_iterator;
            typedef value_type::iterator			value_iterator;

            Server();
            Server(int port, ft::ServerInfo serv);
            ~Server();
            Server(const Server& other);

            ft::Socket      getSocket(void);
            int             getFd(void);
            ft::ServerInfo  getInfo(void);
            ft::ServerInfo* getInfoAddress(void);
        private:
            ft::Socket      socket;
            ft::ServerInfo  *info;
    };
}

#endif