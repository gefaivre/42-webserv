/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoncalv <mgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/29 15:23:03 by mgoncalv          #+#    #+#             */
/*   Updated: 2023/02/03 23:32:00 by mgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

#include <map>
#include <iostream>
#include "webserv.h"

class Config
{
protected:
	std::map<std::string, std::string> _cgi;
	bool				_autoIndex;
	std::string				_root;
	t_methods			_acceptedMethods;

	int					_port;

	
	int						_clientMaxBodySize;
	std::vector<std::string>			_name;
	std::string 		_index;
public:

	std::vector<std::string> 		_wasSet;
	Config(/* args */);
	virtual ~Config();

	// Acessors:
	void	addCgi(std::string key, std::string value);
	std::map<std::string, std::string> getCgiMap(void);
	std::string	getCgiValue(std::string key);

	void	setAutoIndex(bool autoIndex);
	bool	getAutoIndex(void);

	void	setRoot(std::string root);
	std::string	getRoot(void);

	void 		setAcceptedMethods(t_methods methods);
	t_methods	getAcceptedMethods();

	virtual void			setPort(int port) = 0;
	virtual void			setName(std::vector<std::string> name) = 0;

	int						getPort(void);
	std::vector<std::string> 			getName(void);
	int						getClientMaxBodySize(void);
	void			setClientMaxBodySize(int clientMaxBodySize);

	void			setIndex(std::string index);
	std::string			getIndex(void);
	
};



#endif