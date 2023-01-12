#include "ParsingRequest.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ParsingRequest::ParsingRequest( std::vector<std::string> request, Server *server, std::string cgiResponse, int error_code):
 _errorcode(error_code), _request(request), _server(server)
{
	_requestData.isCgi = !cgiResponse.empty();
	if (_requestData.isCgi)
		_requestData._cgiResponse = cgiResponse;
	parsingRequest();
	foundFileToSend();
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

ParsingRequest::~ParsingRequest()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

// ParsingRequest &				ParsingRequest::operator=( ParsingRequest const & rhs )
// {
// 	if ( this != &rhs )
// 	{
// 		this->_value = rhs.getValue();
// 	}
// 	return *this;
// }

// std::ostream &			operator<<( std::ostream & o, ParsingRequest const & i )
// {
// 	o << "Value = " << i.getValue();
// 	return o;
// }


/*
** --------------------------------- METHODS ----------------------------------
*/

void ParsingRequest::parsingRequest()
{
	for(unsigned long int i = 0; i < _request.size()  ; i++)
	{
		if (i == 0)
		{
			_requestData.methode = _request[i].substr(0, _request[i].find(" ", 0));
			unsigned int first = _requestData.methode.size() + 1;
			unsigned int last = _request[i].find(" ", first);
			_requestData.path = _request[i].substr(first, last - first);
			_requestData.protocol = _request[i].substr(last + 1, _request[i].size());
		}
	}
	_autoindex = _server->getLocationByPath(_requestData.path).getAutoIndex();

}

void ParsingRequest::setFileToSend404()
{
	std::string path;
	path = _server->getLocationByPath("/" + _requestData.path).getRoot();
	path += "404.html";
	if (access(path.c_str(), R_OK) == 0)
		_requestData.fileToSend = path;
	else
		_requestData.fileToSend = "error_pages/404.html";
}

void ParsingRequest::setFileToSend403()
{
	std::string path;
	path = _server->getLocationByPath("/" +_requestData.path).getRoot();
	path += "403.html";
	if (access(path.c_str(), R_OK) == 0)
		_requestData.fileToSend = path;
	else
		_requestData.fileToSend = "error_pages/403.html";
}



int ParsingRequest::filepermission()
{
	int fd;

	fd = access(_requestData.fileToSend.c_str(), F_OK);
	if (fd == -1 || _errorcode == 404)
		setFileToSend404();
	fd = access(_requestData.fileToSend.c_str(), R_OK);
	if (fd == -1 || _autoindex == 0)
		setFileToSend403();
	return (0);
}

int ParsingRequest::foundFileToSend()
{
	std::string rootPath;
	std::string fullPathFile;
	_requestData.isIndex = 0;

	rootPath = _server->getLocationByPath(_requestData.path).getRoot();

	if (_requestData.path[0] == '/')
		_requestData.path.erase(0, 1);

	_requestData.fileToSend = rootPath + _requestData.path ;
	fullPathFile = _requestData.fileToSend;
	
	if (_autoindex == 1 && isDirectory(fullPathFile) && !fileExist(fullPathFile + "index.html"))
	{
		// std::cout << "--1--" << std::endl;
		_requestData.fileToSend = fullPathFile;
		_requestData.isIndex = 1;
	}
	else if (isDirectory(fullPathFile))
	{
		// std::cout << "--2--" << std::endl;
		_requestData.fileToSend = rootPath + "index.html";
	}
	else if (_autoindex == 0 && isDirectory(fullPathFile) && !fileExist(fullPathFile + "index.html"))
	{
		// std::cout << "--3--" << std::endl;
		filepermission();
	}
	else if (_errorcode == 404)
		filepermission();
	else
	{
		// std::cout << "--4--" << std::endl;
		_requestData.fileToSend = fullPathFile;
	}


	FILE *f = fopen(fullPathFile.c_str(), "r+");
	if ( f == NULL)
	{
		std::cout << "Can't Open" << std::endl;
		if (_autoindex == 1 && errno == 21)
			return (1);
		filepermission();
	}
	else
		fclose(f);
	return (0);
}


void ParsingRequest::displayData() const
{
	std::cout << "Request Data :" << std::endl;
	std::cout << "methode \t= " 				<< _requestData.methode << std::endl;
	std::cout << "protocol \t= " 			<< _requestData.protocol << std::endl;
	// std::cout << "Connection = "			<< _requestData.Connection << std::endl;
	std::cout << "path \t\t= "					<< _requestData.path << std::endl;
	std::cout << "fileToSend \t= "			<< _requestData.fileToSend << std::endl;
	std::cout << "isIndex \t= " 				<< _requestData.isIndex << std::endl;
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/

s_requestData ParsingRequest::getData()
{
	return (_requestData);
}


/* ************************************************************************** */