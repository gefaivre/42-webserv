/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoncalv <mgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 14:08:38 by mgoncalv          #+#    #+#             */
/*   Updated: 2022/11/21 14:44:35 by mgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"


Location::Location(/* args */)
{
}

Location::~Location()
{
}

void	Location::addCgi(string key, string value)
{
	if (_cgi.find(key) != _cgi.end())
		cerr << "This cgi is already been handled by: " << _cgi[key] << endl;
	else
		_cgi[key] = value;
}

map<string, string> Location::getCgiMap(void)
{
	return (_cgi);
}

string	Location::getCgiValue(string key)
{
	if (_cgi.find(key) != _cgi.end())
		return (_cgi[key]);
	else
		return (NULL);
}

void Location::setAutoIndex(bool autoIndex)
{
	_autoIndex = autoIndex;
}

bool	Location::getAutoIndex(void)
{
	return (_autoIndex);
}

void	Location::setRoot(string root)
{
	_root = root;
}

string Location::getRoot(void)
{
	return (_root);
}

void	Location::setAcceptedMethods(t_methods methods)
{
	_acceptedMethods = methods;
}

t_methods	Location::getAcceptedMethods()
{
	return (_acceptedMethods);
}