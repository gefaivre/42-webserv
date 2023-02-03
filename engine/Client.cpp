#include "Client.hpp"
#include <algorithm>
#include <vector>
#include <iostream>
/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Client::Client()
{
	_createR = NULL;
}

Client::Client(Server *server, int clientfd) : _clientfd(clientfd), _server(server)
{
	this->_headerIsRead = false;
	this->_firstTimeBody = true;
	this->_bodyContentLenght = 0;
	this->_isSend = false;
	this->_moverSave = 0;
	this->_errorcode = 0;
	this->_cgiResponse.clear();
	this->_firstTimeCreate = 0;
	this->_createIsFinish = 0;
}

Client::Client(const Client &src) : _clientfd(src._clientfd), _server(src._server)
{
	this->_headerIsRead = false;
	this->_firstTimeBody = true;
	this->_bodyContentLenght = 0;
	this->_errorcode = 0;
	this->_isSend = false;
	this->_moverSave = 0;
	this->_cgiResponse.clear();
	this->_firstTimeCreate = 0;
	this->_createIsFinish = 0;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Client::~Client()
{
	// std::cout << "DESTRUCTEUR CLIENT" << std::endl;
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
		this->_moverSave = rhs._moverSave;
		this->_cgiResponse = rhs._cgiResponse;
		this->_errorcode = rhs._errorcode;
		this->_firstTimeCreate = rhs._firstTimeCreate;
		this->_createIsFinish = rhs._createIsFinish;
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
	if (atoi(_requestmap[std::string("Content-Length")].c_str()))
		return (atoi(_requestmap[std::string("Content-Length")].c_str()));
	return (atoi(_requestmap[std::string("content-length")].c_str()));
	
}
std::string Client::getHost()
{
	std::string host;
	host = _requestmap[std::string("Host")];
	host.erase(host.find(":"));
	return(host);
}

size_t Client::setBodyContentLenght()
{
	for(size_t i = 0; i < _request.size(); i++)
	{
		 if (_request[i].find("Content-Length:") != std::string::npos)
		 	return (std::atoi(_request[i].c_str()));
		if (i == (_request.size() - 1))
			return 0;
	}
	return 0;
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
			std::string value = _request[i].substr(colon + 2, _request[i].size());
			_requestmap.insert(std::pair<std::string, std::string>(key, value));
		}
	}
}

void Client::resetClient()
{
	_requestLine.clear();
	_request.clear();
	_requestmap.clear();
	_requestBody.clear();
	_response.clear();
	_cgiResponse.clear();
	_requestmapBody.clear();
	_headerIsRead = false;
	_firstTimeBody = false;
	_bodyContentLenght = 0;
	_isKeepAlive = false;
	_isSend = false;
	_moverSave = 0;
	_errorcode = 0;
	//delete new create response
	if (_createR != NULL)
	{
		delete _createR;
	}
	//apeler dans le delete dans le server
}

void Client::EndOfRead()
{
	struct epoll_event event;
	event.events = EPOLLOUT | EPOLLRDHUP;
	event.data.fd = _clientfd;
	epoll_ctl(_server->getEpollFd(), EPOLL_CTL_MOD, _clientfd, &event);

	
	// _requestmap.clear();
}

bool Client::parseChunked()
{
		for (size_t i = 0; i < _request.size(); i++)
		{
			if (_request[i].find("transfer-encoding: chunked") != std::string::npos)
			{
				return (true);
			}
	}
	return (false);
}

std::string Client::chunkedBody()
{
	for (size_t i = 0; i< _request.size(); i++)
	{
		if (_request[i].find("transfer-encoding: chunked") != std::string::npos)
			_request.erase(_request.begin() + i);
	}
	std::string str;
	std::vector<std::string> vector;
	vector = ft_split_chunked_request(_requestBody);
	for (size_t i = 0; i< vector.size();i++)
	{
		if (vector[i].find("Content-Disposition") != std::string::npos)
		{
			if (vector[i + 1].empty() && (i + 2) < vector.size())
				vector.erase(vector.begin() + i + 2);
		}
		//TODO: verifier si vector[i + 1] est un num sinon error
		vector[i].find_last_of("Content-Disposition");
	}
	size_t i = vector.size() - 1;
	while (vector[i].find(ft_find_boundary_utils(_requestmap)) == std::string::npos)
	{
		vector.erase(vector.begin() + i);
		i--;
	}
	vector.erase(vector.begin());
	for (size_t i = 0; i< vector.size(); i++)
	{
		str += vector[i];
		str += "\r\n";
	}
	return (str);
}

