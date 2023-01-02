#include "ParsingRequest.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ParsingRequest::ParsingRequest( std::vector<std::string> request, Server *server):
 _request(request), _server(server)
{
	parsingRequest();
	foundFileToSend();
}

// ParsingRequest::ParsingRequest( const ParsingRequest & src )
// {
// }


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

void ParsingRequest::setFileToSend404()
{
	std::string path;
	path = _server->getLocationByPath(_requestData.path).getRoot();
	path += "404.html";
	if (access(path.c_str(), R_OK) == 0)
		_requestData.fileToSend = path;
	else
		_requestData.fileToSend = "error_pages/404.html";
}

void ParsingRequest::setFileToSend403()
{
	std::string path;
	path = _server->getLocationByPath(_requestData.path).getRoot();
	// std::cout << _server->getLocationByPath(_requestData.path).getRoot() << std::endl;
	path += "403.html";
	if (access(path.c_str(), R_OK) == 0)
		_requestData.fileToSend = path;
	else
		_requestData.fileToSend = "error_pages/403.html";
}


void ParsingRequest::parsingRequest()
{
	for(unsigned long int i = 0; i < _request.size()  ; i++)
	{
		if (_request.size() > 0 && i == 0)
		{
			_requestData.methode = _request[i].substr(0, _request[i].find(" ", 0));
			unsigned int first = _requestData.methode.size() + 1;
			unsigned int last = _request[i].find(" ", first);
			_requestData.path = _request[i].substr(first, last - first);
			_requestData.protocol = _request[i].substr(last + 1, _request[i].size());
		}
		// std::cout << "_request = " <<_request[i] << std::endl;
	}
	_autoindex = _server->getLocationByPath(_requestData.path).getAutoIndex();
	// std::cout << "auto index = "<< _autoindex << std::endl;
}

int ParsingRequest::filepermission()
{
	std::cout << _requestData.fileToSend << std::endl;
	int fd;
	std::cout << _requestData.fileToSend << std::endl;
	fd = access(_requestData.fileToSend.c_str(), R_OK);
	if (fd == -1 || _autoindex == 0)
		setFileToSend403();
	fd = access(_requestData.fileToSend.c_str(), F_OK);
	// std::cout << "TESTTTTTTTTTT" << fd << std::endl;
	if (fd == -1)
	// {
		setFileToSend404();
		// return (0);
	// }
		// _requestData.fileToSend = _path + "403.html";
	return (0);
}

int ParsingRequest::foundFileToSend()
{
	_requestData.fileToSend = _requestData.path ;
	if (_requestData.fileToSend[0] == '/')
		_requestData.fileToSend.erase(0, 1);
	// std::cout << "file = " << _requestData.fileToSend[0] << std::endl;
	// std::cout << "fileToSend = " << _requestData.fileToSend << std::endl;

	_requestData.isIndex = 0;
	// std::cout << "Root = " << isDirectory(_server->getLocationByPath(_requestData.path).getRoot() + _requestData.fileToSend) << std::endl;
	// std::cout << "file = " << !fileExist(_server->getLocationByPath(_requestData.path).getRoot() + _requestData.fileToSend + "index.html")<< std::endl;
	// std::cout << "_autoindex = " << _autoindex<< std::endl;
	
	if (_autoindex == 1 && isDirectory(_server->getLocationByPath(_requestData.path).getRoot() + _requestData.fileToSend) && !fileExist(_server->getLocationByPath(_requestData.path).getRoot() + _requestData.fileToSend + "index.html"))
	{
		_requestData.fileToSend = _server->getLocationByPath(_requestData.path).getRoot() + _requestData.fileToSend;
		_requestData.isIndex = 1;
	}
	else if (isDirectory(_server->getLocationByPath(_requestData.path).getRoot() + _requestData.fileToSend))
		_requestData.fileToSend = _server->getLocationByPath(_requestData.path).getRoot() + "index.html";
	
	else if (_autoindex == 0 && isDirectory(_server->getLocationByPath(_requestData.path).getRoot() + _requestData.fileToSend) && !fileExist(_server->getLocationByPath(_requestData.path).getRoot() + _requestData.fileToSend + "index.html"))
		filepermission();
	else
		_requestData.fileToSend = _server->getLocationByPath(_requestData.path).getRoot() + _requestData.fileToSend;
	FILE *f = fopen(_requestData.fileToSend.c_str(), "r+");
	if ( f == NULL)
	{
		if (_autoindex == 1 && errno == 21)
			return (1);
		filepermission();
	}
	else
		fclose(f);
	return (0);
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/

s_requestData ParsingRequest::getData()
{
	return (_requestData);
}


/* ************************************************************************** */