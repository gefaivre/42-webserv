#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/epoll.h>
#include "utils.hpp"
#include "../includes/Server.hpp"

char **ft_split(const char *str);

class Client;

class OldServer
{

public:

	OldServer(int port, Server *Mserv);
	~OldServer();

	int getPort() const;
	int getServerFd() const;

	std::map<int, Client *> clients;
	void newclient(int epoll_fd);

	int _newsocket;

	// Server *Mserv;

private:
	OldServer();

	struct sockaddr_in _addr;
	void setStruct();

	int _port;
	int _sockfd;
};

std::ostream &			operator<<( std::ostream & o, Server const & i );

#endif /* ********************************************************** SERVER_H */