#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/epoll.h>
#include "utils.hpp"
#include "../includes/Server1.hpp"

char **ft_split(const char *str);

class Client;

class Server
{

public:

	Server(int port, Server1 *Mserv);
	~Server();

	int getPort() const;
	int getServerFd() const;

	std::map<int, Client *> clients;
	void newclient(int epoll_fd);

	int _newsocket;

	Server1 *Mserv;




private:
	Server();

	struct sockaddr_in _addr;
	void setStruct();

	int _port;
	int _sockfd;

	std::string _currentDirectory;
};

std::ostream &			operator<<( std::ostream & o, Server const & i );

#endif /* ********************************************************** SERVER_H */