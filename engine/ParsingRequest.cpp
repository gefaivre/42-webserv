#include "ParsingRequest.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ParsingRequest::ParsingRequest(std::string path, bool autoindex, std::vector<std::string> request):
_path(path), _autoindex(autoindex), _request(request)
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

void ParsingRequest::parsingRequest()
{
	for(unsigned long int i = 0; i < /* _request.size() */ 1 ; i++)
	{
		if (_request.size() > 0 && i == 0)
		{
			_requestData.methode = _request[i].substr(0, _request[i].find(" ", 0));
			unsigned int first = _requestData.methode.size() + 1;
			unsigned int last = _request[i].find(" ", first);
			_requestData.path = _request[i].substr(first, last - first);
			_requestData.protocol = _request[i].substr(last + 1, _request[i].size());
		}
	}
}

int ParsingRequest::foundFileToSend()
{
	_requestData.fileToSend = _requestData.path ;
	if (_requestData.fileToSend[0] == '/')
		_requestData.fileToSend.erase(0, 1);

	_requestData.isIndex = 0;
	
	if (_autoindex == 1 && isDirectory(_path + _requestData.fileToSend) && !fileExist(_path + _requestData.fileToSend + "index.html"))
	{
		_requestData.fileToSend = _path + _requestData.fileToSend;
		_requestData.isIndex = 1;
	}
	else if (isDirectory(_path + _requestData.fileToSend))
		_requestData.fileToSend = _path + "index.html";
	else
		_requestData.fileToSend = _path + _requestData.fileToSend;

	FILE *f = fopen(_requestData.fileToSend.c_str(), "r+");
	if ( f == NULL)
	{
		if (_autoindex == 1 && errno == 21)
			return (1);
		_requestData.fileToSend = _path + "404.html";
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