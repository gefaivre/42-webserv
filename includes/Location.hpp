/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbach <jbach@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 13:52:17 by mgoncalv          #+#    #+#             */
/*   Updated: 2023/02/06 15:49:45 by jbach            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

#include <map>
#include <iostream>
#include "webserv.h"
#include "Config.hpp"

class Server;
class Location : public Config
{
private:
	/* data */
	std::string				_key;
	// map<string, string> _cgi;
	// bool				_autoIndex;
	// string				_root;
	// t_methods			_acceptedMethods;
public:
	Location();
	Location(std::string key);
	~Location();


	Location(Location const & src);
	Location &	operator=(Location const & rhs);
	
	std::string	getKey(void);

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
	void			setName(std::vector<std::string> name);

	void 			beSetup(Server *server);
};

#endif