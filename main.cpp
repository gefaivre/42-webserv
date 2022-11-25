#include "Socket.hpp"
#include "Parser.hpp"
#include "Server.hpp"

int main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cerr << "Usage: ./webserv [port]" << std::endl;
		return (0);
	}

	// Parser *config = new Parser(av[1]);

	// (void) config;

	Socket socket(atoi(av[1]));

	for(size_t i = 0;; i++)
	{
		socket.waitAndParseRequest();
		// socket.displayRequest();
		socket.createAndSendResponse();
	}
}

// int main_exemple(int ac, char **av)
// {
// 	if (ac != 2)
// 	{
// 		std::cerr << "Usage: ./webserv [port]" << std::endl;
// 		return (0);
// 	}

// 	Parser *config = new Parser(av[1]);

// 	Socket socket(config);

// 	Parsing parsing;

// 	parsing = socket.waitRequest;

// 	Engine engine(parsing);

// 	socket.sendResponse(engine);

// }