/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoncalv <mgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/15 16:37:14 by mgoncalv          #+#    #+#             */
/*   Updated: 2022/11/17 15:04:07 by mgoncalv         ###   ########.fr       */
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

void Server::setAutoIndex(bool autoIndex)
{
	_autoIndex = autoIndex;
}

bool	Server::getAutoIndex(void)
{
	return (_autoIndex);
}

void	Server::setRoot(string root)
{
	_root = root;
}

string Server::getRoot(void)
{
	return (_root);
}

void	Server::setClientMaxBodySize(int clientMaxBodySize)
{
	_clientMaxBodySize = clientMaxBodySize;
}

int		Server::getClientMaxBodySize(void)
{
	return (_clientMaxBodySize);
}

void 	Server::setCgi(string cgi)
{
	_cgi = cgi;
}

string	Server::getCgi(void)
{
	return (_cgi);
}

void	Server::setAcceptedMethods(t_methods methods)
{
	_acceptedMethods = methods;
}