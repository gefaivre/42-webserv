#include "ParsingRequest.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ParsingRequest::ParsingRequest(std::string path, bool autoindex, std::vector<std::string> request):
_path(path), _autoindex(autoindex), _request(request)
{
	std::cout << autoindex << std::endl;
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
}

int ParsingRequest::filepermission()
{
	int fd;
	std::cout << _requestData.fileToSend << std::endl;
	fd = access(_requestData.fileToSend.c_str(), F_OK);
	if (fd == -1)
		_requestData.fileToSend = _path + "404.html";
	fd = access(_requestData.fileToSend.c_str(), R_OK);
	if (fd == -1 || _autoindex == 0)
		_requestData.fileToSend = _path + "403.html";
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
	
	if (_autoindex == 1 && isDirectory(_path + _requestData.fileToSend) && !fileExist(_path + _requestData.fileToSend + "index.html"))
	{
		_requestData.fileToSend = _path + _requestData.fileToSend;
		_requestData.isIndex = 1;
	}
	else if (isDirectory(_path + _requestData.fileToSend))
		_requestData.fileToSend = _path + "index.html";
	else if (_autoindex == 0 && isDirectory(_path + _requestData.fileToSend) && !fileExist(_path + _requestData.fileToSend + "index.html"))
		filepermission();
	else
		_requestData.fileToSend = _path + _requestData.fileToSend;

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