#include "engine/Socket.hpp"
#include "engine/ParsingRequest.hpp"
#include "engine/CreateResponse.hpp"

#include <sys/epoll.h>


// int main(int ac, char **av)
// {
// 	if (ac != 2)
// 	{
// 		std::cerr << "Usage: ./webserv [port]" << std::endl;
// 		return (0);
// 	}
	
// 	std::string path("www/");
// 	bool autoindex = 1;



// 	Socket socket(atoi(av[1]));

// 	for(;;)
// 	{
// 		socket.waitRequest();
// 		// socket.displayRequest();

// 		ParsingRequest parsingRequest(path, autoindex, socket.getRequest());

// 		CreateResponse createResponse(path, autoindex, parsingRequest.getData());

// 		socket.sendResponse(createResponse.getResponse());
// 	}
// }

#define MAX_EVENTS 5
#define READ_SIZE 10


void epolling(Socket socket)
{
	int running = 1, event_count, i;
	char read_buffer[READ_SIZE + 1];
	struct epoll_event event, events[MAX_EVENTS];

	std::string path("www/");
	bool autoindex = 1;

	int epoll_fd = epoll_create1(0);
	if (epoll_fd == -1)
	{
		fprintf(stderr, "Failed to create epoll file descriptor\n");
		return;
	}

	event.events = EPOLLIN | EPOLLET;
	event.data.fd = socket.getSocketFd();
	
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket.getSocketFd(), &event))
	{
		fprintf(stderr, "Failed to add file descriptor to epoll\n");
		close(epoll_fd);
		return;
	}
	
	while (running)
	{
		printf("\nPolling for input...\n");
		event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, 30000);
		printf("%d ready events\n", event_count);


		for (i = 0; i < event_count; i++)
		{
			socket.waitRequest();
			// socket.displayRequest();

			ParsingRequest parsingRequest(path, autoindex, socket.getRequest());

			CreateResponse createResponse(path, autoindex, parsingRequest.getData());

			sleep(2);

			socket.sendResponse(createResponse.getResponse());
		
			if(!strncmp(read_buffer, "stop\n", 5))
			running = 0;
		}
	}



	if (close(epoll_fd))
	{
		fprintf(stderr, "Failed to close epoll file descriptor\n");
		return;
	}
}



int main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cerr << "Usage: ./webserv [port]" << std::endl;
		return (0);
	}

	Socket socket(atoi(av[1]));
	epolling(socket);

	return (0);

}