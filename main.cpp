#include "engine/Socket.hpp"
#include "engine/ParsingRequest.hpp"
#include "engine/CreateResponse.hpp"

int main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cerr << "Usage: ./webserv [port]" << std::endl;
		return (0);
	}

	// Parser *config = new Parser(av[1]);

	// (void) config;



	std::string path("www/");
	bool autoindex = 1;


	Socket socket(atoi(av[1]));

	for(;;)
	{
		socket.waitRequest();
		socket.displayRequest();

		ParsingRequest parsingRequest(path, autoindex, socket.getRequest());

		CreateResponse createResponse(path, autoindex, parsingRequest.getData());

		socket.sendResponse(createResponse.getResponse());
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