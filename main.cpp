#include "engine/Socket.hpp"
#include "engine/ParsingRequest.hpp"
#include "engine/CreateResponse.hpp"
#include "Parser.hpp"
#include "Server1.hpp"
#include <sys/epoll.h>

void epolling(Socket socket, Server1 *server);

int main(int argc, char **argv)
{
	if ( argc != 2 )
	{
		cerr << "Usage: ./webserv [config file]" << endl;
		return (1);
	}

	Parser *config = new Parser(argv[1]);

	vector<Server1 *> servers = config->getServers();

	Socket socket(servers[0]->getPort());

	epolling(socket, servers[0]);

	return (0);
}