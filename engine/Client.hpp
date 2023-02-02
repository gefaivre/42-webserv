#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <algorithm>
#include <fcntl.h>
#include <sys/wait.h>
#include "utils.hpp"
#include "ParsingRequest.hpp"
#include "CGI.hpp"
#include "CreateResponse.hpp"
#include "define.hpp"


class Server;
class CreateResponse;


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

		int readRequestHeader();
		void readRequestBody();
		void readRequest1();

		int CreateAndSendResponse();
		int sendResponse();


		std::vector<std::string> getRequest() const;

	private:

		std::string _requestLine;
		std::vector<std::string> _request;
		std::map<std::string, std::string> _requestmap;
		std::string _requestBody;
		std::map<std::string, std::string> _requestmapBody;

		std::string _response;

		int _clientfd;
		Server *_server;

		char * _responsePointer;

		int _errorcode;

		size_t _bodyContentLenght;

		std::string _cgiResponse;
		size_t setBodyContentLenght();
		void transformRequestVectorToMap();
		size_t findBodyContentLenght();
		std::string getHost();
		void setKeepAlive();
		bool _isKeepAlive;
		bool parseChunked();
		std::string chunkedBody();

		void EndOfRead();
		void resetClient();


		// POLLING CREATE RESPONSE
		bool _headerIsRead;
		bool _firstTimeBody;
		bool _firstTimeCreate;
		bool _createIsFinish;
		bool _isSend;
		size_t _moverSave;

		CreateResponse *_createR;



};

// std::ostream &			operator<<( std::ostream & o, Client const & i );

#endif /* ********************************************************** CLIENT_H */