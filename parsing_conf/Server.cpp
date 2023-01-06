/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gefaivre <gefaivre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/15 16:37:14 by mgoncalv          #+#    #+#             */
/*   Updated: 2023/01/06 19:12:46 by gefaivre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"


Server::Server()
{
	Location *location;
	location = new Location("/");
	this->addLocation(location);
	
	
}

Server::~Server()
{
	close(_sockfd);
}

void Server::setSocket()
{
	setStruct();

	_sockfd = socket(AF_INET, SOCK_STREAM, 0); //AF_INET,SOCK_STREAM, 0
	if (_sockfd == -1)
		ft_define_error("Server error");
	int yes = 1;
	if (setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR,
		(void*)&yes, sizeof(yes)) == -1)
		ft_define_error("Setsockopt error");
	if( (bind(_sockfd, (struct sockaddr*)&_addr, sizeof(_addr))) == -1)
		ft_define_error("Bind error");
	fcntl(_sockfd, F_SETFL, O_NONBLOCK);
	if (listen(_sockfd, SOMAXCONN) == -1)
		ft_define_error("Listen error");
}

void Server::setStruct()
{
	// std::cout<< "Port = " << getPort() << std::endl;
	memset((char *)&_addr, 0, sizeof(_addr));
	_addr.sin_addr.s_addr = INADDR_ANY;
	_addr.sin_port = htons(_port); 
	_addr.sin_family = AF_INET; 
}

void Server::newclient(int epoll_fd)
{
	int serverfd;
	struct epoll_event event;
	socklen_t addrlen = sizeof(_addr);

	serverfd = accept(this->getServerFd(), (struct sockaddr *)&_addr, &addrlen);
	if (serverfd == -1)
		ft_define_error("Error the connection with the socket was not established");
	event.events = EPOLLIN ;
	event.data.fd = serverfd;
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, serverfd, &event);
	
	
}

void	Server::setPort(int port)
{
	_port = port;
}

void	Server::setName(std::vector<std::string> name)
{
	_name = name;
}

void	Server::setClientMaxBodySize(int clientMaxBodySize)
{
	_clientMaxBodySize = clientMaxBodySize;
}

int		Server::getClientMaxBodySize(void)
{
	return (_clientMaxBodySize);
}

bool		Server::locationExist(std::string key)
{
	return (_locations.find(key) != _locations.end());
}

void		Server::addLocation(Location *location)
{
	if (!locationExist(location->getKey()))
	{
		_locations[location->getKey()] = *location;
	}
	else
		cerr << "Location already exists!" << endl;
		
}

Location	Server::getLocationByPath(std::string path)
{
	int			max = 0;
	Location	loc;

	std::map<std::string, Location>::iterator	it;
	for (it = _locations.begin(); it != _locations.end(); it++)
	{
		if	(ft_strcmp_fowardslash(path, it->first) > max)
			loc = it->second;
	}
	return loc;
}

void	Server::setupLocations()
{
	std::map<std::string, Location>::iterator iter;
	for (iter = _locations.begin(); iter != _locations.end(); iter++)
	{
		iter->second.beSetup(this);
	}
}

int Server::getServerFd() const
{
	return (_sockfd);
}


void			Server::setEpollFd(int epollfd)
{
	this->_epollfd = epollfd;
}

int				Server::getEpollFd() const
{
	return(this->_epollfd);
}

