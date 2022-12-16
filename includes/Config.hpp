/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoncalv <mgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/29 15:23:03 by mgoncalv          #+#    #+#             */
/*   Updated: 2022/12/16 17:47:00 by mgoncalv         ###   ########.fr       */
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
protected:
	map<string, string> _cgi;
	bool				_autoIndex;
	string				_root;
	t_methods			_acceptedMethods;

	vector<string> 		*_wasSet;
	int					_port;

	
	int						_clientMaxBodySize;
	vector<string>			_name;
	
public:

	Config(/* args */);
	virtual ~Config();

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

	virtual void			setPort(int port) = 0;
	virtual void			setName(vector<string> name) = 0;
	virtual void			setClientMaxBodySize(int clientMaxBodySize) = 0;

	int						getPort(void);
	vector<string> 			getName(void);
	int						getClientMaxBodySize(void);
	
};



#endif