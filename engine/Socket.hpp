#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "utils.hpp"
#include "general_includes.hpp"

char **ft_split(const char *str);

class Socket
{

public:

	Socket(int port, int domain = AF_INET, int type = SOCK_STREAM, int protocol = 0);
	~Socket();


	void waitRequest();

	void displayRequest();

	void sendResponse(std::string str);

	int getPort() const;
	int getSocketFd() const;

	std::vector<std::string> getRequest() const;
	int _newsocket;

private:
	Socket();
	void makeSocketNonBlocking();

	struct sockaddr_in _addr;
	void setStruct();

	std::vector<std::string> _request;

	int _port;
	int _sockfd;

	std::string _currentDirectory;

};

std::ostream &			operator<<( std::ostream & o, Socket const & i );

#endif /* ********************************************************** SOCKET_H */