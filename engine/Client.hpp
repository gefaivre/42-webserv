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
class Server;


class Client
{

	public:

		Client(Server *server, int clientfd);
		// Client( Client const & src );
		~Client();

		void displayRequest();
		void readRequest();
		void sendResponse();
		void createResponse();

		std::vector<std::string> getRequest() const;

	private:

		std::vector<std::string> _request;
		std::string _response;

		int _clientfd;
		Server *_server;


};

// std::ostream &			operator<<( std::ostream & o, Client const & i );

#endif /* ********************************************************** CLIENT_H */