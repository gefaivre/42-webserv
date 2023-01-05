#include "Socket.hpp"
#include "webserv.h"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Socket::Socket()
{}

Socket::Socket(int port, int domain, int type, int protocol):
	_port(port)
{
	setStruct();

	_sockfd = socket(domain, type, protocol); //AF_INET,SOCK_STREAM, 0
	if (_sockfd == -1)
		ft_define_error("Socket error");
	int yes = 1;
	if (setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR,
		(void*)&yes, sizeof(yes)) == -1)
		ft_define_error("Setsockopt error");
	if( (bind(_sockfd, (struct sockaddr*)&_addr, sizeof(_addr))) == -1)
		ft_define_error("Bind error");
	makeSocketNonBlocking();
	if (listen(_sockfd, SOMAXCONN) == -1)
		ft_define_error("Listen error");
}

// Socket::Socket( const Socket & src )
// {
// }


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Socket::~Socket()
{
	close(_sockfd);
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

// Socket &				Socket::operator=( Socket const & rhs )
// {
	// if ( this != &rhs )
	// {	
	// 	this->_value = rhs.getValue();
	// }
// 	return *this;
// }

std::ostream &			operator<<( std::ostream & o, Socket const & i )
{
	o << "Value = " << i.getPort();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

void Socket::setStruct()
{
	memset((char *)&_addr, 0, sizeof(_addr));
	_addr.sin_addr.s_addr = INADDR_ANY;
	_addr.sin_port = htons(_port); 
	_addr.sin_family = AF_INET; 
}

void Socket::makeSocketNonBlocking()
{
  int flags;

  	flags = fcntl (_sockfd, F_GETFL, 0);
  	if (flags == -1)
      perror ("fcntl");

  	flags |= O_NONBLOCK;
	if(fcntl (_sockfd, F_SETFL, flags) == -1)
      perror ("fcntl");

}


//	----------------------
//	WAIT AND PARSE REQUEST
//	----------------------

void	ft_continue_reading(char a, std::string buf)
{
	std::cout << "--- continue reading ---" << std::endl;
	buf += a;
	std::cout << "." << a << ".";
	std::cout << buf;
}


void Socket::parseHeader(std::string buf, char a, ssize_t tmp_recv)
{
	_request = ft_split_header(buf);
	for (size_t i = 0; i < _request.size(); i++)
	{
		size_t colon = _request[i].find(": ");
		if (colon != std::string::npos)
		{
			std::string key = _request[i].substr(0, colon);
			std::string value = _request[i].substr(colon + 2, _request.size());
			_requestmap.insert(std::pair<std::string, std::string>(key, value));
		}
	}
	std::map<std::string,std::string>::iterator it;
	it = _requestmap.find("Content-Length");
	if (it != _requestmap.end())
	{
		int content_lenght = std::atoi(it->second.c_str());

		while ((tmp_recv = recv(_newsocket, &a, 1, 0)) && content_lenght--) //add -1 to handle errors (This call returns the number of bytes read into the buffer, otherwise it will return -1 on error.)
			std::cout << a;
	}
}

void Socket::waitRequest()
{
	socklen_t addrlen = sizeof(_addr);

	_newsocket = accept(_sockfd, (struct sockaddr *)&_addr, &addrlen);
	if (_newsocket == -1) //handle errors (This call returns a non-negative descriptor on success, otherwise it returns -1 on error)
		ft_define_error("Error the connection with the socket was not established");
	char a = 0;
	std::string buf;
	_request.clear();
	// size_t last_newline;
	ssize_t tmp_recv;
	while ((tmp_recv = recv(_newsocket, &a, 1, 0))) //add -1 to handle errors (This call returns the number of bytes read into the buffer, otherwise it will return -1 on error.)
	{
		if (tmp_recv == -1)
			ft_define_error("Error with the message from a socket");
		buf += a;
			if (buf.find("\n\r\n") != std::string::npos)
			{
				std::cout << "hello" << std::endl;
				break;
			}
	}
	parseHeader(buf, a, tmp_recv);
}


//	-------
//	SEND
//	-------


void Socket::sendResponse(std::string str)
{
	if (send(_newsocket, str.c_str(), str.size(), 0) == -1)
		ft_define_error("Send error");
	if (close(_newsocket) == -1)
		ft_define_error("Close error");
}


//
//	UTILS
//


void Socket::displayRequest()
{
	for(long unsigned int i = 0; i < _request.size(); i++)
		std::cout << _request[i] << std::endl;
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/

int Socket::getPort() const
{
	return (_port);
}

int Socket::getSocketFd() const
{
	return (_sockfd);
}


std::vector<std::string> Socket::getRequest() const
{
	return (_request);
}

/* ************************************************************************** */