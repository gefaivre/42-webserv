#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <vector>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <algorithm>
#include <fcntl.h>
#include <sys/wait.h>
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


		std::vector<std::string> authorizedExtension;
		std::vector<std::string> getRequest() const;

	private:

		std::string _requestLine;
		std::vector<std::string> _request;
		std::map<std::string, std::string> _requestmap;
		std::string _requestBody;
		std::map<std::string, std::string> _requestmapBody;

		std::string _response;

		int _clientfd;
		Server * _server;

		bool _isSend;
		size_t _moverSave;
		char * _responsePointer;

		int _errorcode;
	
		Location _loc;

		std::string _getParams;

		bool _headerIsRead;
		bool _firstTimeBody;
		size_t _bodyContentLenght;

		std::string _cgiResponse;
		size_t findContentLenght();
		void transformRequestVectorToMap();
		void transformBodyVectorToMap();
		std::string	ft_find_boundary();
		void saveFile();
		void verifyCgi();
		int workGetCgi(std::string format, std::string requestFile);
		int workPostCgi(std::string format, std::string requestFile);
		int workDeleteCgi(std::string format, std::string requestFile);
		void transformBodyStringtoMap();
		size_t findBodyContentLenght();
		std::string	findValueEnvCgi(std::string key);
		std::string	findMethod();
		void setKeepAlive();
		bool _isKeepAlive;
		bool parseChunked();
		std::string chunkedBody();

		void EndOfRead();
		void resetClient();


};

// std::ostream &			operator<<( std::ostream & o, Client const & i );

#endif /* ********************************************************** CLIENT_H */