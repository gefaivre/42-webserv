#include "Client.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Client::Client()
{
}

Client::Client(Server *server, int clientfd) : _clientfd(clientfd), _server(server)
{
	this->_headerIsRead = false;
	this->_firstTimeBody = true;
	this->_bodyContentLenght = 0;
	this->_isSend = false;
	this->_headerIsSend = false;
}

Client::Client(const Client &src) : _clientfd(src._clientfd), _server(src._server)
{
	this->_headerIsRead = false;
	this->_firstTimeBody = true;
	this->_bodyContentLenght = 0;
	this->_isSend = false;
	this->_headerIsSend = false;

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

Client &Client::operator=(Client const &rhs)
{
	if (this != &rhs)
	{
		this->_requestLine = rhs._requestLine;
		this->_request = rhs._request;
		this->_requestmap = rhs._requestmap;
		this->_requestBody = rhs._requestBody;

		this->_clientfd = rhs._clientfd;
		this->_server = rhs._server;
		this->_headerIsRead = rhs._headerIsRead;
		this->_firstTimeBody = rhs._firstTimeBody;
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
	event.events = EPOLLOUT | EPOLLRDHUP;
	event.data.fd = _clientfd;
	epoll_ctl(_server->getEpollFd(), EPOLL_CTL_MOD, _clientfd, &event);

	setKeepAlive();
	displayRequest();
	createResponse();

	
}

int Client::readRequest1()
{

	std::cout << "READ REQUEST CLIENT FD = "  << _clientfd << std::endl;
	char buf[READING_BUFFER];
	int sizeRead;

	bzero(buf, READING_BUFFER);

	if (_headerIsRead == false)
	{
		sizeRead = recv(_clientfd, buf, READING_BUFFER, 0);
		if (sizeRead == -1)
			std::cout << "Error recv" << std::endl;
		if (sizeRead == 0)
			return (0);
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
			if (_bodyContentLenght == 0)
			{
				EndOfRead();
			}
			_headerIsRead = true;
		}
	}
	else if (_bodyContentLenght)
	{
		if ((sizeRead = recv(_clientfd, buf, READING_BUFFER, 0)) != 0)
		{
			if (sizeRead == -1)
				std::cout << "Error recv" << std::endl;
			_requestBody.insert(_requestBody.size(), buf, sizeRead);
			std::cout << buf << std::endl;
		}
		if (_requestBody.size() == _bodyContentLenght)
		{
			EndOfRead();
			std::cout << "EOR "  << _clientfd << std::endl;
		}
	}
	return (1);
}

void Client::createResponse()
{
	if (_firstTimeBody == true)
	{
		ParsingRequest parsingRequest(_request, _server);
		_createResponse = new CreateResponse(_server, _requestmap, parsingRequest.getData());
		_createResponse->displayHeaderResponse();
		// createResponse.displayFullResponse();
		_headerResponse = _createResponse->getHeaderResponse();
		_createResponse->displayHeaderResponse();
	}
	else
	{
		_headerResponse = _createResponse->getBodyResponse();
	}
}

void Client::resetClient()
{
	std::cout << RED << "RESET CLIENT" << reset << std::endl;

	_requestLine.clear();
	_request.clear();
	_requestmap.clear();
	_requestBody.clear();
	_headerIsRead = false;
	_firstTimeBody = false;
	_bodyContentLenght = 0;
	_isKeepAlive = false;
	_isSend = false;
	_headerIsSend = false;

}

int Client::sendResponse()
{
	struct epoll_event event;
	int ret;

	if (_isSend == false)
	{
		if (_headerIsSend == false)
		{
			int max_size = _headerResponse.size() > SENDING_BUFFER ? SENDING_BUFFER : _headerResponse.size();

			if ((ret = send(_clientfd, _headerResponse.data(), max_size, 0)) == -1)
				ft_define_error("Send error");
			else
				_headerResponse = _headerResponse.substr(ret);

			if (_headerResponse.size() == 0)
				_headerIsSend = true;
		}
		else
		{
			
		}
	}
	else
	{
		if (this->_isKeepAlive)
		{
			resetClient();
			event.events =  EPOLLIN | EPOLLRDHUP;
			event.data.fd = _clientfd;
			epoll_ctl(_server->getEpollFd(), EPOLL_CTL_MOD, _clientfd, &event);
		}
		else
		{
			return 0;
		}
	}
	return 1;
}


void Client::displayRequest()
{
	std::cout << "\n"
			  << "\033[33m" << _request[0] << "\033[0m" << std::endl;
}

void Client::displayFullRequest()
{
	std::cout << std::endl;
	for (size_t i = 0; i < _request.size(); i++)
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