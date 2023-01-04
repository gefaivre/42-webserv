#ifndef SERVER_HPP
#define SERVER_HPP

#include "utils.hpp"
#include "Client.hpp"
#include <sys/epoll.h>
#include "general_includes.hpp"

char **ft_split(const char *str);

class Server
{

public:

	Server(int port,int domain = AF_INET, int type = SOCK_STREAM, int protocol = 0);
	~Server();

	void readRequest();

	void displayRequest();

	void sendResponse(std::string str);

	int getPort() const;
	int getServerFd() const;

	std::vector<std::string> getRequest() const;

	std::map<int, Client*> clients;
	void newclient(int epoll_fd);

	int _newsocket;

private:
	Server();

	struct sockaddr_in _addr;
	void setStruct();

	std::vector<std::string> _request;

	int _port;
	int _sockfd;

	std::string _currentDirectory;

};

std::ostream &			operator<<( std::ostream & o, Server const & i );

#endif /* ********************************************************** SOCKET_H */