int Client::readRequestHeader()
{
	char buf[READING_BUFFER];
	int sizeRead;

	bzero(buf, READING_BUFFER);

	sizeRead = recv(_clientfd, buf, READING_BUFFER - 1, 0);
	if (sizeRead == -1)
		return ERROR;
	if (sizeRead == 0)
		return READ;
	_requestLine += std::string(buf);
	while (_requestLine.find("\r\n") != std::string::npos)
	{
		std::string line = _requestLine.substr(0, _requestLine.find("\r\n"));
		_requestLine.erase(0, _requestLine.find("\r\n") + 2);
		_request.push_back(line);
		if (line.size() == 0)
			break;
	}
	if (_request.size() != 0 && (_request[_request.size() - 1].size() == 0))
	{
		_requestBody += _requestLine;
		transformRequestVectorToMap();
		_bodyContentLenght = findBodyContentLenght();
		if ((_bodyContentLenght == 0 || _requestBody.size() == _bodyContentLenght) && parseChunked() == false)
			{
				Location loc;
				loc = _server->getLocationByPath('/' + getRequestFile(_request[0], NULL));
				if ((_bodyContentLenght > (size_t)loc.getClientMaxBodySize()) && (loc.getClientMaxBodySize() != 0))
					_errorcode = 413;
				EndOfRead();
			}
			return READ;
	}
	return NOT_READ_YET;
}

void Client::readRequestBody()
{
	char buf[READING_BUFFER];
	int sizeRead;

	bzero(buf, READING_BUFFER);
	std::cout << "READ REQUEST BODY" << std::endl;
	if ((sizeRead = recv(_clientfd, buf, READING_BUFFER - 1, 0)) != 0)
	{
		if (sizeRead == -1)
			std::cout << "Error recv" << std::endl;
		_requestBody.insert(_requestBody.size(), buf, sizeRead);
	}
	if (_requestBody.size() == _bodyContentLenght || _requestBody.find(ft_find_boundary_utils(_requestmap) + "\r\n" + '0') !=  std::string::npos)
	{
		Location loc;
		if (parseChunked() == true)
			_requestBody = chunkedBody();

		loc = _server->getLocationByPath('/' + getRequestFile(_request[0], NULL));
		if ((_bodyContentLenght > (size_t)loc.getClientMaxBodySize()) && (loc.getClientMaxBodySize() != 0))
				_errorcode = 413;
		EndOfRead();
	}
}

void Client::readRequest1()
{
	std::cout << "readRequest1 = "  <<std::endl;
	if (_headerIsRead == false)
	{
		if (readRequestHeader() == READ)
		{
			_headerIsRead = true;
			transformRequestVectorToMap();
		}
		std::cout << "LENGTH = " <<_bodyContentLenght <<std::endl;
	}
	else if (_bodyContentLenght || parseChunked() == true )
		readRequestBody();
}

int Client::CreateAndSendResponse()
{
	int ret;
	if(_firstTimeCreate == false)
	{
		Server *server;
		server = _server->getServerByName(getHost());
		if (server != NULL)
			_server = server;
		setKeepAlive();
		CGI cgi = CGI(&_request, _server, &_errorcode, &_requestBody, &_requestmap, &_cgiResponse);
		if (!_errorcode)
			cgi.verifyCgi();
		ParsingRequest parsingRequest(_request, _server, _cgiResponse, _errorcode);
		CreateResponse *CR = new CreateResponse(_server, _requestmap, parsingRequest.getData());
		std::map<std::string,std::string>::iterator it;

		// for (it = _requestmap.begin(); it != _requestmap.end(); it++)
		// {
		// 	std::cout << "first = " << it->first << "second = " << it->second << std::endl;
		// }
		_createR = CR;

		_firstTimeCreate = true;
		return NOT_READ_YET;
	}
	else if(_createIsFinish == false)
	{
		ret = _createR->create();
		if (ret == READ)
		{
			_response = _createR->getResponse();
			_createIsFinish = true;
		}
		return NOT_READ_YET;
	}
	else
	{
		ret = sendResponse();
		if (ret != NOT_READ_YET)
		{
			displayRequest();
			_createR->displayHeaderResponse();
			// _createR->displayFullResponse();
			_response = _createR->getResponse();
			if (ret == DELETE_CLIENT)
				return DELETE_CLIENT;
			else if (ret == READ)
			{
				resetClient();
				return READ;
			}
		}
		else
			return NOT_READ_YET;
	}
	return ERROR;
}


int Client::sendResponse()
{
	// std::cout << "SEND RESPONSE" << std::endl;

	struct epoll_event event;
	int ret;
	size_t mover = _moverSave * SENDING_BUFFER;
	size_t responseSize = _response.size() - mover;


	if (_isSend == false)
	{
		int max_size = responseSize > SENDING_BUFFER ? SENDING_BUFFER : responseSize;

		if ((ret = send(_clientfd, &_response[mover], max_size, 0)) == -1)
			ft_define_error("Send error");

		if (ret < SENDING_BUFFER || mover > _response.size())
			_isSend = true;
		else
			_moverSave++;
	}
	else
	{
		if (_isKeepAlive)
		{
			event.events =  EPOLLIN | EPOLLRDHUP;
			event.data.fd = _clientfd;
			epoll_ctl(_server->getEpollFd(), EPOLL_CTL_MOD, _clientfd, &event);
			return READ;
		}
		else
			return DELETE_CLIENT;
	}

	return NOT_READ_YET;
}









void Client::displayRequest()
{
	std::cout << "\n"
			  << YEL << _request[0] << "\033[0m" << std::endl;
}

void Client::displayFullRequest()
{
	std::cout << std::endl;
	for (size_t i = 0; i < _request.size(); i++)
		std::cout << YEL << _request[i] << "\033[0m" << std::endl;
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