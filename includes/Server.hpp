/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoncalv <mgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 14:29:46 by mgoncalv          #+#    #+#             */
/*   Updated: 2023/02/02 17:03:16 by mgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Server_HPP
# define Server_HPP

#include <iostream>
#include <string>
#include <vector>
#include "webserv.h"
#include <map>
#include "Config.hpp"
#include "Location.hpp"
#include <sys/epoll.h>
#include "../engine/utils.hpp"
#include "../includes/Server.hpp"
#include "../engine/Client.hpp"

char **ft_split(const char *str);
class Client;

class Server: public Config
{
private:

	int _sockfd;
	struct sockaddr_in _addr;
	int _epollfd;
	std::vector<Server *> _servers;
		

	
public:

	std::map<std::string, Location *>	_locations;
	Server();
	// Server(Server const & src);
	// Server&	operator=(Server const & rhs);
	~Server();

	void 			setStruct();
	void			setPort(int port);
	
	void			setName(std::vector<std::string> name);

	void 			newclient(int epoll_fd);
	
	std::map<int, Client *> 		clients;

	void 			setSocket();
	
	void			setEpollFd(int epollfd);
	int				getEpollFd() const;

	void			setClientMaxBodySize(int clientMaxBodySize);
	int				getClientMaxBodySize(void);

	int 			getServerFd() const;

	bool			locationExist(std::string key);
	void			addLocation(Location *location);
	Location		getLocationByPath(std::string path);

	void 			deleteClient(int fd);
	void 			setupLocations();

	Server 			*getServerByName(std::string name);
};



#endif
