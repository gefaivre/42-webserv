/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateus <mateus@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/29 15:23:00 by mgoncalv          #+#    #+#             */
/*   Updated: 2022/11/30 14:43:22 by mateus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

Config::Config()
{
}

Config::~Config()
{
}

void	Config::addCgi(string key, string value)
{
	if (_cgi.find(key) != _cgi.end())
		cerr << "This cgi is already been handled by: " << _cgi[key] << endl;
	else
		_cgi[key] = value;
}

map<string, string> Config::getCgiMap(void)
{
	return (_cgi);
}

string	Config::getCgiValue(string key)
{
	if (_cgi.find(key) != _cgi.end())
		return (_cgi[key]);
	else
		return (NULL);
}

void Config::setAutoIndex(bool autoIndex)
{
	_autoIndex = autoIndex;
}

bool	Config::getAutoIndex(void)
{
	return (_autoIndex);
}

void	Config::setRoot(string root)
{
	_root = root;
}

string Config::getRoot(void)
{
	return (_root);
}

void	Config::setAcceptedMethods(t_methods methods)
{
	_acceptedMethods = methods;
}

t_methods	Config::getAcceptedMethods()
{
	return (_acceptedMethods);
}