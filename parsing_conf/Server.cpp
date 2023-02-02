/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aavezou <aavezou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/15 16:37:14 by mgoncalv          #+#    #+#             */
/*   Updated: 2023/02/01 15:32:58 by aavezou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"


Server::Server()
{
	// TODO
	// Location *location;
	// location = new Location("/");
	// this->addLocation(location);
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
	int clientfd;
	struct epoll_event event;
	socklen_t addrlen = sizeof(_addr);

	clientfd = accept(this->getServerFd(), (struct sockaddr *)&_addr, &addrlen);
	if (clientfd == -1)
		ft_define_error("Error the connection with the socket was not established");
	event.events = EPOLLIN | EPOLLRDHUP;
	event.data.fd = clientfd;
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, clientfd, &event);
	Client *client = new Client(this, clientfd);
	clients.insert(std::pair<int, Client *>(clientfd, client));
}

void Server::deleteClient(int clientfd) {
	epoll_ctl(getEpollFd(), EPOLL_CTL_DEL, clientfd, NULL);
	if (close(clientfd) == -1)
		ft_define_error("Close error");
	std::map<int, Client *>::iterator it = clients.find(clientfd);
	if (it != clients.end())
	{
		if (it->second)
			delete it->second;
		clients.erase(it);
	}
	else
		std::cout << GRN << "Le client existe pas" << reset << std::endl;
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
		_locations[location->getKey()] = location;
	}
	else
		cerr << "Location already exists!" << endl;
		
}

Location	Server::getLocationByPath(std::string path)
{
	int			max = -1;
	int 		newMax = -1;
	Location	*loc;
	std::map<std::string, Location *>::iterator	it;
	// std::cout << "path = " << path<< std::endl;
	for (it = _locations.begin(); it != _locations.end(); it++)
	{
		// std::cout << "it -> " << it->second.getRoot() << std::endl;
		newMax = ft_strcmp_fowardslash(path, it->first);
		if	( newMax > max)
		{
			max = newMax;
			loc = it->second;
			// std::cout << "LOC = " << loc.getKey() << std::endl;
		}
	}
	return *loc;
}

void	Server::setupLocations()
{
	if (!_locations.count("/"))
	{
		Location *location;
		location = new Location("/");
		this->addLocation(location);
	}
	std::map<std::string, Location *>::iterator iter;
	for (iter = _locations.begin(); iter != _locations.end(); iter++)
	{
		iter->second->beSetup(this);
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

void			Server::setServers(vector<Server *>	servers)
{
	this->_servers = servers;
}

std::vector<Server *>	Server::getServers()
{
	return (_servers);
}

Server 			*Server::getServerByName(std::string name)
{
	for (std::vector<Server *>::iterator it = _servers.begin(); it != _servers.end(); ++it)
	{
		std::vector<string> names = (*it)->getName();
		if (std::find(names.begin(), names.end(), name) != names.end())
		{
			return (*it);
		}
	}
	return (NULL);
}