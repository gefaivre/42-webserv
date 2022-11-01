#include "Socket.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Socket::Socket()
{
}


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


void Socket::acceptAndRead()
{
	int new_socket;
	socklen_t addrlen = sizeof(_addr);

	new_socket = accept(_sockfd, (struct sockaddr *)&_addr, &addrlen);

	// const size_t npos = -1;
	char a[1] = {0};
	std::string buf;

	while (read( new_socket , a, 1) != 0)
	{
		if (a[0] != '\n')
		{
			buf += a[0];
		}
		else
		{
			_request.push_back(buf);
			buf.clear();
			if (_request.size() > 1)
				std::cout << " _request.find = " <<  _request[_request.size() - 1].find("\r\n") << std::endl;
		}

		// MET LES \n DANS LA STRING OU COMPAR JUSTE LES \r


		// if (_request.size() > 1 && _request[_request.size() - 2].find("\r\n") != npos && _request[_request.size() - 1].find("\r\n") != npos)
		// {
		// 	std::cout << "############3_request.size()\t=\t" << _request.size() << std::endl;
		// 	break;
		// }

	// 	if (_request.size() > 1 && _request[_request.size() - 1].find("\r\n") == 0 )
	// 	{
	// 		std::cout << "############3_request.size()\t=\t" << _request.size() << std::endl;
	// 		break;
	// 	}
	}

	for(long unsigned int i = 0; i < _request.size(); i++)
    {
        std::cout << _request[i] << std::endl;
    }
	std::cout << std::endl;

	write(new_socket , "hello\n" , strlen("hello\n"));
	close(new_socket);
}



/*
** --------------------------------- ACCESSOR ---------------------------------
*/

int Socket::getPort() const
{
	return (_port);
}

/* ************************************************************************** */