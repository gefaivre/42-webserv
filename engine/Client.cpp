#include "Client.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Client::Client(Server *server, int clientfd):
    _clientfd(clientfd), _server(server)
{
	this->_headerIsRead = false;
	this->_firstTimeBody = true;
	this->_bodyContentLenght = 0;
}

// Client::Client( const Client & src )
// {
// }


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Client::~Client()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

// Client &				Client::operator=( Client const & rhs )
// {
// 	//if ( this != &rhs )
// 	//{
// 		//this->_value = rhs.getValue();
// 	//}
// 	return *this;
// }

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

void Client::readRequest1()
{

	char buf[50];
	int sizeRead;
	struct epoll_event event;

	bzero(buf, 50);

	if (_headerIsRead == false)
	{
		sizeRead = recv(_clientfd, buf, 49, 0);
		if (sizeRead == -1)
			std::cout << "Error recv" << std::endl;
		_requestLine += std::string(buf);
		
		while (_requestLine.find("\r\n") != std::string::npos)
		{
			std::string line = _requestLine.substr(0, _requestLine.find("\r\n"));
			_requestLine.erase(0, _requestLine.find("\r\n") + 2);
			_request.push_back(line);
		}
		if ((_request[_request.size() - 1].size() == 0))
		{
			_requestBody += _requestLine;
			transformRequestVectorToMap();
			_bodyContentLenght = findBodyContentLenght();
			if(_bodyContentLenght == 0)
			{
				event.events = EPOLLOUT;
				event.data.fd = _clientfd;
				epoll_ctl(_server->getEpollFd(), EPOLL_CTL_MOD, _clientfd, &event);
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
		}
		if (_requestBody.size() == _bodyContentLenght)
		{
			event.events = EPOLLOUT;
			event.data.fd = _clientfd;
			epoll_ctl(_server->getEpollFd(), EPOLL_CTL_MOD, _clientfd, &event);
		}
	}
}

void Client::createResponse()
{
    ParsingRequest parsingRequest(_request, _server);
	CreateResponse createResponse("www/", true, parsingRequest.getData());
	createResponse.displayHeaderResponse();
    _response = createResponse.getResponse();
}

void Client::sendResponse()
{
	if (send(_clientfd, _response.c_str(), _response.size(), 0) == -1)
		ft_define_error("Send error");
	if (close(_clientfd) == -1)
		ft_define_error("Close error");
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