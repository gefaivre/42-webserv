#include "ParsingRequest.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ParsingRequest::ParsingRequest( std::vector<std::string> request, Server *server, std::string cgiResponse, int error_code):
 _errorcode(error_code), _request(request), _server(server)
{
	_requestData.isRedirection = false;
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
	std::string key;
	for(unsigned long int i = 0; i < _request.size()  ; i++)
	{
		if (i == 0)
		{
			_requestData.methode = _request[i].substr(0, _request[i].find(" ", 0));
			unsigned int first = _requestData.methode.size() + 1;
			unsigned int last = _request[i].find_first_of(" ", first);
			_requestData.path = _request[i].substr(first, last - first);
			_requestData.getParams = _requestData.path.substr(_requestData.path.find_first_of('?') + 1);
			_requestData.path =_requestData.path.substr(0, _requestData.path.find_first_of('?'));
			_requestData.protocol = _request[i].substr(last + 1, _request[i].size());
		}
	}
	_requestData.pathKey = _requestData.path;
	key = _server->getLocationByPath(_requestData.path).getKey();
	if (key.length() != 1)
		_requestData.path = _requestData.path.substr(key.length() - 1);
	_autoindex = _server->getLocationByPath(_requestData.path).getAutoIndex();

}

void ParsingRequest::setFileToSend(std::string errorcode)
{
	std::string path;
	path = _server->getLocationByPath("/" +_requestData.path).getRoot();
	path += errorcode;
	if (access(path.c_str(), R_OK) == 0)
		_requestData.fileToSend = path;
	else
		_requestData.fileToSend = ft_pwd() + "/error_pages/" + errorcode;
}

int ParsingRequest::filepermission()
{
	int fd;

	if (_errorcode == 405)
		setFileToSend("405.html");
	else if (_errorcode == 413)
		setFileToSend("413.html");
	else if (_errorcode == 202)
		setFileToSend("202.html");
	else if (_errorcode == 4041)
		setFileToSend("404_noSpecified.html");
	else if (_errorcode == 500)
		setFileToSend("500.html");
	fd = access(_requestData.fileToSend.c_str(), F_OK);
	if (fd == -1)
	{
		if (!(_requestData.methode == "DELETE"))
			setFileToSend("404.html");
	}
	if (_errorcode == 404)
			setFileToSend("404.html");
	fd = access(_requestData.fileToSend.c_str(), R_OK);
	if (fd == -1)
	{
		if (!(_requestData.methode == "DELETE"))
			setFileToSend("403.html");
	}
	if(_autoindex == 0)
			setFileToSend("403.html");
	return (0);
}

int ParsingRequest::foundFileToSend()
{

	std::cout << "Found file to send" << std::endl;

	std::string rootPath;
	std::string fullPathFile;
	_requestData.isIndex = 0;
	rootPath = _server->getLocationByPath(_requestData.pathKey).getRoot();
	if (_requestData.path[0] == '/')
		_requestData.path.erase(0, 1);

	_requestData.fileToSend = rootPath + _requestData.path ;
	fullPathFile = _requestData.fileToSend;
	_autoindex =  _server->getLocationByPath(_requestData.pathKey).getAutoIndex();

	std::string redirection = _server->getLocationByPath(_requestData.pathKey).getRedirectionValue(_requestData.pathKey);
	if(!redirection.empty() )
	{
		_requestData.isRedirection = true;
		_requestData.fileToSend = redirection;
	}
	else if(isDirectory(fullPathFile) && _server->getLocationByPath(_requestData.pathKey).getIndex().size())
	{
		_requestData.fileToSend = rootPath + _server->getLocationByPath(_requestData.pathKey).getIndex();
		_requestData.path = _server->getLocationByPath(_requestData.pathKey).getIndex();
	}
	else if (_autoindex == 1 && isDirectory(fullPathFile) && !fileExist(fullPathFile + "index.html"))
	{
		_requestData.fileToSend = fullPathFile;
		_requestData.isIndex = 1;
	}
	else if (isDirectory(fullPathFile))
	{
		_requestData.fileToSend = rootPath + "index.html";
		_requestData.path = _server->getLocationByPath(_requestData.pathKey).getIndex() + "index.html";

	}
	else if (_autoindex == 0 && isDirectory(fullPathFile) && !fileExist(fullPathFile + "index.html"))
		filepermission();
	else if (_errorcode)
		filepermission();
	else
		_requestData.fileToSend = fullPathFile;

	if(_requestData.isRedirection == false)
	{
		FILE *f = fopen(_requestData.fileToSend.c_str(), "r+");

		if (f == NULL)
		{
			if (_autoindex == 1 && errno == 21)
				return (1);
			filepermission();
		}
		else
			fclose(f);
	}
	return (0);
}


void ParsingRequest::displayData() const
{
	std::cout << "Request Data :" << std::endl;
	std::cout << "methode \t= " 				<< _requestData.methode << std::endl;
	std::cout << "protocol \t= " 			<< _requestData.protocol << std::endl;
	std::cout << "Connection = "			<< _requestData.Connection << std::endl;
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