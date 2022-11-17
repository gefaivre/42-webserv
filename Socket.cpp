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

	while (recv( _newsocket , a, 1, 0))
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
			_requestData.protocol = _request[i].substr(last + 1, _request[i].size());
		}
	}
}

//	------------------------
//	CREATE AND SEND RESPONSE
//	------------------------

void Socket::createAndSendResponse()
{
	if (_requestData.methode == "GET")
	{
		foundFileToSend();
		fillHeaderData();
		sendResponse();

	}
}

void Socket::foundFileToSend()
{
	// std::cout << "_requestData.path\t=\t" << _requestData.path << std::endl;
	if (_requestData.path == "/")
		_fileToSend = "index.html";
	else if (_requestData.path.find('.') == std::string::npos)
	{
		_fileToSend = _requestData.path + ".html";
	}
	else
		_fileToSend = _requestData.path;
	if (_fileToSend[0] == '/')
		_fileToSend.erase(0,1);

	struct stat buffer;
	if (stat(_fileToSend.c_str(), &buffer) == -1)
		_fileToSend = "404.html";
	
	// std::cout << "_fileToSend\t=\t" << _fileToSend << std::endl;
}

void Socket::fillFilesExtension()
{
	_switchFilesExtension.insert( std::pair<std::string, std::string>("html","text/html"));
	_switchFilesExtension.insert(std::pair<std::string, std::string>("ico","image/x-icon"));
	_switchFilesExtension.insert(std::pair<std::string, std::string>("default","text/html") );
}

void Socket::fillHeaderData()
{
	fillFilesExtension();
	_headerData.protocol = _requestData.protocol;
	if (_fileToSend == "404.html")
	{
		_headerData.statusCode = "404";
		_headerData.statusMessage = "Not Found";

	}
	else
	{
		_headerData.statusCode = "200";
		_headerData.statusMessage = "OK";
	}
	std::string type = _fileToSend.substr(_fileToSend.find('.') + 1, _fileToSend.size());
	_headerData.contentType = _switchFilesExtension[type];
	if (_headerData.contentType.size() == 0)
		_headerData.contentType = _switchFilesExtension["default"];

	std::string file;
	std::string line;
	std::ifstream myfile;

	myfile.open(_fileToSend.c_str(), std::ifstream::in);
	while ( std::getline(myfile,line) )
	{
		if (file.size() != 0)
			file += '\n';
		file += line;
	}
	myfile.close();

	std::stringstream oui;
	oui << file.size();
	oui >> _headerData.contentLength;
}

void Socket::sendResponse()
{

	std::string str;
	std::string file;
	std::string line;
	std::ifstream myfile;

	myfile.open(_fileToSend.c_str(), std::ifstream::in);
	while ( std::getline(myfile,line) )
	{
		if (file.size() != 0)
			file += '\n';
		file += line;
	}
	myfile.close();

	str += _headerData.protocol;
	str += " ";
	str += _headerData.statusCode;
	str += " ";
	str += _headerData.statusMessage + "\r\n";
	str += "Content-Length: " + _headerData.contentLength + "\r\n";
	str += "\r\n";
	str += file;

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

/* ************************************************************************** */