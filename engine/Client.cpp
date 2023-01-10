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
			std::string value = _request[i].substr(colon + 2, _request[i].size());
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

int Client::readRequest()
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
	// displayFullBody();
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

std::string	Client::ft_find_boundary()
{
	std::cout << BRED <<  "--- BOUNDARY ---" << WHT << std::endl;
	std::string boundary;
	size_t pos_equal = 0;
	std::map<std::string,std::string>::iterator it;
	// std::map<std::string,std::string>::const_iterator it;
	// std::map<std::string,std::string>::const_iterator ite = _requestmap.end(); 
	// for (it = _requestmap.begin(); it != ite; ++it)
	// {
	// 	std::cout << "REQUEST = " << it->first << " ** " << it->second << std::endl;
	// }
		it = _requestmap.find("Content-Type");
		if (it != _requestmap.end())
		{
			pos_equal = it->second.find_last_of('=');
			boundary = it->second.substr(pos_equal + 1);
		}
	return (boundary);
}

std::string findBodyKey(std::vector<std::string> vector, size_t i)
{
	std::string key;
	if (vector[i].find("Content-Disposition") != std::string::npos)
	{
		size_t colon_equal = vector[i].find("=");
		size_t size_type = 0;
		if (colon_equal != std::string::npos)
		{
			for (int tmp = colon_equal + 2; vector[i][tmp] != '\"'; tmp++)
				size_type++;
			key = vector[i].substr((colon_equal + 2),size_type);
		}
	}
	return (key);
}
std::string findBodyValue(std::vector<std::string> vector, size_t i, std::string value, std::string key)
{
	size_t colon_r = vector[i].find("\r");
	if (colon_r != std::string::npos)
	{
		if (colon_r == 0)
			vector[i].clear();
	}
	size_t content_type = vector[i].find("Content-Type");
	if (content_type != std::string::npos)
		vector[i].clear();		
	value = vector[i];
	if (key == "file")
		value += "\n";
	return (value);
}

void Client::transformBodyStringtoMap()
{
	std::string value;
	std::string key;
    std::vector<std::string> vector;
	vector = ft_split_vector_string(_requestBody, '\n');
	std::string boundary = ft_find_boundary();
	for (size_t i = 1; i < vector.size(); i++)
	{
		size_t colon_boundary = vector[i].find(boundary);
		if (colon_boundary == std::string::npos)
		{
			if (vector[i].find("Content-Disposition") != std::string::npos)
				key = findBodyKey(vector, i);
			else
				value += findBodyValue(vector, i, value, key);
		}
		else
		{
			// std::cout << "key = " << key << "&& value = " << value << std::endl;
			_requestmapBody.insert(std::pair<std::string, std::string>(key, value));
			value.clear();
			key.clear();
		}
	}

}

std::string Client::verifyCgi()
{
	std::cout << "** verifyCgi **" << std::endl;

	std::map<std::string,std::string>::iterator it;
	std::string format;
	size_t pos_equal = 0;
	it = _requestmap.find("Referer");
	if (it != _requestmap.end())
	{
		pos_equal = it->second.find_last_of('.');
		format = it->second.substr(pos_equal + 1);
		std::cout << format << std::endl;
		std::cout << "server = " << _server->getCgiValue(format)<< std::endl;
	}
	return (format);
}

void Client::saveFile()
{
	std::cout << "Save file = " << std::endl;
	transformBodyStringtoMap();
	std::map<std::string,std::string>::iterator it_file;
	std::map<std::string,std::string>::iterator it_name;
	it_file = _requestmapBody.find("file");
	it_name = _requestmapBody.find("name");
	if (it_file != _requestmapBody.end())
	{
		std::cout <<YEL <<  "FILE = " << it_name->second<< WHT<< std::endl;
		//open a file in write mode
		ofstream outfile;
		std::string new_path = "new_files/";
		mkdir(new_path.c_str(), 0777);
		outfile.open(new_path.append(it_name->second).c_str());
		//write the string
		outfile << it_file->second.substr(2, it_file->second.size() - 3);
		outfile.close();
		_requestmapBody.clear();
	}
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/
std::vector<std::string> Client::getRequest() const
{
	return (_request);
}

/* ************************************************************************** */