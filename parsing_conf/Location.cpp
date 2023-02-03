/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoncalv <mgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 14:08:38 by mgoncalv          #+#    #+#             */
/*   Updated: 2023/02/03 16:04:25 by mgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"
#include "Server.hpp"


#include <algorithm>

Location::Location(/* args */)
{
}

Location::Location(Location const & src)
{
	// std::cout << "Copy constructor called" << std::endl;
	*this = src;
	return ;
}

Location&	Location::operator=(Location const & rhs)
{
	// std::cout << "Copy assignment operator called" << std::endl;
	_wasSet = new std::vector<std::string>();
	_wasSet->insert(_wasSet->end(), rhs._wasSet->begin(), rhs._wasSet->end());
	
	_key = rhs._key;
	_cgi = rhs._cgi;
	_autoIndex = rhs._autoIndex;
	_root = rhs._root;
	_acceptedMethods = rhs._acceptedMethods;
	_port = rhs._port;
	_clientMaxBodySize = rhs._clientMaxBodySize;
	_name = rhs._name;
	
	return (*this);
}

Location::Location(std::string key)
{
	_key = key;
}

Location::~Location()
{
	// std::cout << "LOC DESTRUCTEUR " << this->_key << std::endl;
}

std::string	Location::getKey(void)
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


void	Location::setName(std::vector<std::string> name)
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
	std::map<std::string, std::string> serverCgi = server->getCgiMap();
	_cgi.insert(serverCgi.begin(), serverCgi.end());
	// if (find(_wasSet->begin(), _wasSet->end(), "cgi") == _wasSet->end())
	// nao usamos pois mesmo se o cgi ja foi colocado no location, o server pode ter outras keys

	for (std::map<std::string, std::string>::iterator it = serverCgi.begin(); it != serverCgi.end(); ++it) {
		std::map<std::string, std::string>::iterator loc = _cgi.find(it->first);
		if (loc == _cgi.end()) {
			std::cout << it->second << "!!!!!!!!!!!!!!!!!!!" << std::endl;
			_cgi.insert(std::make_pair(it->first, it->second));
		}
	}




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