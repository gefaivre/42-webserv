#include "CreateResponse.hpp"


/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

CreateResponse::CreateResponse(Server *server,std::map<std::string, std::string> &requestMap, t_requestData const requestData):
	_server(server), _requestData(requestData), _requestMap(requestMap)
{
	fillFilesExtension();
	createBody();
	fillHeaderData();
	createHeader();

	if (_requestData.methode == "POST")
	{
		// std::cout << "new socket = " << newsocket << std::endl;
		// collectData(newsocket);
	}
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


void CreateResponse::collectData(int newsocket)
{
	(void)newsocket;
	std::cout << "post method!" << std::endl;
	
}

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

	if (_requestMap[std::string("Connection")] == std::string("keep-alive"))
		_headerData.connection = std::string("keep-alive");
	else
		_headerData.connection = std::string("close");

	_headerData.contentLength = itos(_body.size());
}

void CreateResponse::createHeader()
{
	_header.clear();
	_header += _headerData.protocol;
	_header += " ";
	_header += _headerData.statusCode;
	_header += " ";
	_header += _headerData.statusMessage + "\r\n";
	_header += "Content-length:  " + _headerData.contentLength + "\r\n";
	_header += "Content-Type:  " + _headerData.contentType + "\r\n";
	_header += "Connection: " + _headerData.connection + "\r\n";
	std::cout << "TYPE = " << _headerData.contentType<< std::endl;
}

void CreateResponse::BodyIsCgi()
{
	_body = _requestData._cgiResponse;
}

void CreateResponse::createBody()
{
	if (_requestData.isIndex)
		BodyIsIndex();
	else if (_requestData.isCgi)
		BodyIsCgi();
	else
		BodyIsNotIndex();
}

#include <ctime>

void CreateResponse::BodyIsNotIndex()
{

	time_t current_time;
  	current_time = time(NULL);
	
	std::string line;
	std::ifstream myfile;

	myfile.open(_requestData.fileToSend.c_str(), std::ifstream::in);
	while ( std::getline(myfile,line) )
	{
		if (_body.size() != 0)
			_body += '\n';
		_body += line;
	}
	myfile.close();



    // std::ifstream file(_requestData.fileToSend.c_str(), std::ifstream::in);
	// if (file.good())
	// {
	// 	_body.insert(_body.begin(), std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
	// 	file.close();
	// }
	// else
	// 	std::cout << "can' t open file in create body" << std::endl;

	std::cout << "Seconde for read file = " << BYEL << time(NULL) - current_time << reset << std::endl;
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