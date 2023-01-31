#include "CGI.hpp"


/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

CGI::CGI(std::vector<std::string> *request, Server *server, int *errorcode, \
std::string *requestBody, std::map<std::string, std::string> *requestmap, std::string *cgiResponse) \
:_requestBody(requestBody), _server(server), _errorcode(errorcode), _request(request), \
_requestmap(requestmap), _cgiResponse(cgiResponse)
{
	this->_getParams.clear();
	this->authorizedExtension = setAuthorizedExtension(this->authorizedExtension);

}

CGI::CGI(const CGI &src) : _getParams(src._getParams),_requestBody(src._requestBody), _server(src._server), \
_errorcode(src._errorcode), _request(src._request), _requestmap(src._requestmap), _cgiResponse(src._cgiResponse)
{
	this->authorizedExtension = setAuthorizedExtension(this->authorizedExtension);
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

CGI::~CGI()
{

}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

CGI &CGI::operator=(CGI const &rhs)
{
	if (this != &rhs)
	{
		this->_getParams = rhs._getParams;
		this->_loc = rhs._loc;
		this->_server = rhs._server;
		this->_errorcode = rhs._errorcode;
		this->_request = rhs._request;
		this->authorizedExtension = rhs.authorizedExtension;
		this->_requestBody = rhs._requestBody;
		this->_requestmap = rhs._requestmap;
	}
	return *this;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

std::vector<std::string> CGI::setAuthorizedExtension(std::vector<std::string> ext)
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


void CGI::verifyCgi()
{
	//TODO: faire une erreur qd le php ne peut pas lire
	//TODO: que faire ds le cas d'un file sans ext ?
	std::cout << "** verifyCgi **" << std::endl;
	std::string format;
	std::string requestFile;
	std::string firstReq = (*_request)[0];
	std::string key;
	size_t pos_space = 0;
	size_t pos_slash = 0;
	size_t pos_point = 0;
	pos_space = firstReq.find_last_of(' ');
	pos_point = firstReq.find_first_of('.');
	pos_slash = firstReq.find_first_of('/');
	if (pos_space < pos_point)
		format = "";
	else
	{
		format = firstReq.substr(pos_point + 1, pos_space - (pos_point + 1));
		format =format.substr(0, format.find_first_of('?'));
	}
	std::cout << "format = " << format<< std::endl;
	size_t postIndex = firstReq.find("POST");
	size_t getIndex = firstReq.find("GET");
	size_t deleteIndex = firstReq.find("DELETE");
	_getParams = "";
	requestFile = getRequestFile(firstReq, &_getParams);
	std::cout << "requestFile= " << requestFile << std::endl;
	_loc = _server->getLocationByPath('/' + requestFile);
	key = _loc.getKey();
	if (key.length() != 1)
		requestFile = requestFile.substr(key.length() - 2);
	if (postIndex != std::string::npos)
	{
		std::cout << "** POST **" << std::endl;
		if (!_loc.getAcceptedMethods()._post)
			*_errorcode = 405;
		else {
			try {
				// _server->getLocationByPath(_requestData.path)
				// std::cout << "_server->getCgiValue(format)" << format << std::endl;
				// std::cout << "SERVERU = "
				std::cout << "request = " << format << std::endl;
				workPostCgi(_loc.getCgiValue(format), requestFile);
			}
			catch(std::exception e)
			{
				// std::cout << "Request = " <<
				std::cout << "request = " << *_requestBody << std::endl;
				// exit(1);
				if (!saveFile(*_requestmap, *_requestBody))
					*_errorcode = 405;
			}
		}
	}
	else if (getIndex != std::string::npos)
	{
		std::cout << "** GET **" << std::endl;
		if (!_loc.getAcceptedMethods()._get)
			*_errorcode = 405;
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
					*_errorcode = 500;
			}
		}
	}
	else if (deleteIndex != std::string::npos)
	{
		std::cout << "** DELETE **" << std::endl;
		if (!_loc.getAcceptedMethods()._delete)
		{
			*_errorcode = 405;
			std::cout << "*_errorcode" << *_errorcode<< std::endl;
		}
		else
		{
			std::cout << "SALUT " << std::endl;
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
							*_errorcode = 500;
							// std::cout <<"File not deleted" << std::endl;
					}
					else
						*_errorcode = 4041;
						// std::cout <<"No rights to delete" << std::endl;
				}
				else
					*_errorcode = 404;
					// std::cout << "File not exist" << std::endl;
			}
		}
	}
}


int CGI::workPostCgi(std::string format, std::string requestFile)
{
	std::cout << "_requestBody =" << _requestBody[0] << std::endl;
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
	if(envCGI.empty())
		envCGI = itos(_requestBody->length());
	script_filename.append(requestFileRoot);
	try 
	{
		std::cout << "Found boundary" << std::endl;
		ft_find_boundary_utils(*_requestmap);
	}
	catch(std::exception e)
	{
		content_type = "CONTENT_TYPE=application/x-www-form-urlencoded";
	}
	if (content_type.empty())
	{
		content_type = "CONTENT_TYPE=multipart/form-data; boundary=";
		content_type.append(ft_find_boundary_utils(*_requestmap));
	}
	// char *args[]= {const_cast<char*>(format.c_str()), const_cast<char*>(requestFileRoot.c_str()), NULL};	
	char *args[]= {const_cast<char*>(format.c_str()), (char *) "-f", const_cast<char*>(requestFileRoot.c_str()), NULL};	
	char *header[] = {
		const_cast<char*> (script_filename.c_str()),
		const_cast<char*> (request_method.c_str()),
		const_cast<char*>(content_type.c_str()),
		const_cast<char*>(content_length.append(envCGI).c_str()),
		// const_cast<char*>("CONTENT_LENGTH=80"),
		const_cast<char*> (query_string.append(_getParams).c_str()),
		(char *) "REDIRECT_STATUS=200",
		(char *) NULL
	};
	std::cout << YEL << "SCRIPT = " << script_filename<<reset<< std::endl;
	std::cout << YEL << "METHOD = " << request_method<<reset<< std::endl;
	std::cout << YEL << "TYPE = " << content_type<<reset<< std::endl;
	std::cout << YEL << "LENGTH = " << content_length<<reset<< std::endl;
	std::cout << YEL << "Body = " << _requestBody <<reset<< std::endl;
   	pipe(fd);
	pipe(fd_out);
	write(fd[1], _requestBody->c_str(), _requestBody->length());
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
			_cgiResponse->append(mystring);
			// std::cout << "_cgiResponse" << *_cgiResponse << std::endl;
		}
	}
	return (1);
}

int CGI::workGetCgi(std::string format, std::string requestFile)
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
			_cgiResponse->append(mystring);
			std::cout << "CGI response ="<<_cgiResponse << std::endl;
		}
	}
	return (1);
}

int CGI::workDeleteCgi(std::string format, std::string requestFile)
{
	std::cout << "*** DELETE CGI ***" << std::endl;
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
			_cgiResponse->append(mystring);
			std::cout << "CGI DELETE response ="<<_cgiResponse << std::endl;
		}
	}
	return (1);
}


std::string CGI::findValueEnvCgi(std::string key)
{
	std::map<std::string,std::string>::iterator it;
	it = _requestmap->find(key);
	if (it != _requestmap->end())
		return (it->second);
	return (NULL);
}