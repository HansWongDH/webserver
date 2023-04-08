#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include <iostream>
#include <stdio.h>
#include <dirent.h>
#include <sstream>
#include <fstream>
#include <string>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdarg.h>
#include "../request/Request.hpp"
#include "../../server/ServerInfo.hpp"
#include "../../../../includes/colour.h"

using std::string;

enum {
	OK = 200,
	CREATED = 201,
	ACCEPTED = 202,
	NO_CONTENT = 204,
	METHOD_NOT_ALLOWED = 405,
	NOT_FOUND = 404,
	PAYLOAD_TOO_LARGE = 413,
	MOVED_PERMANENTLY = 301,
	TEMPORARY_REDIRECT = 307,
	PERMANENTLY_REDIRECT = 308,
	INTERNAL_SERVER_ERROR = 500

};
namespace ft
{
	class Response{
		public:
			Response(ft::ServerInfo* info, const map<string,string>& env);
			~Response();

			int	insertResponse(string infile);
			void	parseResponse(ft::Request *request);
			void	methodGet(ft::Request *request);
			void	methodPost(ft::Request *request);
			void	methodDelete(ft::Request *request);
			void	returnResponse(int fd);
			string	responseHeader(int status_code);
			string 	fileOpen(ft::Request *request);
			bool	empty();

		private:
			ft::ServerInfo	*info;
			string			_response;
			int				size;
			int				status_code;
			string			root;
			vector<string>	index;
			bool			auto_index;
			bool			redirection;
			bool			cookie;
			string			prefix;
			string			target;
			map<string, string>	env;
			
			string	prefererentialPrefixMatch(string url);
			string	errorPage(void);
			int	allowedMethod(ft::Request *request);
			string	defaultErrorPage(void);
			vector<string>	initalizeLocationConfig(string prefix, string value);
			string autoIndexGenerator(string prefix);
			string pageRedirection(string target);
			int	executeCGI(string prefix, ft::Request *request);
			bool	checkCookie(ft::Request *request);
			string	generateCookie();
	};
} // namespace ft

#endif
