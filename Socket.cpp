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

void Socket::waitAndParseRequest()
{


	socklen_t addrlen = sizeof(_addr);

	_newsocket = accept(_sockfd, (struct sockaddr *)&_addr, &addrlen);


	char a[1] = {0};
	std::string buf;
	_request.clear();

	while (recv( _newsocket , a, 1, 0) != 0)
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

void Socket::parsingRequest()
{
	for(unsigned long int i = 0; i < /* _request.size() */ 1 ; i++)
	{
		if (_request.size() > 0 && i == 0)
		{
			_requestData.methode = _request[i].substr(0, _request[i].find(" ", 0));
			unsigned int first = _requestData.methode.size() + 1;
			unsigned int last = _request[i].find(" ", first);
			_requestData.path = _request[i].substr(first, last - first);
			_requestData.protocolVersion = _request[i].substr(last + 1, _request[i].size());
		}
	}
	// std::cout << _requestData.methode << std::endl;
	// std::cout << _requestData.path << std::endl;
}

//	------------------------
//	CREATE AND SEND RESPONSE
//	------------------------

void Socket::createAndSendResponse()
{

	std::cout <<"-----------" << std::endl;
	std::cout << "Methode\t=\t" << _requestData.methode << std::endl;
	std::cout << "File\t=\t" << _requestData.path << std::endl;
	std::cout << "ProtoV\t=\t" << _requestData.protocolVersion << std::endl;
	std::cout <<"-----------" << std::endl;

	fillHeaderData();

	if (_requestData.methode == "GET")
	{
		if (_requestData.path == "/")
		{
			std::ifstream fileToSend("index.html");
			sendResponse(fileToSend);
		}
		else
		{
			std::ifstream fileToSend(_requestData.path.c_str());
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

void Socket::fillHeaderData()
{
	_headerData.protocolVersion = _requestData.protocolVersion;
	if (stat(_requestData.path) == 0)
	{
		_headerData.statusCode = 200;
		_headerData.statusMessage = "OK";

	}
	_headerData.contentLenght = ;
	_headerData.contentType;

}


void Socket::sendResponse(std::ifstream &fileToSend)
{

	std::string str;
	std::ostringstream ss;

	ss << fileToSend.rdbuf();
	str = ss.str();
	fileToSend.close();

	send(_newsocket , str.c_str() , strlen(str.c_str()), 0);

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

/* ************************************************************************** */