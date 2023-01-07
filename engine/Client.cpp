#include "Client.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Client::Client()
{

}

Client::Client(Server *server, int clientfd):
    _clientfd(clientfd), _server(server)
{
	this->_headerIsRead = false;
	this->_firstTimeBody = true;
	this->_bodyContentLenght = 0;
}

Client::Client( const Client & src ) : _clientfd(src._clientfd), _server(src._server)
{
	this->_headerIsRead = false;
	this->_firstTimeBody = true;
	this->_bodyContentLenght = 0;
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Client::~Client()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Client &				Client::operator=( Client const & rhs )
{
	if ( this != &rhs )
	{
		this->_requestLine = rhs._requestLine;
		this->_request = rhs._request;
		this->_requestmap = rhs._requestmap;
		this->_requestBody = rhs._requestBody;
		this->_response = rhs._response;

		this->_clientfd = rhs._clientfd;
		this->_server = rhs._server;
		this->_headerIsRead = rhs._headerIsRead;
		this->_firstTimeBody = rhs._firstTimeBody ;
		this->_isKeepAlive = rhs._isKeepAlive;
	}
	return *this;
}

// std::ostream &			operator<<( std::ostream & o, Client const & i )
// {
// 	//o << "Value = " << i.getValue();
// 	return o;
// }


/*
** --------------------------------- METHODS ----------------------------------
*/

size_t Client::findBodyContentLenght()
{
	return (atoi(_requestmap[std::string("Content-Length")].c_str()));
}

void Client::setKeepAlive()
{
	if (_requestmap[std::string("Connection")] == std::string("keep-alive"))
		_isKeepAlive = true;
	else
		_isKeepAlive = false;
}

void Client::transformRequestVectorToMap()
{
	for (size_t i = 1; i < _request.size(); i++)
	{
		size_t colon = _request[i].find(": ");
		if (colon != std::string::npos)
		{
			std::string key = _request[i].substr(0, colon);
			std::string value = _request[i].substr(colon + 2, _request.size());
			_requestmap.insert(std::pair<std::string, std::string>(key, value));
		}
	}
}

void Client::EndOfRead()
{
	struct epoll_event event;

	event.events = EPOLLOUT;
	event.data.fd = _clientfd;
	epoll_ctl(_server->getEpollFd(), EPOLL_CTL_MOD, _clientfd, &event);
	setKeepAlive();

}

int Client::readRequest1()
{

	std::cout << "READ REQUEST" << std::endl;
	char buf[50];
	int sizeRead;
	

	bzero(buf, 50);

	if (_headerIsRead == false)
	{
		sizeRead = recv(_clientfd, buf, 49, 0);
		if (sizeRead == -1)
			std::cout << "Error recv" << std::endl;
		if (sizeRead == 0)
		{
			return (0);
		}
		_requestLine += std::string(buf);
		
		while (_requestLine.find("\r\n") != std::string::npos)
		{
			std::string line = _requestLine.substr(0, _requestLine.find("\r\n"));
			_requestLine.erase(0, _requestLine.find("\r\n") + 2);
			_request.push_back(line);
		}
		if (_request.size() != 0 && (_request[_request.size() - 1].size() == 0))
		{
			_requestBody += _requestLine;
			transformRequestVectorToMap();
			_bodyContentLenght = findBodyContentLenght();
			if(_bodyContentLenght == 0)
			{
				EndOfRead();
			}
			_headerIsRead = true;
		}
	}
	else if(_bodyContentLenght)
	{
		if ((sizeRead = recv(_clientfd, buf, 49, 0)) != 0)
		{
			if (sizeRead == -1)
				std::cout << "Error recv" << std::endl;
			_requestBody.insert(_requestBody.size(), buf, sizeRead);
			std::cout << buf << std::endl;
		}
		if (_requestBody.size() == _bodyContentLenght)
		{
			EndOfRead();
		}
	}
	return (1);
}

void Client::createResponse()
{
    ParsingRequest parsingRequest(_request, _server);
	CreateResponse createResponse(_server, parsingRequest.getData());
	createResponse.displayHeaderResponse();
	// createResponse.displayFullResponse();
    _response = createResponse.getResponse();
}

void Client::resetClient()
{
	std::cout << RED << "RESET CLIENT" << reset << std::endl;

	_requestLine.clear();
	_request.clear();
	_requestmap.clear();
	_requestBody.clear();
	_response.clear();
	_headerIsRead = false;
	_firstTimeBody = false;
	_bodyContentLenght = 0;
	_isKeepAlive = false;
}

void Client::sendResponse()
{
	struct epoll_event event;

	if (send(_clientfd, _response.c_str(), _response.size(), 0) == -1)
		ft_define_error("Send error");

	if (!this->_isKeepAlive)
	{
		epoll_ctl(_server->getEpollFd(), EPOLL_CTL_DEL, _clientfd, &event);
		_server->clients.erase(_server->clients.find(_clientfd));
		if (close(_clientfd) == -1)
			ft_define_error("Close error");
	}
	else
	{
		resetClient();

		event.events = EPOLLIN;
		event.data.fd = _clientfd;
		epoll_ctl(_server->getEpollFd(), EPOLL_CTL_MOD, _clientfd, &event);
	}
}

void Client::displayRequest()
{
	std::cout << "\n" << "\033[33m" << _request[0] << "\033[0m" << std::endl;
}

void Client::displayFullRequest()
{
	std::cout << std::endl;
	for(size_t i = 0; i < _request.size(); i++)
		std::cout << "\033[33m" << _request[i] << "\033[0m" << std::endl;
}

void Client::displayFullBody()
{
	std::cout << _requestBody << std::endl;
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/
std::vector<std::string> Client::getRequest() const
{
	return (_request);
}

/* ************************************************************************** */