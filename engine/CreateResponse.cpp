#include "CreateResponse.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

CreateResponse::CreateResponse(std::string path, bool autoindex, t_requestData requestData):
_path(path), _autoindex(autoindex), _requestData(requestData)
{
	fillHeaderData();

	createHeader();

	createBody();

	joinHeaderBody();
}

// CreateResponse::CreateResponse( const CreateResponse & src )
// {
// }


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

CreateResponse::~CreateResponse()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

// CreateResponse &				CreateResponse::operator=( CreateResponse const & rhs )
// {
// 	if ( this != &rhs )
// 	{
// 		this->_value = rhs.getValue();
// 	}
// 	return *this;
// }

// std::ostream &			operator<<( std::ostream & o, CreateResponse const & i )
// {
// 	o << "Value = " << i.getValue();
// 	return o;
// }


/*
** --------------------------------- METHODS ----------------------------------
*/

void CreateResponse::fillFilesExtension()
{
	_switchFilesExtension.insert( std::pair<std::string, std::string>("html","text/html"));
	_switchFilesExtension.insert(std::pair<std::string, std::string>("ico","image/x-icon"));
	_switchFilesExtension.insert(std::pair<std::string, std::string>("default","text/html") );
}

void CreateResponse::fillHeaderData()
{
	fillFilesExtension();
	_headerData.protocol = _requestData.protocol;
	if (_requestData.fileToSend == _path + "404.html")
	{
		_headerData.statusCode = "404";
		_headerData.statusMessage = "Not Found";

	}
	else
	{
		_headerData.statusCode = "200";
		_headerData.statusMessage = "OK";
	}
	std::string type = _requestData.fileToSend.substr(_requestData.fileToSend.find('.') + 1, _requestData.fileToSend.size());
	_headerData.contentType = _switchFilesExtension[type];
	if (_headerData.contentType.size() == 0)
		_headerData.contentType = _switchFilesExtension["default"];

	std::string file;
	std::string line;
	std::ifstream myfile;

	myfile.open(_requestData.fileToSend.c_str(), std::ifstream::in);
	while ( std::getline(myfile,line) )
	{
		if (file.size() != 0)
			file += '\n';
		file += line;
	}
	myfile.close();

	std::stringstream oui;
	oui << file.size();
	oui >> _headerData.contentLength;
}

void CreateResponse::createHeader()
{
	_header.clear();
	_header += _headerData.protocol;
	_header += " ";
	_header += _headerData.statusCode;
	_header += " ";
	_header += _headerData.statusMessage + "\r\n";
	// _header += "Content-Length: " + _headerData.contentLength + "\r\n";
}

void CreateResponse::createBody()
{
	if (_requestData.isIndex)
		BodyIsIndex();
	else
		BodyIsNotIndex();
}

void CreateResponse::BodyIsNotIndex()
{


	_header += "Content-Type: text/html";
	_header += "\r\n";

	std::string file;
	std::string line;
	std::ifstream myfile;

	myfile.open(_requestData.fileToSend.c_str(), std::ifstream::in);
	while ( std::getline(myfile,line) )
	{
		if (file.size() != 0)
			file += '\n';
		file += line;
	}
	myfile.close();

	_body += file;
}


void CreateResponse::BodyIsIndex()
{
	_body += "<!DOCTYPE html>\n<html>\n<head>\n<title> Index of ";
	_body += _requestData.path;
	_body += "</title>\n</head>";
	_body += "<body>\n<h1>Index of ";
	_body += _requestData.path;
	_body += "</h1>\n";

	DIR *dir;
	struct dirent *ent;

	dir = opendir (_requestData.fileToSend.c_str());
	while ((ent = readdir (dir)) != NULL)
	{
		std::string file(ent->d_name);
		if (isDirectory(_requestData.fileToSend + file))
			file += "/";
		
		_body += ("<a href= \"" + file + "\">" + file + "</a></br>\n\r");
	}
	closedir (dir);

	_body += "</h1>\n";
	_body += "<hr>\n<p>Webserv/1.0</p>\n</body>";
	_body += "\n\r";
}

void CreateResponse::joinHeaderBody()
{
	_response = _header + "\r\n" + _body;
}



/*
** --------------------------------- ACCESSOR ---------------------------------
*/

std::string CreateResponse::getResponse() const
{
	return(_response);
}



/* ************************************************************************** */