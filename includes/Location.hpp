/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoncalv <mgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 13:52:17 by mgoncalv          #+#    #+#             */
/*   Updated: 2022/11/23 14:07:05 by mgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

#include <map>
#include <iostream>
#include "webserv.h"

using namespace std;

class Location
{
private:
	/* data */
	string				_key;
	map<string, string> _cgi;
	// int					port;
	// int					client_max_body_size;
	bool				_autoIndex;
	string				_root;
	t_methods			_acceptedMethods;
public:
	Location();
	Location(string key);
	~Location();

	string	getKey(void);

	void	addCgi(string key, string value);
	map<string, string> getCgiMap(void);
	string	getCgiValue(string key);

	void	setAutoIndex(bool autoIndex);
	bool	getAutoIndex(void);

	void	setRoot(string root);
	string	getRoot(void);

	void 		setAcceptedMethods(t_methods methods);
	t_methods	getAcceptedMethods();
};

#endif