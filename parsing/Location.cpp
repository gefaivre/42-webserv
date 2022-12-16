/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoncalv <mgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 14:08:38 by mgoncalv          #+#    #+#             */
/*   Updated: 2022/12/16 15:49:30 by mgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"
#include "Server.hpp"


#include <algorithm>

Location::Location(/* args */)
{
}

Location::Location(string key)
{
	_key = key;
}

Location::~Location()
{
}

string	Location::getKey(void)
{
	return (_key);
}



void	Location::setPort(int port)
{
	(void)port;
	// _port = port;
	// Simplesmente nao vai mudar a porta pois a porta é definido somente pelo server
	// Throw an error
}


void	Location::setName(vector<string> name)
{
	(void) name;
	// _name = name;
	// Simplesmente nao vai mudar o nome pois o nome é definido somente pelo server
	// Throw an error
}

void	Location::setClientMaxBodySize(int clientMaxBodySize)
{
	(void)clientMaxBodySize;
}

void Location::beSetup(Server *server)
{
	_clientMaxBodySize = server->getClientMaxBodySize();
	_name = server->getName();
	_port = server->getPort();
	// cout << "Location '" << this->_key << "' was setup by server !" << endl;

	//SETUP CGI
	map<string, string> serverCgi = server->getCgiMap();
	_cgi.insert(serverCgi.begin(), serverCgi.end());
	// if (find(_wasSet->begin(), _wasSet->end(), "cgi") == _wasSet->end())
	// nao usamos pois mesmo se o cgi ja foi colocado no location, o server pode ter outras keys
	if (find(_wasSet->begin(), _wasSet->end(), "autoIndex") == _wasSet->end())
		_autoIndex = server->getAutoIndex();
	if (find(_wasSet->begin(), _wasSet->end(), "root") == _wasSet->end())
		_root = server->getRoot();
	if (find(_wasSet->begin(), _wasSet->end(), "acceptedMethods") == _wasSet->end())
		_acceptedMethods = server->getAcceptedMethods();

	// size_t iter;
	// cout << _wasSet->size() << endl;
	// for (iter = 0; iter < _wasSet->size(); iter++)
	// 	cout << (*_wasSet)[iter] << endl;
}