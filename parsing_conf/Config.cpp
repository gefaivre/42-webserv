/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoncalv <mgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/29 15:23:00 by mgoncalv          #+#    #+#             */
/*   Updated: 2023/02/03 23:32:18 by mgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include <limits.h>

Config::Config()
{
	_autoIndex = true;
	_clientMaxBodySize = INT_MAX;
	
}

Config::~Config()
{
}

void	Config::addCgi(std::string key, std::string value)
{
	_wasSet.push_back("cgi");
	if (_cgi.find(key) != _cgi.end())
		std::cerr << "This cgi is already been handled by: " << _cgi[key] << std::endl;
	else
		_cgi[key] = value;
}

std::map<std::string, std::string> Config::getCgiMap(void)
{
	return (_cgi);
}

std::string	Config::getCgiValue(std::string key)
{
	if (_cgi.find(key) != _cgi.end())
		return (_cgi[key]);
	else
		throw (std::exception());
}

void Config::setAutoIndex(bool autoIndex)
{
	_wasSet.push_back("autoIndex");
	_autoIndex = autoIndex;
}

bool	Config::getAutoIndex(void)
{
	return (_autoIndex);
}

void	Config::setRoot(std::string root)
{  
	_wasSet.push_back("root");
	if (root.size() != 0 && root[root.size() - 1] != '/')
		root += '/';

	_root = root;
}

std::string Config::getRoot(void)
{
	return (_root);
}

void	Config::setAcceptedMethods(t_methods methods)
{
	_wasSet.push_back("acceptedMethods");
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

std::vector<std::string> Config::getName(void)
{
	return (_name);
}

void	Config::setClientMaxBodySize(int clientMaxBodySize)
{
	_wasSet.push_back("client_max_body_size");
	_clientMaxBodySize = clientMaxBodySize;
}


int		Config::getClientMaxBodySize(void)
{
	return (_clientMaxBodySize);
}

void			Config::setIndex(std::string index)
{
	_wasSet.push_back("index");
	this->_index = index;
}

std::string 			Config::getIndex(void)
{
	return (_index);
}