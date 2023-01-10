#include "Server.hpp"
#include "engine/ParsingRequest.hpp"
#include "engine/CreateResponse.hpp"
#include "Parser.hpp"
#include <sys/epoll.h>

void epolling(vector<Server *> servers);

int main(int argc, char **argv)
{
	if ( argc != 2 )
	{
		cerr << "Usage: ./webserv [config file]" << endl;
		return (1);
	}

	Parser *config = new Parser(argv[1]);

	vector<Server *> servers = config->getServers();


	epolling(servers);

	return (0);
}