/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   epolling.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gefaivre <gefaivre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 12:42:44 by gefaivre          #+#    #+#             */
/*   Updated: 2023/01/03 16:13:08 by gefaivre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "ParsingRequest.hpp"
#include "CreateResponse.hpp"
#include "Parser.hpp"
#include <sys/epoll.h>


#define MAX_EVENTS 5
#define READ_SIZE 10

void epolling(Socket socket, Server1 *server)
{
	(void)server; 
	
	int running = 1, event_count, i;
	// char read_buffer[READ_SIZE + 1];
	struct epoll_event event, events[MAX_EVENTS];
	std::map<int, char[50000]> buf;


	int epoll_fd = epoll_create1(0);
	if (epoll_fd == -1)
	{
		fprintf(stderr, "Failed to create epoll file descriptor\n");
		return;
	}

	event.events = EPOLLIN;
	event.data.fd = socket.getSocketFd();
	
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket.getSocketFd(), &event))
	{
		fprintf(stderr, "Failed to add file descriptor to epoll\n");
		close(epoll_fd);
		return;
	}
	
	while (running)
	{
		event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);

		for (i = 0; i < event_count; i++)
		{

			if (events[i].data.fd == socket.getSocketFd())
			{
				int newsocket;
				newsocket = accept(socket.getSocketFd(), 0, 0);
				if (newsocket == -1)
					ft_define_error("Error the connection with the socket was not established");
				event.events = EPOLLIN;
				event.data.fd = newsocket;
				epoll_ctl(epoll_fd, EPOLL_CTL_ADD, newsocket, &event);
				std::cout << "new client = " << newsocket << std::endl;
			}
			else
			{
				if (events[i].events == EPOLLIN)
				{
					char buf2[50000];
					ssize_t ret = recv(events[i].data.fd, buf2, 49999, 0);
					buf[events[i].data.fd] = buf2;
					buf[events[i].data.fd][ret] = '\0';
					std::cout << buf[events[i].data.fd] << std::endl;
					event.events = EPOLLOUT;
					event.data.fd = events[i].data.fd;
					epoll_ctl(epoll_fd, EPOLL_CTL_MOD, events[i].data.fd, &event);
				}
				else if (events[i].events == EPOLLOUT)
				{
					char buf2[] = "HTTP/1.1 200 OK\n\rDate: Mon, 27 Jul 2009 12:28:53 GMT\n\rServer: Apache/2.2.14 (Win32)\n\rLast-Modified: Wed, 22 Jul 2009 19:15:56 GMT\n\rContent-Type: text/html\n\rConnection: Closed\n\r\n\r<html>\n<body>\n<h1>Hello, World!</h1>\n</body>\n</html>";
					send(events[i].data.fd, buf2, strlen(buf2), 0);

				}
					
			}
			bzero(events, MAX_EVENTS);

			
			// socket.waitRequest();
			// socket.displayRequest();

			// ParsingRequest parsingRequest(socket.getRequest(), server);
			// parsingRequest.displayData();

			// CreateResponse createResponse("www/", true, parsingRequest.getData());
			// createResponse.displayHeaderResponse();


			// socket.sendResponse(createResponse.getResponse());
		
			// if(!strncmp(read_buffer, "stop\n", 5))
			// running = 0;
		}
	}

	if (close(epoll_fd))
	{
		fprintf(stderr, "Failed to close epoll file descriptor\n");
		return;
	}
}
