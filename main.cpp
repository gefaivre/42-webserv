#include "Server.hpp"
#include "engine/ParsingRequest.hpp"
#include "engine/CreateResponse.hpp"
#include "Parser.hpp"
#include <sys/epoll.h>

void epolling(Server Server);

int main(int argc, char **argv)
{
	if ( argc != 2 )
	{
		cerr << "Usage: ./webserv [config file]" << endl;
		return (1);
	}

	Parser *config = new Parser(argv[1]);

	vector<Server *> servers = config->getServers();

	servers[0]->setSocket();

	epolling(*servers[0]);

	return (0);
}