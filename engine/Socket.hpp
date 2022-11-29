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
#include <map>
#include <fstream>
#include <sstream>

#include <cerrno>

#include "utils.hpp"





class Socket
{

public:

	Socket(int port, int domain = AF_INET, int type = SOCK_STREAM, int protocol = 0);
	~Socket();


	void waitRequest();

	void displayRequest();

	void sendResponse(std::string str);

	int getPort() const;

	std::vector<std::string> getRequest() const;

private:
	Socket();

	struct sockaddr_in _addr;
	void setStruct();

	std::vector<std::string> _request;
	int _newsocket;

	int _port;
	int _sockfd;

	std::string _currentDirectory;

};

std::ostream &			operator<<( std::ostream & o, Socket const & i );

#endif /* ********************************************************** SOCKET_H */