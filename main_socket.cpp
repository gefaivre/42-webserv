#include "Socket.hpp"

int main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cout << "Usage: ./webserv [port]" << std::endl;
		return (0);
	}

	Socket socket(atoi(av[1]));

	for(size_t i = 0;; i++)
	{
		socket.waitAndParseRequest();
		// socket.displayRequest();
		socket.createAndSendResponse();
	}
}