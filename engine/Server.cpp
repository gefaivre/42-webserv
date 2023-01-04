#include "Server.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Server::Server()
{}

Server::Server(int port, Server1 *Mserv):
	Mserv(Mserv), _port(port)
{
	setStruct();

	_sockfd = socket(AF_INET, SOCK_STREAM, 0); //AF_INET,SOCK_STREAM, 0
	if (_sockfd == -1)
		ft_define_error("Server error");
	int yes = 1;
	if (setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR,
		(void*)&yes, sizeof(yes)) == -1)
		ft_define_error("Setsockopt error");
	if( (bind(_sockfd, (struct sockaddr*)&_addr, sizeof(_addr))) == -1)
		ft_define_error("Bind error");
	fcntl(_sockfd, F_SETFL, O_NONBLOCK);
	if (listen(_sockfd, SOMAXCONN) == -1)
		ft_define_error("Listen error");
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Server::~Server()
{
	close(_sockfd);
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/


// std::ostream &			operator<<( std::ostream & o, Server const & i )
// {
// 	o << "Value = " << i.getPort();
// 	return o;
// }


/*
** --------------------------------- METHODS ----------------------------------
*/

void Server::setStruct()
{
	memset((char *)&_addr, 0, sizeof(_addr));
	_addr.sin_addr.s_addr = INADDR_ANY;
	_addr.sin_port = htons(_port); 
	_addr.sin_family = AF_INET; 
}


void Server::newclient(int epoll_fd)
{
	int newsocket;
	struct epoll_event event;
	socklen_t addrlen = sizeof(_addr);

	newsocket = accept(this->getServerFd(), (struct sockaddr *)&_addr, &addrlen);
	if (newsocket == -1)
		ft_define_error("Error the connection with the socket was not established");
	event.events = EPOLLIN;
	event.data.fd = newsocket;
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, newsocket, &event);
	
}


//	----------------------
//	WAIT AND PARSE REQUEST
//	----------------------


//	-------
//	SEND
//	-------




//
//	UTILS
//



/*
** --------------------------------- ACCESSOR ---------------------------------
*/

int Server::getPort() const
{
	return (_port);
}

int Server::getServerFd() const
{
	return (_sockfd);
}



/* ************************************************************************** */