/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbach <jbach@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/29 15:23:00 by mgoncalv          #+#    #+#             */
/*   Updated: 2023/01/17 20:27:01 by jbach            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

Config::Config()
{
	_wasSet = new vector<string>();
}

Config::~Config()
{
	// delete _wasSet;
}

void	Config::addCgi(string key, string value)
{
	_wasSet->push_back("cgi");
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
		throw (std::exception());
}

void Config::setAutoIndex(bool autoIndex)
{
	_wasSet->push_back("autoIndex");
	_autoIndex = autoIndex;
}

bool	Config::getAutoIndex(void)
{
	return (_autoIndex);
}

void	Config::setRoot(string root)
{ 
	_wasSet->push_back("root");
	if (root.size() != 0 && root[root.size() - 1] != '/')
		root += '/';

	_root = root;
	std::cout << "SET ROOT:" << root << std::endl;
}

string Config::getRoot(void)
{
	std::cout << "GET ROOT:" << _root << std::endl;
	return (_root);
}

void	Config::setAcceptedMethods(t_methods methods)
{
	_wasSet->push_back("acceptedMethods");
	_acceptedMethods = methods;
}

t_methods	Config::getAcceptedMethods()
{
	return (_acceptedMethods);
}

int		Config::getPort(void)
{
	return (_port);
}

vector<string> Config::getName(void)
{
	return (_name);
}


int		Config::getClientMaxBodySize(void)
{
	return (_clientMaxBodySize);
}
