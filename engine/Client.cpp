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
	this->_moverSave = 0;
	this->_cgiResponse.clear();
}

Client::Client(const Client &src) : _clientfd(src._clientfd), _server(src._server)
{
	this->_headerIsRead = false;
	this->_firstTimeBody = true;
	this->_bodyContentLenght = 0;
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
	event.events = EPOLLOUT | EPOLLRDHUP;
	event.data.fd = _clientfd;
	epoll_ctl(_server->getEpollFd(), EPOLL_CTL_MOD, _clientfd, &event);

	setKeepAlive();
	displayRequest();
	verifyCgi();
	saveFile();	
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
	ParsingRequest parsingRequest(_request, _server, _cgiResponse);
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
	_headerIsRead = false;
	_firstTimeBody = false;
	_bodyContentLenght = 0;
	_isKeepAlive = false;
	_isSend = false;
	_moverSave = 0;


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
			epoll_ctl(_server->getEpollFd(), EPOLL_CTL_MOD, _clientfd, &event);
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
	// std::cout << BRED <<  "--- BOUNDARY ---" << WHT << std::endl;
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

int Client::workCgi(std::string format, std::string requestFile)
{
	std::cout << "_requestBody\t=\t" << _requestBody << std::endl;
	pid_t pid;
    int fd[2];
    int fd_out[2];
	char buf[1024];
	std::string content_type;
	std::string request_method = "REQUEST_METHOD=";
	std::string content_length = "CONTENT_LENGTH=";
	std::string requestFileRoot = _server->getRoot().append(requestFile);
	std::string script_filename = "SCRIPT_FILENAME=";
	script_filename.append(requestFileRoot);
	if (ft_find_boundary().empty())
		content_type = "CONTENT_TYPE=application/x-www-form-urlencoded";
	else
	{
		content_type = "CONTENT_TYPE=multipart/form-data; boundary=";
		content_type.append(ft_find_boundary());
	}
	char *args[]= {const_cast<char*>(format.c_str()), (char *) "-f", const_cast<char*>(requestFileRoot.c_str()), NULL};	
	char *header[] = {
	const_cast<char*> (script_filename.c_str()),
	const_cast<char*> (request_method.append(findMethod()).c_str()),
	const_cast<char*>(content_type.c_str()),
	const_cast<char*>(content_length.append(findValueEnvCgi("Content-Length")).c_str()),
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
			_cgiResponse.append(buf);
			std::cout << _cgiResponse << std::endl;
		}
	}
	return (1);
}

void Client::verifyCgi()
{
	std::cout << "** verifyCgi **" << std::endl;

	std::string format;
	std::string requestFile;
	size_t pos_space = 0;
	size_t pos_slash = 0;
	size_t pos_point = 0;
	size_t postIndex = _request[0].find("POST");


	if (postIndex != std::string::npos)
	{
		pos_point = _request[0].find_first_of('.');
		pos_space = _request[0].find_last_of(' ');
		pos_slash = _request[0].find_first_of('/');
		format = _request[0].substr(pos_point + 1, pos_space - (pos_point + 1));
		requestFile = _request[0].substr(pos_slash + 1, pos_space - (pos_slash + 1));
		std::cout << "Request file = " <<  _server->getCgiValue(format) << std::endl;
		std::cout << "format = " << format << std::endl;
		try {
			std::cout << "server = " << _server->getCgiValue(format) << std::endl;
			workCgi(_server->getCgiValue(format), requestFile);
		}
		catch(std::exception e)
		{
			std::cout << "CGI NOT FOUND!" << std::endl;
		}
	}
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
	std::cout << "Save file 2 = " << std::endl;
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