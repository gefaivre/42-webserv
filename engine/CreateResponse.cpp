#include "CreateResponse.hpp"


/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

CreateResponse::CreateResponse(Server *server, t_requestData const requestData):
	_server(server), _requestData(requestData)
{
	fillHeaderData();
	createHeader();
	createBody();
	joinHeaderBody();
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
	// int cgiPipe[2];
	// int nReponse = 0;
	// if (pipe(cgiPipe))
	// 	perror("error from pipe cgi");
			

	// int cgiPid = fork();


	// if (cgiPid == 0)
	// {
	// 	//child
	// 	char **cgienvs = ft_split((char *)"last_name=jbach GATEWAY_INTERFACE=CGI/1.1 PATH_INFO=/website/sendDatas/data.php REQUEST_METHOD=POST SCRIPT_FILENAME=./website/sendDatas/data.php SERVER_PROTOCOL=HTTP/1.1 REDIRECT_STATUS=200 CONTENT_TYPE=application/x-www-form-urlencoded CONTENT_LENGTH=11 /usr/bin/php-cgi");
	// 	char *args[]= {(char *)"/usr/bin/php-cgi", NULL};
	// 	close(cgiPipe[1]);
	// 	dup2(cgiPipe[0], newsocket);
	// 	dup2(nReponse, newsocket);
	// 	if (execve(args[0], args, cgienvs))
	// 	{
	// 		/* unable to execute CGI... */
    // 		perror("error execve cgi");
	// 	// sendString(nRemote,
	// 	//     "HTTP/1.1 200 OK\r\n"
	// 	//     "Content-length: 97\r\n"
	// 	//     "Content-Type: text/html\r\n\r\n"
	// 	//     "<!doctype html><html><head><title>CGI Error</title></head><body><h1>CGI Error.</h1></body></html>\r\n"
	// 		// return ;
	// 	}
	// }
	// else if (cgiPid > 0)
	// {
	// 	//parent
	// 	close(cgiPid[0]);

	// }

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

void CreateResponse::createBody()
{
	if (_requestData.isIndex)
		BodyIsIndex();
	else
		BodyIsNotIndex();
}

void CreateResponse::BodyIsNotIndex()
{
	_header += "Content-Type: " + _headerData.contentType;
	_header += "\r\n";
	_header += "Connection: closed";
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