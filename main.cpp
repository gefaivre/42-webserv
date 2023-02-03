#include "Server.hpp"
#include "engine/ParsingRequest.hpp"
#include "engine/CreateResponse.hpp"
#include "Parser.hpp"
#include <sys/epoll.h>
#include <signal.h>

void epolling(vector<Server *> servers);

void sig_handler(int signo)
{
	if (signo == SIGINT)
    std::cout << "SIGINT" << std::endl;
	exit(0);
}

int main(int argc, char **argv)
{
	if ( argc != 2 )
	{
		cerr << "Usage: ./webserv [config file]" << endl;
		return (1);
	}
	  if (signal(SIGINT, sig_handler) == SIG_ERR)
 		std::cout << "can't catch SIGINT" << std::endl;

	Parser *config = new Parser(argv[1]);

	vector<Server *> servers = config->getServers();


	epolling(servers);

	return (0);
}