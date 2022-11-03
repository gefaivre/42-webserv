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




void Socket::waitAndCopyRequest()
{


	socklen_t addrlen = sizeof(_addr);

	_newsocket = accept(_sockfd, (struct sockaddr *)&_addr, &addrlen);


	char a[1] = {0};
	std::string buf;
	_request.clear();

	while (read( _newsocket , a, 1) != 0)
	{
		if (a[0] != '\n')
			buf += a[0];
		else
		{
			_request.push_back(buf);
			buf.clear();
			if (_request.size() > 1 && _request[_request.size() - 1].find("\r") == 0 )
				break;
		}
	}
	parsingRequest();
}

void Socket::createAndSendResponse()
{

	std::cout <<"-----------" << std::endl;
	std::cout << "Methode\t=\t" << _parsing.methode << std::endl;
	std::cout << "File\t=\t" << _parsing.file << std::endl;
	std::cout <<"-----------" << std::endl;

	if (_parsing.methode == "GET")
	{
		if (_parsing.file == "/")
		{
			std::ifstream fileToSend("index.html");
			sendResponse(fileToSend);
		}
		else
		{
			std::ifstream fileToSend(_parsing.file.c_str());
			if (fileToSend.is_open())
			{
				sendResponse(fileToSend);
			}
			else
			{
				std::ifstream fileToSend("404.html");
				sendResponse(fileToSend);
			}


		}
	}

}

void Socket::sendResponse(std::ifstream &fileToSend)
{

	std::string str;
	if(fileToSend.is_open())
	{
		std::ostringstream ss;
		ss << fileToSend.rdbuf(); // reading data
		str = ss.str();
		fileToSend.close();
	}

	write(_newsocket , str.c_str() , strlen(str.c_str()));

	close(_newsocket);
}

void Socket::displayRequest()
{
	for(long unsigned int i = 0; i < _request.size(); i++)
		std::cout << _request[i] << std::endl;
}


void Socket::parsingRequest()
{
	for(unsigned long int i = 0; i < /* _request.size() */ 1 ; i++)
	{
		if (_request.size() > 0 && _request[i].compare(0, 3, "GET") == 0)
		{
			_parsing.methode = "GET";
			unsigned int first = 4;
			unsigned int last = _request[i].find(" ", 5);
			_parsing.file = _request[i].substr(first, last - first);
		}
	}
	// std::cout << _parsing.methode << std::endl;
	// std::cout << _parsing.file << std::endl;
}




/*
** --------------------------------- ACCESSOR ---------------------------------
*/

int Socket::getPort() const
{
	return (_port);
}

/* ************************************************************************** */