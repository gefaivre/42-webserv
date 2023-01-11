#include "CreateResponse.hpp"


/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

CreateResponse::CreateResponse(Server *server, t_requestData const requestData):
	_server(server), _requestData(requestData)
{
	std::cout << "CHO = " << _requestData._cgiResponse << std::endl;
	fillHeaderData();
	// if (cgi)
	// {}
	// else
	// {}
	createBody();
	createHeader();
	joinHeaderBody();
}




/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

CreateResponse::~CreateResponse()
{
	// delete [] _headerData.protocol;
	// _headerData.protocol.delete();
	// _headerData.statusCode.clear();
	// _headerData.statusMessage.clear();
	// _headerData.contentLength.clear();
	_headerData.contentType.erase();
	_header.clear();
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/


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
	_switchFilesExtension.insert(std::pair<std::string, std::string>("jpg","image/png"));
	_switchFilesExtension.insert(std::pair<std::string, std::string>("css","text/css"));
	_switchFilesExtension.insert(std::pair<std::string, std::string>("default","text/html") );
}

void CreateResponse::fillHeaderData()
{
	fillFilesExtension();
	std::string path = _server->getLocationByPath("/" + _requestData.path).getRoot();
	
	_headerData.protocol = _requestData.protocol;
	if ((_requestData.fileToSend == path + "404.html") && (path + _requestData.path != _requestData.fileToSend))
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
	// std::time_t time_now = std::time(0);
	// _headerData.date = time_now;
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
	
	// _header += " ";
	// _header += _headerData.date ;
	// _header += "Content-Length: " + _headerData.contentLength + "\r\n";
}

void CreateResponse::BodyIsCgi()
{
	std::cout << "***" << std::endl;
	_body = _requestData._cgiResponse;
}

void CreateResponse::createBody()
{
	std::cout << "IS index = " << _requestData.isIndex << std::endl;
	std::cout << "IS CGI = " << _requestData.isCgi << std::endl;
	if (_requestData.isIndex)
		BodyIsIndex();
	else if (_requestData.isCgi)
		BodyIsCgi();
	else
		BodyIsNotIndex();
}

void CreateResponse::BodyIsNotIndex()
{
	_header += "Content-Type: " + _headerData.contentType;
	_header += "\r\n";
	// _header += "Connection: closed";
	// _header += "\r\n";

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
	_body += "<hr>\n";

	_body += ("<a href= \"" + std::string("../") + "\">" + std::string("../") + "</a></br>\n\r");

	DIR *dir;
	struct dirent *ent;

	dir = opendir (_requestData.fileToSend.c_str());
	if (dir == NULL)
		ft_define_error("Error while opening the directory");
	while ((ent = readdir (dir)) != NULL)
	{
		std::string file(ent->d_name);
		if (file != ".." && file != ".")
		{
			if (isDirectory(_requestData.fileToSend + file))
				file += "/";
			
			_body += ("<a href= \"" + file + "\">" + file + "</a></br>\n\r");
		}
	}
	if (closedir(dir) == -1)
		ft_define_error("Error while closing the directory");
	_body += "</h1>\n";
	_body += "<hr>\n<p>Webserv/1.0</p>\n</body>";
	_body += "\n\r";
}

void CreateResponse::joinHeaderBody()
{
	_header += "Content-Length: " + itos(_body.size());
	_header += "\r\n";

	_response = _header + "\r\n" + _body;
}

void CreateResponse::displayHeaderResponse() const
{
	std::cout << _header << std::endl;
}

void CreateResponse::displayFullResponse() const
{
	std::cout << _header << std::endl;
	std::cout << _body << std::endl;
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/

std::string CreateResponse::getResponse() const
{
	return(_response);
}



/* ************************************************************************** */