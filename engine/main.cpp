#include "Server.hpp"
#include "ParsingRequest.hpp"
#include "CreateResponse.hpp"
#include "Parser.hpp"
#include <sys/epoll.h>
#include <signal.h>

#include "Client.hpp"

#define MAX_EVENTS 5
#define READ_SIZE 10

Parser *g_servers;

std::map<int, bool> portMap;

void epolling(vector<Server *> *servers);

void sig_handler(int signo)
{
	if (signo == SIGINT)
	{
		delete g_servers;
		portMap.clear();
	}
	
	exit(0);
}

void epolling(vector<Server *> *servers)
{ 
	int running = 1, event_count, i;
	struct epoll_event events[MAX_EVENTS];


	for (size_t i = 0; i < (*servers).size(); i++)
	{
		struct epoll_event event;
		if (portMap.find((*servers)[i]->getPort()) == portMap.end())
		{
			portMap.insert(std::pair<int, bool>((*servers)[i]->getPort(), true));
			int epoll_fd = epoll_create1(0);
			if (epoll_fd == -1)
			{
				fprintf(stderr, "Failed to create epoll file descriptor\n");
				return;
			}

			(*servers)[i]->setEpollFd(epoll_fd);
			(*servers)[i]->setSocket();
		
			event.events = EPOLLIN;
			event.data.fd = (*servers)[i]->getServerFd();

			
			if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, (*servers)[i]->getServerFd(), &event))
			{
				fprintf(stderr, "Failed to add file descriptor to epoll\n");
				close(epoll_fd);
				return;
			}
		}
	}

	while (running)
	{
		for (size_t s = 0; s < (*servers).size(); s++)
		{
			event_count = epoll_wait((*servers)[s]->getEpollFd(), events, MAX_EVENTS, 0);
			for (i = 0; i < event_count; i++)
			{
				if (events[i].data.fd == (*servers)[s]->getServerFd())
				{
					(*servers)[s]->newclient((*servers)[s]->getEpollFd());
				}
				else if (events[i].events & (EPOLLHUP | EPOLLRDHUP))
				{
					(*servers)[s]->deleteClient(events[i].data.fd);
				}
				else if (events[i].events & EPOLLIN)
				{
					(*servers)[s]->clients[events[i].data.fd]->readRequest1();
				}
				else if (events[i].events & EPOLLOUT)
				{
						if ((*servers)[s]->clients[events[i].data.fd]->CreateAndSendResponse() == DELETE_CLIENT)
							(*servers)[s]->deleteClient(events[i].data.fd);
				}
			}
			if (s == (*servers).size())
				s = 0;
			bzero(events, MAX_EVENTS);
		}
	}
	for (size_t s = 0; s < (*servers).size(); i++)
	{
		if (close((*servers)[s]->getServerFd()))
		{
			fprintf(stderr, "Failed to close epoll file descriptor\n");
			return;
		}
	}
}



int main(int argc, char **argv)
{
	if ( argc != 2 )
	{
		cerr << "Usage: ./webserv [config file]" << endl;
		return (1);
	}

	Parser *config = new Parser();

	if (config->set(argv[1]) == PARS_ERROR)
	{
		delete config;
		return 1;
	}

	g_servers = config;
	std::vector<Server *> *servers = config->getServers();

	if (signal(SIGINT, sig_handler) == SIG_ERR)
 		std::cout << "can't catch SIGINT" << std::endl;

	std::cout << "BEFORE EPOLLING" <<std::endl;
	epolling(servers);
	
	return (0);
}