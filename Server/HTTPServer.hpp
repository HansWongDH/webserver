#ifndef HTTPSERVER_HPP
#define HTTPSERVER_HPP

#include <vector>
#include "Server.hpp"

using std::map;

/**
 * To make life easier instead of navigating through multiple class's getter, HTTPServer Class is created to serve as shortcut
 * so we can create function that return something deeply burried within class within class.
 * 
 * another thing is that I added a map which contain a client_fd as key and server_fd as value to be able to traceback to the server it connected to
 * since we only know the client fd from pollfd struct.
 * 
 * 
 * 
 */
namespace ft
{
	class HTTPServer {

		public:
			typedef ft::Client				client_type;
			typedef	ft::Server				value_type;
			typedef value_type&				reference;
			typedef map<int, ft::Server>	servermap_type;
			typedef map<int, int>			clientmap_type;
			typedef servermap_type::iterator	iterator;
			typedef clientmap_type::iterator	clientmap_iterator;


			HTTPServer() : Serverlist() {}
			~HTTPServer(){}

			HTTPServer(const HTTPServer& other) : Serverlist(other.Serverlist), clientlist(other.clientlist)
			{
			}

			// HTTPServer&	operator=(HTTPServer& other)
			// {
			// 	HTTPServer tmp(other);
			// 	return tmp;
			// }
			void	insertServer(int server_fd, value_type server)
			{
				Serverlist.insert(std::make_pair(server_fd, server));
			}

			int		size()
			{
				return Serverlist.size();
			}

			iterator	begin()
			{
				return this->Serverlist.begin();
			}

			iterator 	end()
			{
				return this->Serverlist.end();
			}

			ft::Socket	serverSocket(iterator it)
			{
				return it->second.getSocket();
			}

			ft::Server	findServer(int	server_fd)
			{
				return (this->Serverlist.find(server_fd))->second;
			}
			// reference	operator[](int n)
			// {
			// 	return *(this->begin() + n);
			// }
			void	newClient(int client_fd, int server_fd)
			{
				this->clientlist.insert(std::make_pair(client_fd, server_fd));
				findServer(server_fd).insertClient(server_fd, client_fd);
			}

			int	findServerfd(int client_fd)
			{
				return clientlist.find(client_fd)->second;
			}

			void	deleteClient(int client_fd)
			{
				findServer(findServerfd(client_fd)).eraseClient(client_fd);
			}
			/**
			 * @brief 
			 * 
			 * @for client to find server
			 */
			// reference	findServer(int server_id)
			// {
			// 	return operator[](server_id);
			// }

			// client_type	findClient(int server_id, int fd)
			// {
			// 	this->Serverlist.at()
			// }

		private:
			servermap_type	Serverlist;
			clientmap_type	clientlist;
	};
} // namespace ft


#endif
