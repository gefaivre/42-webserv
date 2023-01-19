#include "Client.hpp"
std::vector<std::string> setAuthorizedExtension(std::vector<std::string> ext);
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
	this->_getParams.clear();
	this->authorizedExtension = setAuthorizedExtension(this->authorizedExtension);
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
	this->_getParams.clear();
	this->authorizedExtension = setAuthorizedExtension(this->authorizedExtension);
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
		this->authorizedExtension = rhs.authorizedExtension;
		this->_getParams = rhs._getParams;
		this->_loc = rhs._loc;
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

std::vector<std::string> setAuthorizedExtension(std::vector<std::string> ext)
{
	ext.push_back("html");
	ext.push_back("css");
	ext.push_back("js");
	ext.push_back("jpg");
	ext.push_back("jpeg");
	ext.push_back("png");
	ext.push_back("gif");
	ext.push_back("mp3");
	ext.push_back("ogg");
	ext.push_back("mp4");
	ext.push_back("avi");
	ext.push_back("pdf");
	ext.push_back("txt");
	ext.push_back("ico");
	return (ext);
}

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
	struct epoll_event event;
	event.events = EPOLLOUT | EPOLLRDHUP;
	event.data.fd = _clientfd;

	std::cout << "BEFORE CHUNKED" << std::endl;

	// std::map<std::string,std::string>::const_iterator it_chunked = _requestmapBody.find("transfer-encoding");
	// std::cout << "Chunked = " << it_chunked->second << std::endl;
	// if (_requestmapBody.find("transfer-encoding:") != std::string::npos)
	// 	std::cout << "CHUNKED" << std::endl;
	epoll_ctl(_server->getEpollFd(), EPOLL_CTL_MOD, _clientfd, &event);
	setKeepAlive();
	displayRequest();
	verifyCgi();
	createResponse();
	// _requestmap.clear();
}

bool Client::parseChunked()
{
	// std::string bodyChunked;
	// int sizeRead;
	// std::string line;
		std::cout << RED <<"** parseChunked **"<<reset<<std::endl;
	// if (_headerIsRead == true)
	// {
		for (size_t i = 0; i < _request.size(); i++)
		{
			std::cout << "request == " << _request[i] << std::endl;
			if (_request[i].find("transfer-encoding: chunked") != std::string::npos)
			{
				std::cout << "chunked" << std::endl;
				return (true);
			}
				// _chunked = true;
		// }
		// do{

		// 	char buf[READING_BUFFER];
		// 	bzero(buf, READING_BUFFER);
		// 	sizeRead = recv(_clientfd, buf, READING_BUFFER - 1, 0);
		// 	line += std::string(buf);
		// }while(bodyChunked.find(ft_find_boundary() + '\n' + '0') != std::string::npos && sizeRead > 0);
		// std::cout << "line = " << line << std::endl;
		// std::cout << "exit" << std::endl;
	}
	return (false);
	// _chunked = false;
}

