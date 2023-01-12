#include "Client.hpp"

std::vector<std::string> setAuthorizedExtension(std::vector<std::string> ext);

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
			if (_bodyContentLenght == 0 || _requestBody.size() == _bodyContentLenght)
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
			if (pos_equal == std::string::npos)
				throw std::exception();
			std::cout << "pos_equal= " << pos_equal << std::endl;
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
					value += findBodyValue(vector, i, value, key);
			}
			else
			{
				std::cout << "key = " << key << "&& value = " << value << std::endl;
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
		std::map<std::string,std::string>::const_iterator it;
		std::map<std::string,std::string>::const_iterator ite = _requestmapBody.end(); 
		for (it = _requestmapBody.begin(); it != ite; ++it)
		{
			std::cout << "REQUEST = " << it->first << " ** " << it->second << std::endl;
		}
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
	std::string request_method = "REQUEST_METHOD=POST";
	std::string content_length = "CONTENT_LENGTH=";
	std::string requestFileRoot = _server->getRoot().append(requestFile);
	std::string script_filename = "SCRIPT_FILENAME=";
	script_filename.append(requestFileRoot);
	try 
	{
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
	// std::cout << "ft_find_boundary = " << ft_find_boundary() << std::endl;
	// std::cout << "content_type = " << content_type << std::endl;
	char *args[]= {const_cast<char*>(format.c_str()), (char *) "-f", const_cast<char*>(requestFileRoot.c_str()), NULL};	
	char *header[] = {
	const_cast<char*> (script_filename.c_str()),
	const_cast<char*> (request_method.c_str()),
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

int Client::workGetCgi(std::string format, std::string requestFile)
{
	std::cout << "workGetCgi\t=\t" << std::endl;
	std::cout << "requestFile = " << requestFile << std::endl;
	std::cout << "format = " << format << std::endl;
	pid_t pid;
    int fd_out[2];
	char buf[1024];
	std::string request_method = "REQUEST_METHOD=GET";
	std::string requestFileRoot = _server->getRoot().append(requestFile);
	std::string script_filename = "SCRIPT_FILENAME=";
	script_filename.append(requestFileRoot);
	char *args[]= {const_cast<char*>(format.c_str()), (char *) "-f", const_cast<char*>(requestFileRoot.c_str()), NULL};	
	char *header[] = {
	const_cast<char*> (script_filename.c_str()),
	const_cast<char*> (request_method.c_str()),
	(char *) "QUERY_STRING=name=gr&email=hello%40hello.com",
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
			_cgiResponse.append(buf);
			std::cout << "CGI response ="<<_cgiResponse << std::endl;
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
	pos_point = _request[0].find_first_of('.');
	pos_space = _request[0].find_last_of(' ');
	pos_slash = _request[0].find_first_of('/');
	format = _request[0].substr(pos_point + 1, pos_space - (pos_point + 1));
	format =format.substr(0, format.find_first_of('?'));
	size_t postIndex = _request[0].find("POST");
	size_t getIndex = _request[0].find("GET");
	requestFile = _request[0].substr(pos_slash + 1, pos_space - (pos_slash + 1));
	requestFile =requestFile.substr(0, requestFile.find_first_of('?'));
	std::cout << "Format = " << format << std::endl;
	std::cout << "requestFile = " << requestFile << std::endl;
	if (postIndex != std::string::npos)
	{
		// std::cout << ""
		try {
			workPostCgi(_server->getCgiValue(format), requestFile);
			saveFile();
		}
		catch(std::exception e)
		{
			_errorcode = 404;
		}
	}
	else if (getIndex != std::string::npos)
	{
		//mettre catch qd il n'y a pas de CGI
		std::cout << "** GET **" << std::endl;
		try {
			workGetCgi(_server->getCgiValue(format), requestFile);
		}
		catch(std::exception e)
		{
			bool found = false;
			for (std::vector<std::string>::iterator it = \
			authorizedExtension.begin(); it != authorizedExtension.end(); ++it) 
			{
				if (*it == format) 
				{
					std::cout << "found = " << found  << "format = "<<format << std::endl;
					found = true;
					break;
				}
			}
			std::cout << "found = " << found  << "format = "<<format << std::endl;
			// std::cout << "authorizedExtension = " << authorizedExtension[2]  << "format = "<<format << std::endl;
			if (!found)
				_errorcode = 404;
		}
	}
}

void Client::saveFile()
{
	std::cout << "Save file = " << std::endl;
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
		std::string new_path = "new_files/";
		mkdir(new_path.c_str(), 0777);
		outfile.open(new_path.append(it_name->second).c_str());
		//write the string
		outfile << it_file->second.substr(2, it_file->second.size() - 3);
		outfile.close();
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