#include "Client.hpp"
#include <algorithm>
#include <vector>
#include <iostream>
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
	this->_moverSave = 0;
	this->_errorcode = 0;
	this->_cgiResponse.clear();
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
		this->_moverSave = rhs._moverSave;
		this->_cgiResponse = rhs._cgiResponse;
		this->_errorcode = rhs._errorcode;
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
			std::cout << "Key = " << key << std::endl;
			std::string value = _request[i].substr(colon + 2, _request[i].size());
			_requestmap.insert(std::pair<std::string, std::string>(key, value));
		}
	}
}

void Client::EndOfRead()
{
	CGI cgi = CGI(&_request, _server, &_errorcode, &_requestBody, &_requestmap, &_cgiResponse);
	struct epoll_event event;
	event.events = EPOLLOUT | EPOLLRDHUP;
	event.data.fd = _clientfd;

	std::cout << "** EndOfRead **" << std::endl;

	// std::map<std::string,std::string>::const_iterator it_chunked = _requestmapBody.find("transfer-encoding");
	// std::cout << "Chunked = " << it_chunked->second << std::endl;
	// if (_requestmapBody.find("transfer-encoding:") != std::string::npos)
	// 	std::cout << "CHUNKED" << std::endl;
	epoll_ctl(_server->getEpollFd(), EPOLL_CTL_MOD, _clientfd, &event);
	setKeepAlive();
	displayRequest();
	if (!_errorcode)
		cgi.verifyCgi();
	createResponse();
	// _requestmap.clear();
}

bool Client::parseChunked()
{
		std::cout << RED <<"** parseChunked **"<<reset<<std::endl;
		for (size_t i = 0; i < _request.size(); i++)
		{
			if (_request[i].find("transfer-encoding: chunked") != std::string::npos)
			{
				std::cout << YEL << "CHUNKED" << reset <<std::endl;
				return (true);
			}
	}
	return (false);
}

std::string Client::chunkedBody()
{
	std::cout << "** Chunked Body **" << std::endl;
	for (size_t i = 0; i< _request.size(); i++)
	{
		if (_request[i].find("transfer-encoding: chunked") != std::string::npos)
			_request.erase(_request.begin() + i);
	}
	std::string str;
	std::vector<std::string> vector;
	vector = ft_split_chunked_request(_requestBody);
	// std::vector<std::string>::iterator it;
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
	std::cout << "BODYYYYYY == " << _requestBody << std::endl;
	for (size_t i = 0; i< vector.size(); i++)
	{
		str += vector[i];
		str += "\r\n";
	}
	return (str);
}

int Client::readRequest1()
{
	std::cout << "READ REQUEST CLIENT FD = "  << _clientfd << std::endl;
	char buf[READING_BUFFER];
	int sizeRead;
	size_t loc_boundary;
	bzero(buf, READING_BUFFER);
	Location loc;

	if (_headerIsRead == false)
	{
		sizeRead = recv(_clientfd, buf, READING_BUFFER - 1, 0);
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
			if (line == "")
				break;
		}
		if (_request.size() != 0 && (_request[_request.size() - 1].size() == 0))
		{
			_requestBody += _requestLine;
			transformRequestVectorToMap();
			_bodyContentLenght = findBodyContentLenght();
			if ((_bodyContentLenght == 0 || _requestBody.size() == _bodyContentLenght) && parseChunked() == false)
			{
				loc = _server->getLocationByPath('/' + getRequestFile(_request[0], NULL));
				if ((_bodyContentLenght > (size_t)loc.getClientMaxBodySize()) && (loc.getClientMaxBodySize() != 0))
					_errorcode = 413;
				EndOfRead();
			}
			_headerIsRead = true;
		}
	}
	else if (parseChunked() == true || _bodyContentLenght)
	{
		std::cout << "------- Body" << std::endl;
		if ((sizeRead = recv(_clientfd, buf, READING_BUFFER - 1, 0)) != 0)
		{
			if (sizeRead == -1) 
				std::cout << "Error recv" << std::endl;
			_requestBody.insert(_requestBody.size(), buf, sizeRead);
			// std::cout << "BDYYYYY= " << _requestBody <<std::endl;
		}
		// std::cout << "ft_find_boundary_utils"<< std::endl;
		// std::cout << "ft_find_boundary_utils.size()" <<ft_find_boundary_utils(_requestmap) + "\r\n" + '0' << std::endl;
		try 
		{
			ft_find_boundary_utils(_requestmap);
			loc_boundary = _requestBody.find(ft_find_boundary_utils(_requestmap) + "\r\n" + '0');
		}
		catch(std::exception e)
		{
			loc_boundary = std::string::npos;
		}
		if (_requestBody.size() == _bodyContentLenght || loc_boundary !=  std::string::npos)
		{
			if (parseChunked() == true)
				_requestBody = chunkedBody();
			// for (size_t i = 0; i < _request.size();i++)
			// 	std::cout << YEL <<_request[i] << reset <<std::endl;
			// std::cout << YEL <<_requestBody << reset <<std::endl;
			loc = _server->getLocationByPath('/' + getRequestFile(_request[0], NULL));
			if ((_bodyContentLenght > (size_t)loc.getClientMaxBodySize()) && (loc.getClientMaxBodySize() != 0))
					_errorcode = 413;
			std::cout << "LOC2" <<std::endl;
			std::cout << "LOC2" << loc.getClientMaxBodySize()<<std::endl;
			EndOfRead();
			std::cout << "EOR "  << _clientfd << std::endl;
		}
	}
	// for (size_t i = 0; i < _request.size(); i++)
	// 	std::cout << "**_request = ." << _request[i] << "." <<std::endl;
	// std::cout << "** Body request - " << _requestBody << std::endl;
	return (1);
}

void Client::createResponse()
{
	std::cout << "** CREATE RESPONSE **" << std::endl;
	ParsingRequest parsingRequest(_request, _server, _cgiResponse, _errorcode);
	CreateResponse createResponse(_server, _requestmap, parsingRequest.getData());
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
	_cgiResponse.clear();
	_requestmapBody.clear();
	_headerIsRead = false;
	_firstTimeBody = false;
	_bodyContentLenght = 0;
	_isKeepAlive = false;
	_isSend = false;
	_moverSave = 0;
	_errorcode = 0;
}

int Client::sendResponse()
{
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
		if (this->_isKeepAlive)
		{
			resetClient();
			event.events =  EPOLLIN | EPOLLRDHUP;
			event.data.fd = _clientfd;
			// std::cout << "1" << std::endl;
			epoll_ctl(_server->getEpollFd(), EPOLL_CTL_MOD, _clientfd, &event);
			// std::cout << "2" << std::endl;
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