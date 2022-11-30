/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateus <mateus@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/29 15:23:03 by mgoncalv          #+#    #+#             */
/*   Updated: 2022/11/30 14:47:16 by mateus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

#include <map>
#include <iostream>
#include "webserv.h"

using namespace std;

class Config
{
private:
	map<string, string> _cgi;
	bool				_autoIndex;
	string				_root;
	t_methods			_acceptedMethods;
public:
	Config(/* args */);
	~Config();

	// Acessors:
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