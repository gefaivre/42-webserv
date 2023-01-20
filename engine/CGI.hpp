#ifndef CGI_HPP
#define CGI_HPP

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "Location.hpp"
#include "Server.hpp"
#include "utils.hpp"

// class Server;

class CGI
{
	public:
		CGI(std::vector<std::string> *request, Server * server, \
		int *_errorcode, std::string *_requestBody, \
		std::map<std::string, std::string> *_requestmap, std::string *_cgiResponse);
		CGI( CGI const & src );
		CGI &operator=( CGI const & rhs );
		~CGI();
		void verifyCgi();
		std::vector<std::string> setAuthorizedExtension(std::vector<std::string> ext);

	private:

		Location _loc;
		std::string _getParams;
		std::vector<std::string> authorizedExtension;

		std::string *_requestBody;
		Server *_server;
		int *_errorcode;
		std::vector<std::string> *_request;
		std::map<std::string, std::string> *_requestmap;
		std::string *_cgiResponse;


		int workGetCgi(std::string format, std::string requestFile);
		int workPostCgi(std::string format, std::string requestFile);
		int workDeleteCgi(std::string format, std::string requestFile);
		std::string findValueEnvCgi(std::string key);

};


#endif