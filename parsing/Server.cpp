/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateus <mateus@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/15 16:37:14 by mgoncalv          #+#    #+#             */
/*   Updated: 2022/12/08 14:32:29 by mateus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server()
{
}

Server::~Server()
{
}

void	Server::setPort(int port)
{
	_port = port;
}
int		Server::getPort(void)
{
	return (_port);
}

void	Server::setName(vector<string> name)
{
	_name = name;
}
vector<string> Server::getName(void)
{
	return (_name);
}

// void Server::setAutoIndex(bool autoIndex)
// {
// 	_autoIndex = autoIndex;
// }

// bool	Server::getAutoIndex(void)
// {
// 	return (_autoIndex);
// }

// void	Server::setRoot(string root)
// {
// 	_root = root;
// }

// string Server::getRoot(void)
// {
// 	return (_root);
// }

void	Server::setClientMaxBodySize(int clientMaxBodySize)
{
	_clientMaxBodySize = clientMaxBodySize;
}

int		Server::getClientMaxBodySize(void)
{
	return (_clientMaxBodySize);
}

// void 	Server::setCgi(string cgi)
// {
// 	_cgi = cgi;
// }

// string	Server::getCgi(void)
// {
// 	return (_cgi);
// }

// void	Server::setAcceptedMethods(t_methods methods)
// {
// 	_acceptedMethods = methods;
// }

// t_methods	Server::getAcceptedMethods()
// {
// 	return (_acceptedMethods);
// }

bool		Server::locationExist(string key)
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


Location	Server::getLocationByPath(string path)
{
//contar quantas /partes/ a key de uma location tem em comum com path.
	int			max = 0;
	Location	loc;

	map<string, Location>::iterator	it;
	for (it = _locations.begin(); it != _locations.end(); it++)
	{
		if	(ft_strcmp_fowardslash(path, it->first) > max)
			loc = it->second;
	}
	return loc;
}


void	Server::setupLocations()
{
	map<string, Location>::iterator iter;
	for (iter = _locations.begin(); iter != _locations.end(); iter++)
	{
		iter->second.beSetup(this);
	}
}