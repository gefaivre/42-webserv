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


	void waitAndCopyRequest();

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

	struct	s_parsing
	{
		std::string methode;
		std::string file;

	}				t_parsing;
	s_parsing _parsing;

	struct	s_response
	{
		std::string methode;
		std::string file;

	}				t_response;
	s_response _response;

	void parsingRequest();
	void sendResponse(std::ifstream &fileToSend);

};

std::ostream &			operator<<( std::ostream & o, Socket const & i );

#endif /* ********************************************************** SOCKET_H */