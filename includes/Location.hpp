/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateus <mateus@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 13:52:17 by mgoncalv          #+#    #+#             */
/*   Updated: 2022/11/30 14:47:35 by mateus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

#include <map>
#include <iostream>
#include "webserv.h"
#include "Config.hpp"

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
};

#endif