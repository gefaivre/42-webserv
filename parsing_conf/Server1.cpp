/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server1.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoncalv <mgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/15 16:37:14 by mgoncalv          #+#    #+#             */
/*   Updated: 2022/12/16 16:43:00 by mgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server1.hpp"

Server1::Server1()
{
	Location *location;
	location = new Location("/");
	this->addLocation(location);
}

Server1::~Server1()
{
}

void	Server1::setPort(int port)
{
	_port = port;
}


void	Server1::setName(vector<string> name)
{
	_name = name;
}


// void Server1::setAutoIndex(bool autoIndex)
// {
// 	_autoIndex = autoIndex;
// }

// bool	Server1::getAutoIndex(void)
// {
// 	return (_autoIndex);
// }

// void	Server1::setRoot(string root)
// {
// 	_root = root;
// }

// string Server1::getRoot(void)
// {
// 	return (_root);
// }

void	Server1::setClientMaxBodySize(int clientMaxBodySize)
{
	_clientMaxBodySize = clientMaxBodySize;
}

int		Server1::getClientMaxBodySize(void)
{
	return (_clientMaxBodySize);
}

// void 	Server1::setCgi(string cgi)
// {
// 	_cgi = cgi;
// }

// string	Server1::getCgi(void)
// {
// 	return (_cgi);
// }

// void	Server1::setAcceptedMethods(t_methods methods)
// {
// 	_acceptedMethods = methods;
// }

// t_methods	Server1::getAcceptedMethods()
// {
// 	return (_acceptedMethods);
// }

bool		Server1::locationExist(string key)
{
	return (_locations.find(key) != _locations.end());
}

void		Server1::addLocation(Location *location)
{
	if (!locationExist(location->getKey()))
	{
		_locations[location->getKey()] = *location;
	}
	else
		cerr << "Location already exists!" << endl;
		
}


Location	Server1::getLocationByPath(string path)
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


void	Server1::setupLocations()
{
	map<string, Location>::iterator iter;
	for (iter = _locations.begin(); iter != _locations.end(); iter++)
	{
		iter->second.beSetup(this);
	}
}