#ifndef HTTPSERVER_HPP
#define HTTPSERVER_HPP

#include <vector>
#include "./server/Server.hpp"
#include "./client/Client.hpp"
#include <unistd.h>
#include <poll.h>
using std::map;
using std::cout;
using std::endl;

/**
 * To make life easier instead of navigating through multiple class's getter, Webserv Class is created to serve as shortcut
 * so we can create function that return something deeply burried within class within class.
 * 
 * another thing is that I added a map which contain a client_fd as key and server_fd as value to be able to traceback to the server it connected to
 * since we only know the client fd from pollfd struct.
 */
namespace ft
{
	class Webserv {
		public:
			typedef ft::Client				client_type;
			typedef	ft::Server				server_type;
			typedef server_type&			server_reference;
			typedef map<int, ft::Server>	servers_map;
			typedef map<int, ft::Client*>	clients_map;
			typedef map<string, string>		env_map;
			typedef vector<pollfd>			poll_vector;
			typedef servers_map::iterator	servers_iterator;
			typedef servers_map::const_iterator	servers_const_iterator;
			typedef clients_map::iterator	clients_iterator;

			Webserv();
			Webserv(char **env);
			~Webserv();
			Webserv(const Webserv& other);
			void				insertServer(int server_fd, const server_type& server);
			void				insertClient(int server_id, int client_fd);
			void				eraseClient(int client_fd);
			env_map				charToMap(char **env);
			void				runServer(int timeout);
			void				initializePollstruct(void);
		private:
			servers_map	servers;
			clients_map	clients;
			env_map		envs;
			poll_vector	fds;
			

	};
}

#endif
