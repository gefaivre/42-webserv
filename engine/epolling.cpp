/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   epolling.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gefaivre <gefaivre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 12:42:44 by gefaivre          #+#    #+#             */
/*   Updated: 2023/01/04 10:55:24 by gefaivre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "ParsingRequest.hpp"
#include "CreateResponse.hpp"
#include "Parser.hpp"
#include <sys/epoll.h>


#define MAX_EVENTS 5
#define READ_SIZE 10

void epolling(Server server, Server1 *serverconf)
{ 
	int running = 1, event_count, i;
	// char read_buffer[READ_SIZE + 1];
	struct epoll_event event, events[MAX_EVENTS];


	int epoll_fd = epoll_create1(0);
	if (epoll_fd == -1)
	{
		fprintf(stderr, "Failed to create epoll file descriptor\n");
		return;
	}

	event.events = EPOLLIN;
	event.data.fd = server.getServerFd();
	
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server.getServerFd(), &event))
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

			if (events[i].data.fd == server.getServerFd())
			{
				server.newclient(epoll_fd);
			}
			else
			{
				if (events[i].events == EPOLLIN)
				{
					Client *client = new Client(events[i].data.fd);
					server.clients.insert(std::pair<int, Client*>(events[i].data.fd, client));
					server.clients[events[i].data.fd]->readRequest();
					event.events = EPOLLOUT;
					event.data.fd = events[i].data.fd;
					epoll_ctl(epoll_fd, EPOLL_CTL_MOD, events[i].data.fd, &event);
					server.clients[events[i].data.fd]->displayRequest();
				}
				else if (events[i].events == EPOLLOUT)
				{
					ParsingRequest parsingRequest(server.clients[events[i].data.fd]->getRequest(), serverconf);
					CreateResponse createResponse("www/", true, parsingRequest.getData());
					createResponse.displayHeaderResponse();

					server.clients[events[i].data.fd]->sendResponse(createResponse.getResponse());
					
					epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, &event);
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
