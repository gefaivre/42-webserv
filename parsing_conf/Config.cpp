/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoncalv <mgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/29 15:23:00 by mgoncalv          #+#    #+#             */
/*   Updated: 2023/02/03 16:04:33 by mgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

Config::Config()
{
	_wasSet = new std::vector<std::string>();
	_autoIndex = true;
	
}

Config::~Config()
{
	// std::cout << "CONF DESTRUCTEUR" << std::endl;

	
	if (_wasSet)
	{
		// std::cout << "was set loc:" << _wasSet <<std::endl;
		delete _wasSet;
		_wasSet = NULL;
	}
}

void	Config::addCgi(std::string key, std::string value)
{
	_wasSet->push_back("cgi");
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
	_wasSet->push_back("autoIndex");
	_autoIndex = autoIndex;
}

bool	Config::getAutoIndex(void)
{
	return (_autoIndex);
}

void	Config::setRoot(std::string root)
{  
	_wasSet->push_back("root");
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

std::vector<std::string> Config::getName(void)
{
	return (_name);
}


int		Config::getClientMaxBodySize(void)
{
	return (_clientMaxBodySize);
}
