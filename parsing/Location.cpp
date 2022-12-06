/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateus <mateus@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 14:08:38 by mgoncalv          #+#    #+#             */
/*   Updated: 2022/12/06 14:05:03 by mateus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"


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
int		Location::getPort(void)
{
	return (_port);
}

void	Location::setName(vector<string> name)
{
	(void) name;
	// _name = name;
	// Simplesmente nao vai mudar o nome pois o nome é definido somente pelo server
	// Throw an error
}
vector<string> Location::getName(void)
{
	return (_name);
}

// void	Location::addCgi(string key, string value)
// {
// 	if (_cgi.find(key) != _cgi.end())
// 		cerr << "This cgi is already been handled by: " << _cgi[key] << endl;
// 	else
// 		_cgi[key] = value;
// }

// map<string, string> Location::getCgiMap(void)
// {
// 	return (_cgi);
// }

// string	Location::getCgiValue(string key)
// {
// 	if (_cgi.find(key) != _cgi.end())
// 		return (_cgi[key]);
// 	else
// 		return (NULL);
// }

// void Location::setAutoIndex(bool autoIndex)
// {
// 	_autoIndex = autoIndex;
// }

// bool	Location::getAutoIndex(void)
// {
// 	return (_autoIndex);
// }

// void	Location::setRoot(string root)
// {
// 	_root = root;
// }

// string Location::getRoot(void)
// {
// 	return (_root);
// }

// void	Location::setAcceptedMethods(t_methods methods)
// {
// 	_acceptedMethods = methods;
// }

// t_methods	Location::getAcceptedMethods()
// {
// 	return (_acceptedMethods);
// }