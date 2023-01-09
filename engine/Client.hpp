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

		Client();
		Client(Server *server, int clientfd);
		Client( Client const & src );
		Client &operator=( Client const & rhs );
		~Client();

		void displayRequest();
		void displayFullRequest();
		void displayFullBody();
		void readRequest();
		int readRequest1();
		void parseHeader(std::string buf);
		int	 sendResponse();
		void createResponse();


		std::vector<std::string> getRequest() const;

	private:

		std::string _requestLine;
		std::vector<std::string> _request;
		std::map<std::string, std::string> _requestmap;
		std::string _requestBody;

		std::string _headerResponse;
		std::string _bodyResponse;

		CreateResponse *_createResponse;

		int _clientfd;
		Server * _server;

		bool _isSend;
		bool _headerIsSend;

		bool _headerIsRead;
		bool _firstTimeBody;
		size_t _bodyContentLenght;


		size_t findContentLenght();
		void transformRequestVectorToMap();
		size_t findBodyContentLenght();
		void setKeepAlive();
		bool _isKeepAlive;

		void EndOfRead();
		void resetClient();


};

// std::ostream &			operator<<( std::ostream & o, Client const & i );

#endif /* ********************************************************** CLIENT_H */