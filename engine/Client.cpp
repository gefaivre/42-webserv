#include "Client.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Client::Client(Server *server, int clientfd):
    _clientfd(clientfd), _server(server)
{
	headerIsRead = 0;
}

// Client::Client( const Client & src )
// {
// }


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Client::~Client()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

// Client &				Client::operator=( Client const & rhs )
// {
// 	//if ( this != &rhs )
// 	//{
// 		//this->_value = rhs.getValue();
// 	//}
// 	return *this;
// }

// std::ostream &			operator<<( std::ostream & o, Client const & i )
// {
// 	//o << "Value = " << i.getValue();
// 	return o;
// }


/*
** --------------------------------- METHODS ----------------------------------
*/
int	ft_find_content_lenght(std::map<std::string, std::string> _requestmap)
{
	std::map<std::string,std::string>::iterator it;
	it = _requestmap.find("Content-Length");
	int content_lenght = 0;
	if (it != _requestmap.end())
	{
		content_lenght = std::atoi(it->second.c_str());

		// while ((tmp_recv = recv(_newsocket, &a, 1, 0)) && content_lenght--) //add -1 to handle errors (This call returns the number of bytes read into the buffer, otherwise it will return -1 on error.)
		// {
		// 	std::cout << a;
		// }
	}
	return (content_lenght);
}

std::string	ft_find_boundary(std::map<std::string, std::string> _requestmap, int content_lenght)
{
	std::string boundary;
	size_t pos_equal = 0;
	std::map<std::string,std::string>::iterator it;
	if (content_lenght > 0)
	{
		it = _requestmap.find("Content-Type");
		if (it != _requestmap.end())
		{
			pos_equal = it->second.find_last_of('=');
			std::cout << "equal = " << pos_equal << std::endl;
		}
	}
	return (boundary);
}


void Client::parseHeader(std::string buf)
{
	_request = ft_split_header(buf);
	for (size_t i = 0; i < _request.size(); i++)
	{
		size_t colon = _request[i].find(": ");
		if (colon != std::string::npos)
		{
			std::string key = _request[i].substr(0, colon);
			std::string value = _request[i].substr(colon + 2, _request.size());
			_requestmap.insert(std::pair<std::string, std::string>(key, value));
		}
	}
	int content_lenght = ft_find_content_lenght(_requestmap);
	std::string boundary = ft_find_boundary(_requestmap, content_lenght);
	
}

void Client::readRequest()
{
	char a = 0;
	std::string buf;
	_request.clear();
	// size_t last_newline;
	ssize_t tmp_recv;
	while ((tmp_recv = recv(_clientfd, &a, 1, 0))) //add -1 to handle errors (This call returns the number of bytes read into the buffer, otherwise it will return -1 on error.)
	{
		if (tmp_recv == -1)
			ft_define_error("Error with the message from a socket");
		buf += a;
			if (buf.find("\r\n\r\n") != std::string::npos)
			{
				std::cout << "hello" << std::endl;
				break;
			}
	}
	parseHeader(buf);

}


void Client::readRequest1()
{

	char buf[50];
	int sizeRead;
	struct epoll_event event;

	bzero(buf, 50);

	if (headerIsRead == 0)
	{
		sizeRead = recv(_clientfd, buf, 49, 0);
		if (sizeRead == -1)
			std::cout << "Error recv" << std::endl;
		_requestLine += std::string(buf);
		
		while (_requestLine.find("\r\n") != std::string::npos)
		{
			std::string line = _requestLine.substr(0, _requestLine.find("\r\n"));
			_requestLine.erase(0, _requestLine.find("\r\n") + 2);
			_request.push_back(line);

		}
		if ((_request[_request.size() - 1].size() == 0))
		{
			event.events = EPOLLOUT;
			event.data.fd = _clientfd;
			epoll_ctl(_server->getEpollFd(), EPOLL_CTL_MOD, _clientfd, &event);
			headerIsRead = 1;
		}
		// if ("\r\n\r\n")
		// 	headerIsRead = 1;
	}

	
	// else if (parseHeader(buf) != 0)
	// {

	// }

}

void Client::createResponse()
{
    ParsingRequest parsingRequest(_request, _server);
	CreateResponse createResponse("www/", true, parsingRequest.getData());
	createResponse.displayHeaderResponse();
    _response = createResponse.getResponse();
}

void Client::sendResponse()
{
	if (send(_clientfd, _response.c_str(), _response.size(), 0) == -1)
		ft_define_error("Send error");
	if (close(_clientfd) == -1)
		ft_define_error("Close error");
}

void Client::displayRequest()
{
	std::cout << "\n" << "\033[33m" << _request[0] << "\033[0m" << std::endl;
}

void Client::displayFullRequest()
{
	std::cout << std::endl;
	for(size_t i = 0; i < _request.size(); i++)
		std::cout << "\033[33m" << _request[i] << "\033[0m" << std::endl;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/
std::vector<std::string> Client::getRequest() const
{
	return (_request);
}

/* ************************************************************************** */