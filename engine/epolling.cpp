/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   epolling.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbach <jbach@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 12:42:44 by gefaivre          #+#    #+#             */
/*   Updated: 2023/01/11 11:52:38 by jbach            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include <sys/epoll.h>
#include "Server.hpp"
#include "Client.hpp"



#define MAX_EVENTS 5
#define READ_SIZE 10

void epolling(Server server)
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
	server.setEpollFd(epoll_fd);

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
				// std::cout << "----------SERVER EVENT" << std::endl;
				server.newclient(epoll_fd);
			}
			else if (events[i].events == EPOLLHUP)
			{
				std::cout << RED << "EPOLLHUP" << reset << std::endl;
				
				epoll_ctl(server.getEpollFd(), EPOLL_CTL_DEL, events[i].data.fd, &event);
				if (close(events[i].data.fd) == -1)
					ft_define_error("Close error");
				server.clients.erase(server.clients.find(events[i].data.fd));
			}
			else if (events[i].events == EPOLLIN)
			{
				// std::cout << "----------EPOLLIN EVENT" << std::endl;

				if (server.clients.find(events[i].data.fd) == server.clients.end())
				{
					Client client = Client(&server, events[i].data.fd);
					server.clients.insert(std::pair<int, Client *>(events[i].data.fd, &client));
				}
				
				if (server.clients[events[i].data.fd]->readRequest() == -1)
				{
					std::cout << "Read request == 0 so delet eclient" << std::endl;
					epoll_ctl(server.getEpollFd(), EPOLL_CTL_DEL, events[i].data.fd, &event);
					if (close(events[i].data.fd) == -1)
						ft_define_error("Close error");
					server.clients.erase(server.clients.find(events[i].data.fd));
				}
			}
			else if (events[i].events == EPOLLOUT)
			{
				std::cout << "----------EPOLLOUT EVENT" << std::endl;

				server.clients[events[i].data.fd]->displayFullRequest();
				server.clients[events[i].data.fd]->verifyCgi();
				server.clients[events[i].data.fd]->saveFile();				
				server.clients[events[i].data.fd]->createResponse();
				
				server.clients[events[i].data.fd]->sendResponse();
			}		
		}
		bzero(events, MAX_EVENTS);
	}

	if (close(epoll_fd))
	{
		fprintf(stderr, "Failed to close epoll file descriptor\n");
		return;
	}
}
