#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <vector>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include "utils.hpp"
#include "ParsingRequest.hpp"
#include "CreateResponse.hpp"


char **ft_split(const char *str);
std::vector<std::string>	ft_split_header(std::string str);
class Server;


class Client
{

	public:

		Client(Server *server, int clientfd);
		// Client( Client const & src );
		~Client();

		void displayRequest();
		void displayFullRequest();
		void readRequest();
		void readRequest1();
		void parseHeader(std::string buf);
		void sendResponse();
		void createResponse();

		std::vector<std::string> getRequest() const;

	private:

		std::string _requestLine;
		std::vector<std::string> _request;
		std::map<std::string, std::string> _requestmap;
		std::string _response;

		int _clientfd;
		Server *_server;

		int headerIsRead;


};

// std::ostream &			operator<<( std::ostream & o, Client const & i );

#endif /* ********************************************************** CLIENT_H */