std::string Client::chunkedBody()
{
	for (size_t i = 0; i< _request.size(); i++)
	{
		if (_request[i].find("transfer-encoding: chunked") != std::string::npos)
			_request.erase(_request.begin() + i);
	}
	std::cout << "BODYYYYYY == " << _requestBody << std::endl;
	std::string str;
	std::vector<std::string> vector;
	vector = ft_split_chunked_request(_requestBody);
	// std::vector<std::string>::iterator it;
	for (size_t i = 0; i< vector.size();i++)
	{
		if (vector[i].find("Content-Disposition") != std::string::npos && !vector[i + 1].empty())
			vector.erase(vector.begin() + i + 1);
		//TODO: verifier si vector[i + 1] est un num sinon error
		vector[i].find_last_of("Content-Disposition");
	}
	size_t i = vector.size() - 1;
	while (vector[i].find(ft_find_boundary()) == std::string::npos)
	{
		vector.erase(vector.begin() + i);
		i--;
	}
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
	bzero(buf, READING_BUFFER);

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
				std::cout << "_bodyContentLenght" << _bodyContentLenght <<std::endl;
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
		if (_requestBody.size() == _bodyContentLenght || _requestBody.find(ft_find_boundary() + "\r\n" + '0') !=  std::string::npos)
		{
			_requestBody = chunkedBody();
			for (size_t i = 0; i < _request.size();i++)
				std::cout << YEL <<_request[i] << reset <<std::endl;
			std::cout << YEL <<_requestBody << reset <<std::endl;
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
	// _loc.clear();
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


std::string	Client::ft_find_boundary()
{
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
	if (it == _requestmap.end())
		it = _requestmap.find("content-type");
	if (it != _requestmap.end())
	{
		pos_equal = it->second.find_last_of('=');
		if (pos_equal == std::string::npos)
			throw std::exception();
		boundary = it->second.substr(pos_equal + 1);
	}
	// std::cout << "Bound = ." << boundary + '\n' + '0'<<"." << std::endl;
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


std::string	ft_remove_not_print(std::string str)
{
	std::string::iterator it;
	std::string str2;
	for (it = str.begin(); it != str.end(); it++)
	{
		if (isprint(*it))
			str2 += *it;
	}
	std::cout << "str2 = ." << str2 << "."<< std::endl;
	return (str2);
}

void Client::transformBodyStringtoMap()
{
	std::string value;
	std::string key;
    std::vector<std::string> vector;
	// std::string boundary;
	try
	{
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
				{
					value += findBodyValue(vector, i, value, key);
					if (key != "file")
						value = ft_remove_not_print(value);
				}
			}
			else
			{
				_requestmapBody.insert(std::pair<std::string, std::string>(key, value));
				value.clear();
				key.clear();
			}
		}
	}
	catch (std::exception &e)
	{
		vector = ft_split_vector_string(_requestBody, '&');
		for (size_t i = 0; i < vector.size(); i++)
		{
			size_t colon_equal = vector[i].find('=');
			_requestmapBody.insert(std::pair<std::string, std::string>(vector[i].substr(0, colon_equal), vector[i].substr(colon_equal + 1)));
		}
		// std::map<std::string,std::string>::const_iterator it;
		// std::map<std::string,std::string>::const_iterator ite = _requestmapBody.end(); 
		// for (it = _requestmapBody.begin(); it != ite; ++it)
		// {
		// 	std::cout << "REQUEST = " << it->first << " ** " << it->second << std::endl;
		// }
		return ;
	}
}

std::string Client::findValueEnvCgi(std::string key)
{
	std::map<std::string,std::string>::iterator it;
	it = _requestmap.find(key);
	if (it != _requestmap.end())
		return (it->second);
	return (NULL);
}


std::string Client::findMethod()
{
	size_t pos_space = 0;
	size_t postIndex = _request[0].find("POST");
	if (postIndex != std::string::npos)
	{
		pos_space = _request[0].find_first_of(' ');
		return (_request[0].substr(0, pos_space));
	}
	return (NULL);
}

int Client::workPostCgi(std::string format, std::string requestFile)
{
	std::cout << "_requestBody\t=\t" << _requestBody << std::endl;
	pid_t pid;
    int fd[2];
    int fd_out[2];
	char buf[1024];
	std::string content_type;
	std::string query_string = "QUERY_STRING=";
	std::string request_method = "REQUEST_METHOD=POST";
	std::string content_length = "CONTENT_LENGTH=";
	std::string requestFileRoot = _loc.getRoot().append(requestFile);
	std::string script_filename = "SCRIPT_FILENAME=";
	std::string envCGI = findValueEnvCgi("Content-Length");	
	if (envCGI.empty())
		envCGI = findValueEnvCgi("content-length");
	script_filename.append(requestFileRoot);
	try 
	{
		std::cout << "Found boundary" << std::endl;
		ft_find_boundary();
	}
	catch(std::exception e)
	{
		content_type = "CONTENT_TYPE=application/x-www-form-urlencoded";
	}
	if (content_type.empty())
	{
		content_type = "CONTENT_TYPE=multipart/form-data; boundary=";
		content_type.append(ft_find_boundary());
	}
	// char *args[]= {const_cast<char*>(format.c_str()), const_cast<char*>(requestFileRoot.c_str()), NULL};	
	char *args[]= {const_cast<char*>(format.c_str()), (char *) "-f", const_cast<char*>(requestFileRoot.c_str()), NULL};	
	char *header[] = {
	const_cast<char*> (script_filename.c_str()),
	const_cast<char*> (request_method.c_str()),
	const_cast<char*>(content_type.c_str()),
	// const_cast<char*>(content_length.append(envCGI).c_str()),
	const_cast<char*> (query_string.append(_getParams).c_str()),
	(char *) "REDIRECT_STATUS=200",
	(char *) NULL
	};
   	pipe(fd);
	pipe(fd_out);
	write(fd[1], _requestBody.c_str(), _requestBody.length());
	close(fd[1]);

	if ((pid = fork()) < 0)
	{
		perror("fork");
		return EXIT_FAILURE;
	}
	else if (!pid) 
	{ 
		/* child */
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		dup2(fd_out[1], STDOUT_FILENO);
		dup2(fd_out[1], STDERR_FILENO);
		close(fd_out[0]);
		execve(args[0], args, header);
		perror("exec");
		return EXIT_FAILURE;
	} 
	else 
	{ 
		/* parent */
		close(fd[0]);
		close(fd_out[1]);
		while (waitpid(-1, NULL, WUNTRACED) != -1)
			;
		std::cout << "Before read" << std::endl;
		int n = read(fd_out[0], buf, 1024);
		if (n < 0)
		{
			std::cout << "Error read" << std::endl;
			perror("read");
           	exit(EXIT_FAILURE);
        } else
		{
            buf[n] = '\0';
			std::string mystring = buf;
			std::string substring = "Content-type";
			if (mystring.find(substring) != std::string::npos && mystring.find_first_of("\n\n") != std::string::npos)
				mystring.erase(mystring.find(substring), mystring.find_first_of("\n\n"));
			_cgiResponse.append(mystring);
			std::cout << "_cgiResponse" << _cgiResponse << std::endl;
		}
	}
	return (1);
}

int Client::workGetCgi(std::string format, std::string requestFile)
{
	pid_t pid;
    int fd_out[2];
	char buf[1024];
	if (format.empty())
		return (1);
	std::string request_method = "REQUEST_METHOD=GET";
	std::string query_string = "QUERY_STRING=";
	std::string requestFileRoot = _loc.getRoot().append(requestFile);
	std::string script_filename = "SCRIPT_FILENAME=";
	script_filename.append(requestFileRoot);
	char *args[]= {const_cast<char*>(format.c_str()), (char *) "-f", const_cast<char*>(requestFileRoot.c_str()), NULL};	
	// char *args[]= {const_cast<char*>(format.c_str()), const_cast<char*>(requestFileRoot.c_str()), NULL};	
	char *header[] = {
	const_cast<char*> (script_filename.c_str()),
	const_cast<char*> (request_method.c_str()),
	const_cast<char*> (query_string.append(_getParams).c_str()),
	(char *) "REDIRECT_STATUS=200",
	(char *) NULL
	};

	pipe(fd_out);

	if ((pid = fork()) < 0)
	{
		perror("fork");
		return EXIT_FAILURE;
	}
	else if (!pid) 
	{ 
		/* child */
		dup2(fd_out[1], STDOUT_FILENO);
		dup2(fd_out[1], STDERR_FILENO);
		close(fd_out[0]);
		execve(args[0], args, header);
		perror("exec");
		return EXIT_FAILURE;
	} 
	else 
	{ 
		/* parent */
		close(fd_out[1]);
		while (waitpid(-1, NULL, WUNTRACED) != -1)
			;
		std::cout << "Before read" << std::endl;
		int n = read(fd_out[0], buf, 1024);
		if (n < 0)
		{
			std::cout << "Error read" << std::endl;
			perror("read");
           	exit(EXIT_FAILURE);
        } else
		{
            buf[n] = '\0';
			std::string mystring = buf;
			std::string substring = "Content-type";
			if (mystring.find(substring) != std::string::npos && mystring.find_first_of("\n\n") != std::string::npos)
				mystring.erase(mystring.find(substring), mystring.find_first_of("\n\n"));
			_cgiResponse.append(mystring);
			std::cout << "CGI response ="<<_cgiResponse << std::endl;
		}
	}
	return (1);
}

int Client::workDeleteCgi(std::string format, std::string requestFile)
{
	pid_t pid;
    int fd_out[2];
	char buf[1024];
	std::string request_method = "REQUEST_METHOD=DELETE";
	std::string query_string = "QUERY_STRING=";
	std::string requestFileRoot = _loc.getRoot().append(requestFile);
	std::string script_filename = "SCRIPT_FILENAME=";
	script_filename.append(requestFileRoot);
	char *args[]= {const_cast<char*>(format.c_str()), const_cast<char*>(requestFileRoot.c_str()), NULL};	
	char *header[] = {
		const_cast<char*> (script_filename.c_str()),
		const_cast<char*> (request_method.c_str()),
		const_cast<char*> (query_string.append(_getParams).c_str()),
		(char *) "REDIRECT_STATUS=200",
		(char *) NULL
	};

	pipe(fd_out);

	if ((pid = fork()) < 0)
	{
		perror("fork");
		return EXIT_FAILURE;
	}
	else if (!pid) 
	{ 
		/* child */
		dup2(fd_out[1], STDOUT_FILENO);
		dup2(fd_out[1], STDERR_FILENO);
		close(fd_out[0]);
		execve(args[0], args, header);
		perror("exec");
		return EXIT_FAILURE;
	} 
	else 
	{ 
		/* parent */
		close(fd_out[1]);
		while (waitpid(-1, NULL, WUNTRACED) != -1)
			;
		std::cout << "Before read" << std::endl;
		int n = read(fd_out[0], buf, 1024);
		if (n < 0)
		{
			std::cout << "Error read" << std::endl;
			perror("read");
           	exit(EXIT_FAILURE);
        } else
		{
            buf[n] = '\0';
			std::string mystring = buf;
			std::string substring = "Content-type";
			if (mystring.find(substring) != std::string::npos && mystring.find_first_of("\n\n") != std::string::npos)
				mystring.erase(mystring.find(substring), mystring.find_first_of("\n\n"));
			_cgiResponse.append(mystring);
			std::cout << "CGI DELETE response ="<<_cgiResponse << std::endl;
		}
	}
	return (1);
}

void Client::verifyCgi()
{
	//TODO: faire une erreur qd le php ne peut pas lire
	//TODO: que faire ds le cas d'un file sans ext ?
	std::cout << "** verifyCgi **" << std::endl;
	std::string format;
	std::string requestFile;
	std::string key;
	size_t pos_space = 0;
	size_t pos_slash = 0;
	size_t pos_point = 0;
	pos_space = _request[0].find_last_of(' ');
	pos_point = _request[0].find_first_of('.');
	pos_slash = _request[0].find_first_of('/');
	if (pos_space < pos_point)
		format = "";
	else
	{
		format = _request[0].substr(pos_point + 1, pos_space - (pos_point + 1));
		format =format.substr(0, format.find_first_of('?'));
	}
	std::cout << "format = " << format<< std::endl;
	size_t postIndex = _request[0].find("POST");
	size_t getIndex = _request[0].find("GET");
	size_t deleteIndex = _request[0].find("DELETE");
	requestFile = _request[0].substr(pos_slash + 1, pos_space - (pos_slash + 1));
	_getParams = requestFile.substr(requestFile.find_first_of('?') + 1);
	requestFile =requestFile.substr(0, requestFile.find_first_of('?'));
	std::cout << "112 -- " << requestFile << std::endl;
	_loc = _server->getLocationByPath('/' + requestFile);
	key = _loc.getKey();
	if (key.length() != 1)
		requestFile = requestFile.substr(key.length() - 2);
	std::cout << "999 -- " << requestFile << std::endl;
	if (postIndex != std::string::npos)
	{
		std::cout << "** POST **" << std::endl;
		if (!_loc.getAcceptedMethods()._post)
			_errorcode = 405;
		else {
			try {
				// _server->getLocationByPath(_requestData.path)
				// std::cout << "_server->getCgiValue(format)" << format << std::endl;
				workPostCgi(_loc.getCgiValue(format), requestFile);
				saveFile();
			}
			catch(std::exception e)
			{
				_errorcode = 405;
			}
		}
	}
	else if (getIndex != std::string::npos)
	{
		std::cout << "** GET **" << std::endl;
		if (!_loc.getAcceptedMethods()._get)
			_errorcode = 405;
		else
		{
			try {
				workGetCgi(_loc.getCgiValue(format), requestFile);
			}
			catch(std::exception e)
			{
				bool found = false;
				for (std::vector<std::string>::iterator it = \
				authorizedExtension.begin(); it != authorizedExtension.end(); ++it) 
				{
					if (*it == format || format.empty()) 
					{
						found = true;
						break;
					}
				}
				if (!found)
					_errorcode = 500;
			}
		}
	}
	else if (deleteIndex != std::string::npos)
	{
		std::cout << "** DELETE **" << std::endl;
		if (!_loc.getAcceptedMethods()._get)
			_errorcode = 405;
		else
		{
			try
			{
				workDeleteCgi(_loc.getCgiValue(format), requestFile);
			}
			catch(const std::exception& e)
			{
				if (!access(_loc.getRoot().append(requestFile).c_str(), F_OK))
				{
					//file exist
					if (!access(_loc.getRoot().append(requestFile).c_str(), W_OK))
					{
						//file writable
						if (!remove(_loc.getRoot().append(requestFile).c_str()))
							std::cout << "File deleted" << std::endl;
						else
							_errorcode = 500;
							// std::cout <<"File not deleted" << std::endl;
					}
					else
						_errorcode = 4041;
						// std::cout <<"No rights to delete" << std::endl;
				}
				else
					_errorcode = 404;
					// std::cout << "File not exist" << std::endl;
			}
		}
	}
	
}

void Client::saveFile()
{
	//TODO: remettre les spaces ds le file 
	std::cout << "** Save file **" << std::endl;
	transformBodyStringtoMap();

	std::map<std::string,std::string>::iterator it_file;
	std::map<std::string,std::string>::iterator it_name;
	// std::map<std::string,std::string>::const_iterator it;
	// std::map<std::string,std::string>::const_iterator ite = _requestmapBody.end(); 
	// for (it = _requestmapBody.begin(); it != ite; ++it)
	// {
	// 	std::cout << "REQUEST = " << it->first << " ** " << it->second << std::endl;
	// }
	it_file = _requestmapBody.find("file");
	it_name = _requestmapBody.find("name");
	if (it_file != _requestmapBody.end())
	{
		std::cout <<YEL <<  "FILE = " << it_name->second<< WHT<< std::endl;
		//open a file in write mode
		ofstream outfile;
		std::string new_path = "www/new_files/";
		mkdir(new_path.c_str(), 0777);
		outfile.open(new_path.append(it_name->second).c_str());
		//write the string
		outfile << it_file->second.substr(2, it_file->second.size() - 3);
		outfile.close();
	}
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