#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
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
		std::string protocol;

	}				t_requestData;
	s_requestData _requestData;

	typedef struct	s_headerData
	{
		std::string protocol;
		std::string	statusCode;
		std::string statusMessage;
		std::string contentLenght;
		std::string contentType;

	}				t_headerData;
	s_headerData _headerData;
	std::vector<std::string> _responseHeader;
	std::map<std::string, std::string> _switchFilesExtension;
	void fillFilesExtension();
	void fillHeaderData();


	std::string _fileToSend;
	void foundFileToSend();

	void parsingRequest();
	void sendResponse();

};

std::ostream &			operator<<( std::ostream & o, Socket const & i );

#endif /* ********************************************************** SOCKET_H */