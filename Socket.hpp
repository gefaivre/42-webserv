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

#include <cerrno>

class Socket
{

public:

	Socket(int port, int domain = AF_INET, int type = SOCK_STREAM, int protocol = 0);
	// Socket( Socket const & src );
	~Socket();

	void acceptAndRead();
	int getPort() const;

	// Socket &		operator=( Socket const & rhs );

private:
	Socket();

	struct sockaddr_in _addr;
	void setStruct();

	std::vector<std::string> _request;

	int _port;
	int _sockfd;

};

std::ostream &			operator<<( std::ostream & o, Socket const & i );

#endif /* ********************************************************** SOCKET_H */