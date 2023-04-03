#ifndef HTTPSERVER_HPP
#define HTTPSERVER_HPP

#include <vector>
#include "./server/Server.hpp"
#include "./client/Client.hpp"
#include  <unordered_map>
using std::map;

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
			typedef map<int, ft::Client>	clients_map;
			typedef map<string, string>		env_map;
			typedef servers_map::iterator	servers_iterator;
			typedef servers_map::const_iterator	servers_const_iterator;
			typedef clients_map::iterator	clients_iterator;

			Webserv();
			Webserv(char **env);
			~Webserv();
			Webserv(const Webserv& other);
			void				insertServer(int server_fd, server_type server);
			int					server_size();
			int					client_size();
			servers_iterator	begin();
			servers_iterator 	end();
			ft::Socket			serverSocket(servers_iterator it);
			ft::Server			findServer(int	server_fd);
			int					findServerfd(int client_fd);
			void				insertClient(int server_id, int client_fd);
			void				eraseClient(int client_fd);
			ft::Client			findClient(int client_fd);
			char**				mapToChar(env_map envs);
			env_map				charToMap(char **env);
			void				printEnv();

		private:
			servers_map	servers;
			clients_map	clients;
			env_map		envs;

	};
}

#endif
