#include "CreateResponse.hpp"


/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

CreateResponse::CreateResponse(Server *server,std::map<std::string, std::string> &requestMap, t_requestData const requestData):
	_server(server), _requestData(requestData), _requestMap(requestMap)
{

	fillFilesExtension();
	_createBody = false;
	_fillHeaderData = false;
	_createHeader = false;
	_firstTimeBody = true;
	_bodyIsRead = false;
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
	// _header.clear();
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



void CreateResponse::collectData(int newsocket)
{
	(void)newsocket;
	
}

void CreateResponse::fillFilesExtension()
{
	_switchFilesExtension.insert( std::pair<std::string, std::string>("html","text/html"));
	_switchFilesExtension.insert(std::pair<std::string, std::string>("ico","image/x-icon"));
	_switchFilesExtension.insert(std::pair<std::string, std::string>("jpg","image/png"));
	_switchFilesExtension.insert(std::pair<std::string, std::string>("css","text/css"));
	_switchFilesExtension.insert(std::pair<std::string, std::string>("default","text/html") );
}

int CreateResponse::checkErrorPage(std::string errorCode, std::string path)
{
	if ((_requestData.fileToSend == path + errorCode) || (_requestData.fileToSend == ft_pwd() + "/error_pages/" + errorCode))
		return (1);
	return (0);
}

void CreateResponse::errorStatus()
{

	std::string path = _server->getLocationByPath("/" + _requestData.path).getRoot();
	if (path + _requestData.path != _requestData.fileToSend)
	{
		if (_headerData.isRedirection)
		{
			_headerData.statusCode = "301";
			_headerData.statusMessage = "Moved permanently";
		}
		else if (checkErrorPage("400.html", path))
		{
			_headerData.statusCode = "400";
			_headerData.statusMessage = "Bad Request";
		}
		else if (checkErrorPage("403.html", path))
		{
			_headerData.statusCode = "403";
			_headerData.statusMessage = "Forbidden";
		}
		else if (checkErrorPage("404.html", path))
		{
			_headerData.statusCode = "404";
			_headerData.statusMessage = "Not Found";
		}
		else if (checkErrorPage("405.html", path))
		{
			_headerData.statusCode = "405";
			_headerData.statusMessage = "Not Allowed";
		}
		else if (checkErrorPage("500.html", path))
		{
			_headerData.statusCode = "500";
			_headerData.statusMessage = "Internal Server Error";
		}
		else if (checkErrorPage("404_noSpecified.html", path))
		{
			_headerData.statusCode = "404";
			_headerData.statusMessage = "Not Found";
		}
		else if (checkErrorPage("202.html", path))
		{
			_headerData.statusCode = "202";
			_headerData.statusMessage = "Accepted";
		}
		else if (checkErrorPage("413.html", path))
		{
			_headerData.statusCode = "413";
			_headerData.statusMessage = "Request Entity Too Large";
		}
	}
	else
	{
		_headerData.statusCode = "200";
		_headerData.statusMessage = "OK";
	}
}

void CreateResponse::fillHeaderData()
{
	
	_headerData.protocol = _requestData.protocol;
	_headerData.isRedirection = _requestData.isRedirection;
	errorStatus();
	std::string type = _requestData.fileToSend.substr(_requestData.fileToSend.find('.') + 1, _requestData.fileToSend.size());
	_headerData.contentType = _switchFilesExtension[type];
	if (_headerData.contentType.size() == 0)
		_headerData.contentType = _switchFilesExtension["default"];

	if (_requestMap[std::string("Connection")] == std::string("keep-alive"))
		_headerData.connection = std::string("keep-alive");
	else
		_headerData.connection = std::string("close");

	_headerData.contentLength = itos(_body.size());
}

void CreateResponse::createHeader()
{
	if (_headerData.isRedirection)
	{
		_header += _headerData.protocol;
		_header += " ";
		_header += _headerData.statusCode;
		_header += " ";
		_header += _headerData.statusMessage + "\r\n";
		_header += "Location: " + _requestData.fileToSend + "\r\n";
		_header += "Content-length: 0\r\n";

	}
	else
	{
		_header += _headerData.protocol;
		_header += " ";
		_header += _headerData.statusCode;
		_header += " ";
		_header += _headerData.statusMessage + "\r\n";
		_header += "Content-length:  " + _headerData.contentLength + "\r\n";
		_header += "Content-Type:  " + _headerData.contentType + "\r\n";
		_header += "Connection: " + _headerData.connection + "\r\n";
	}
}


int CreateResponse::create()
{
	int ret;

	if (_createBody == false)
	{
		ret = createBody();
		if (ret == READ)
			_createBody = true;
		return NOT_READ_YET;
	}
	else if (_fillHeaderData == false)
	{
		fillHeaderData();
		_fillHeaderData = true;
		return NOT_READ_YET;
	}
	else if (_createHeader == false)
	{
		createHeader();
		_createHeader = true;
		return READ;
	}
	else
		return ERROR;

}


int CreateResponse::createBody()
{
	int ret = 0;
	if (_requestData.isRedirection)
		return READ;
	else if (_requestData.isIndex)
		ret = BodyIsIndex();
	else if (_requestData.isCgi)
		ret = BodyIsCgi();
	else
		ret = BodyIsNotIndex();
	return ret;
}

int CreateResponse::BodyIsCgi()
{
	_body = _requestData._cgiResponse;
	return READ;
}

int CreateResponse::BodyIsNotIndex()
{
	if (_firstTimeBody == true)
	{
		_myfile.open(_requestData.fileToSend.c_str(), std::ifstream::in);
		_firstTimeBody = false;
		return NOT_READ_YET;
	}
	else if (_myfile && _bodyIsRead == false)
	{
		char buf[READING_BUFFER];
		bzero(buf, READING_BUFFER);
		
		_myfile.read(buf, READING_BUFFER - 1);
		_body +=  std::string(buf, _myfile.gcount());
		if (!_myfile)
		{
			_myfile.close();
			_bodyIsRead = true;
			return READ;
		}
		return NOT_READ_YET;
	}
	return ERROR;
}

int CreateResponse::BodyIsIndex()
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
	return READ;
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

std::string CreateResponse::getHeaderResponse() const
{
	return(_header);
}
std::string CreateResponse::getResponse() const
{
	return(_header + "\r\n" + _body);
}

std::string CreateResponse::getBodyResponse() const
{
	return(_body);
}



/* ************************************************************************** */