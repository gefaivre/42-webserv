#include "Socket.hpp"

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
    {
        std::cout << "Socket error" << std::endl;
        std::cout << errno << std::endl;
    }
	int yes = 1;
	if (setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR,
                        (void*)&yes, sizeof(yes)) < 0)
    {
        std::cout << "Setsockopt error" << std::endl;
        std::cout << errno << std::endl;
    }

	if( (bind(_sockfd, (struct sockaddr*)&_addr, sizeof(_addr))) == -1)
    {
        std::cout << "Bind error" << std::endl;
        std::cout << errno << std::endl;
    }

	if (listen(_sockfd, SOMAXCONN) == -1)
    {
        std::cout << "Listen error" << std::endl;
        std::cout << errno << std::endl;
    }

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


//	----------------------
//	WAIT AND PARSE REQUEST
//	----------------------

void Socket::waitRequest()
{
	socklen_t addrlen = sizeof(_addr);

	_newsocket = accept(_sockfd, (struct sockaddr *)&_addr, &addrlen);
	if (_newsocket == -1) //handle errors (This call returns a non-negative descriptor on success, otherwise it returns -1 on error)
	{
        std::cout << "Socket error with accept function" << std::endl;
		exit(errno);
    }
	char a[1] = {0};
	std::string buf;
	_request.clear();
	while (recv(_newsocket, a, 1, 0) != -1) //add -1 to handle errors (This call returns the number of bytes read into the buffer, otherwise it will return -1 on error.)
	{
		buf += a[0];
		if (a[0] == '\n' )
		{
			buf.erase(buf.find("\r\n") ,buf.size());
			_request.push_back(buf);
			buf.clear();
			if (_request[_request.size() - 1].size() == 0 )
				break;
		}
	}

}


//	-------
//	SEND
//	-------


void Socket::sendResponse(std::string str)
{
	send(_newsocket, str.c_str(), str.size(), 0);

	close(_newsocket);
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

std::vector<std::string> Socket::getRequest() const
{
	return (_request);
}

/* ************************************************************************** */