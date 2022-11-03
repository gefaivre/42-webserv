#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include <cerrno>



class Socket
{

public:

	Socket(int port, int domain = AF_INET, int type = SOCK_STREAM, int protocol = 0);
	~Socket();


	void waitAndParseRequest();

	void displayRequest();

	void createAndSendResponse();

	int getPort() const;

private:
	Socket();

	struct sockaddr_in _addr;
	void setStruct();

	std::vector<std::string> _request;
	int _newsocket;

	int _port;
	int _sockfd;

	typedef struct	s_requestData
	{
		std::string methode;
		std::string path;
		std::string protocolVersion;

	}				t_requestData;
	s_requestData _requestData;

	teypedef struct	s_headerData
	{
		std::string protocolVersion;
		int			statusCode;
		std::string statusMessage;
		std::string contentLenght;
		std::string contentType;

	}				t_headerData;
	s_headerData _headerData;
	std::vector<std::string> _responseHeader;
	void fillHeaderData();

	void parsingRequest();
	void sendResponse(std::ifstream &fileToSend);

};

std::ostream &			operator<<( std::ostream & o, Socket const & i );

#endif /* ********************************************************** SOCKET_H */