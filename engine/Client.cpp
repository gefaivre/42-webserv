#include "Client.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Client::Client(Server *server, int clientfd):
    _clientfd(clientfd), _server(server)
{

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

void Client::readRequest()
{
	char a[1] = {0};
	std::string buf;
	_request.clear();
	ssize_t tmp_recv;
	int size = -1;
	int tmp_switch = 0;
	bool method_get = false;
	while ((tmp_recv = recv(_clientfd, a, 1, 0))) //add -1 to handle errors (This call returns the number of bytes read into the buffer, otherwise it will return -1 on error.)
	{
		if (tmp_recv == -1)
			ft_define_error("Error with the message from a socket");
		buf += a[0];
		if (a[0] == '\n' || size == 0)
		{
			//find the size of the body
			const char *c = strstr(buf.c_str(), "Content-Length: ");
			if (c && size == -1)
			{
				char **len_split = ft_split(c);
				size = atoi(len_split[1]);
			}
			//find the method
			if (method_get == false && strstr(buf.c_str(), "GET"))
				method_get = true;
			if (a[0] == '\n')
				buf.erase(buf.find("\r\n") ,buf.size());
			_request.push_back(buf);
			buf.clear();
			if (_request[_request.size() - 1].size() == 0 || size == 0)
			{
				if (tmp_switch == 1 || method_get)
				{
					tmp_switch = 0;
					break;
				}
				tmp_switch = 1;
			}
		}
		if (size > 0 && tmp_switch == 1)
			size--;
	}
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

/*
** --------------------------------- ACCESSOR ---------------------------------
*/
std::vector<std::string> Client::getRequest() const
{
	return (_request);
}

/* ************************************************************************** */