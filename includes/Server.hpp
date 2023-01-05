/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbach <jbach@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 14:29:46 by mgoncalv          #+#    #+#             */
/*   Updated: 2023/01/05 16:57:56 by jbach            ###   ########.fr       */
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

char **ft_split(const char *str);
class Client;

class Server: public Config
{
private:

	map<string, Location>	_locations;
	int _sockfd;
	struct sockaddr_in _addr;
		

	
public:

	Server();
	~Server();

	void 			setStruct();
	void			setPort(int port);
	
	void			setName(std::vector<string> name);

	void 			newclient(int epoll_fd);
	std::map<int, Client *> 		clients;
	void 			setSocket();

	void			setClientMaxBodySize(int clientMaxBodySize);
	int				getClientMaxBodySize(void);

	int 			getServerFd() const;

	bool			locationExist(std::string key);
	void			addLocation(Location *location);
	Location		getLocationByPath(std::string path);

	void 			setupLocations();
};



#endif
