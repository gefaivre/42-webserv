#include "Server.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Server::Server()
{}

Server::Server(int port, int domain, int type, int protocol):
	_port(port)
{
	setStruct();

	_sockfd = socket(domain, type, protocol); //AF_INET,SOCK_STREAM, 0
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


std::ostream &			operator<<( std::ostream & o, Server const & i )
{
	o << "Value = " << i.getPort();
	return o;
}


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

void Server::readRequest()
{
	char a[1] = {0};
	std::string buf;
	_request.clear();
	ssize_t tmp_recv;
	int size = -1;
	int tmp_switch = 0;
	bool method_get = false;
	while ((tmp_recv = recv(_newsocket, a, 1, 0))) //add -1 to handle errors (This call returns the number of bytes read into the buffer, otherwise it will return -1 on error.)
	{
		if (tmp_recv == -1)
			ft_define_error("Error with the message from a socket");
		buf += a[0];
		if (a[0] == '\n' || size == 0)
		{
			//find the size of the body
			const char *c = strstr(buf.c_str(), "Content-Length: ");
			if (c && size == -1)
			{
				char **len_split = ft_split(c);
				size = atoi(len_split[1]);
			}
			//find the method
			if (method_get == false && strstr(buf.c_str(), "GET"))
				method_get = true;
			if (a[0] == '\n')
				buf.erase(buf.find("\r\n") ,buf.size());
			_request.push_back(buf);
			buf.clear();
			if (_request[_request.size() - 1].size() == 0 || size == 0)
			{
				if (tmp_switch == 1 || method_get)
				{
					tmp_switch = 0;
					break;
				}
				tmp_switch = 1;
			}
		}
		if (size > 0 && tmp_switch == 1)
			size--;
	}
}


//	-------
//	SEND
//	-------


void Server::sendResponse(std::string str)
{
	if (send(_newsocket, str.c_str(), str.size(), 0) == -1)
		ft_define_error("Send error");
	if (close(_newsocket) == -1)
		ft_define_error("Close error");
}


//
//	UTILS
//


void Server::displayRequest()
{
	std::cout << "Request : "<< std::endl;
	std::cout << "\033[33m" << _request[0] << "\033[0m" << std::endl;
}


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


std::vector<std::string> Server::getRequest() const
{
	return (_request);
}

/* ************************************************************************** */