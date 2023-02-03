#include "Server.hpp"
#include "engine/ParsingRequest.hpp"
#include "engine/CreateResponse.hpp"
#include "Parser.hpp"
#include <sys/epoll.h>
#include <signal.h>

Parser *g_servers;

void epolling(vector<Server *> servers);

void sig_handler(int signo)
{
	if (signo == SIGINT)
	{

    	// std::cout << "SIGINT" << (*g_servers)[0]->getPort() << std::endl;

		// for (size_t i = 0; i < (*g_servers).size(); i++)
		// {
		// 	// std::cout << &(*g_servers)[i] << std::endl;
		// 	std::map<std::string, Location *>::iterator	it;
		
		// 	// for (it = (*g_servers)[i]->_locations.begin(); it != (*g_servers)[i]->_locations.end(); it++)
		// 	// {
				
		// 	// 	delete it->second->_wasSet;
		// 	// }
		// 	delete (*g_servers)[i];
		// 	// delete (*g_servers)[i]->_wasSet;
		// }
		delete g_servers;
		
	}
	
	exit(0);
}

int main(int argc, char **argv)
{
	if ( argc != 2 )
	{
		cerr << "Usage: ./webserv [config file]" << endl;
		return (1);
	}

	Parser *config = new Parser(argv[1]);
g_servers = config;
	vector<Server *> servers = config->getServers();
	
	

	if (signal(SIGINT, sig_handler) == SIG_ERR)
 		std::cout << "can't catch SIGINT" << std::endl;

	// while (1)
	// 	;
	
	epolling(servers);
	
	// delete config;
	return (0);
}