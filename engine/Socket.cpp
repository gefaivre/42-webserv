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
		ft_define_error("Socket error");
	int yes = 1;
	if (setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR,
		(void*)&yes, sizeof(yes)) == -1)
		ft_define_error("Setsockopt error");
	if( (bind(_sockfd, (struct sockaddr*)&_addr, sizeof(_addr))) == -1)
		ft_define_error("Bind error");
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


//	----------------------
//	WAIT AND PARSE REQUEST
//	----------------------

void Socket::waitRequest()
{
	socklen_t addrlen = sizeof(_addr);

	_newsocket = accept(_sockfd, (struct sockaddr *)&_addr, &addrlen);
	if (_newsocket == -1) //handle errors (This call returns a non-negative descriptor on success, otherwise it returns -1 on error)
		ft_define_error("Error the connection with the socket was not established");
	char a[1] = {0};
	std::string buf;
	_request.clear();
	ssize_t tmp_recv;
	int size = -1;
	int tmp_switch = 0;
	//2 pb : pas de \n  la fin et pour get, il ne faut pas de tmp_switch
	while ((tmp_recv = recv(_newsocket, a, 1, 0))) //add -1 to handle errors (This call returns the number of bytes read into the buffer, otherwise it will return -1 on error.)
	{
		// std::cout << "test4" << std::endl;
		// std::cout << "buf defo = " << buf << std::endl;
		if (tmp_recv == -1)
			ft_define_error("Error with the message from a socket");
		buf += a[0];
		// std::cout << "a[0] = ." << buf << "." << "tmp_switcj = " << tmp_switch<< std::endl;
		if (a[0] == '\n' || size == 0)
		{
			// std::cout << "buf = ." << buf << "."<< std::endl;
			const char *c = strstr(buf.c_str(), "Content-Length: ");

			// std::cout << "size = " << size << "---" << "buf = " << buf << std::endl;
			if (c && size == -1)
			{
				char **len_split = ft_split(c);
				size = atoi(len_split[1]);
				std::cout << c << "size = " << size<< std::endl;
				// std::cout << "size =" << size << std::endl;
			}
			// std::cout << "before = "<< buf << std::endl;
			if (a[0] == '\n')
				buf.erase(buf.find("\r\n") ,buf.size());
			// std::cout << "after = "<< buf << std::endl;
			// std::cout << "test1" << std::endl;
			// std::cout << "AVANT" << a[0] << "---" <<buf << std::endl;
			_request.push_back(buf);
			// std::cout << "APRES" << a[0] << "---" <<buf << std::endl;
			// std::cout << "test2" << std::endl;
			buf.clear();
			// std::cout << "test3" << std::endl;
			if (_request[_request.size() - 1].size() == 0 || size == 0)
			{
				// std::cout << "reuqes = ."<< _request[_request.size() - 1] << "."<< std::endl;
				std::cout << "*" << std::endl;
				if (tmp_switch == 1)
				{
					break;
				}
				tmp_switch = 1;
			}
		}
		// std::cout << "size = " << size << "tmp_switch = " << tmp_switch  << std::endl;
		if (size > 0 && tmp_switch == 1)
			size--;
		std::cout << a[0];
		// std::cout << "buf = ." << buf << "." << std::endl;
		// std::cout<< "a = ." << a[0]  << "."<< std::endl;
	}
	// for (int i = 0; _request[i] != "\0"; i++)
	// {

	// }
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

std::vector<std::string> Socket::getRequest() const
{
	return (_request);
}

/* ************************************************************************** */