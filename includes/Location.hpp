/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbach <jbach@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 13:52:17 by mgoncalv          #+#    #+#             */
/*   Updated: 2023/01/05 16:14:28 by jbach            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

#include <map>
#include <iostream>
#include "webserv.h"
#include "Config.hpp"

class Server;
using namespace std;

class Location : public Config
{
private:
	/* data */
	string				_key;
	// map<string, string> _cgi;
	// bool				_autoIndex;
	// string				_root;
	// t_methods			_acceptedMethods;
public:
	Location();
	Location(string key);
	~Location();


	Location(Location const & src);
	Location &	operator=(Location const & rhs);
	
	string	getKey(void);

	// void	addCgi(string key, string value);
	// map<string, string> getCgiMap(void);
	// string	getCgiValue(string key);

	// void	setAutoIndex(bool autoIndex);
	// bool	getAutoIndex(void);

	// void	setRoot(string root);
	// string	getRoot(void);

	// void 		setAcceptedMethods(t_methods methods);
	// t_methods	getAcceptedMethods();

	void			setPort(int port);
	void			setName(vector<string> name);

	void			setClientMaxBodySize(int clientMaxBodySize);
	void 			beSetup(Server *server);
};

#endif