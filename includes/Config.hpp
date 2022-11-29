/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoncalv <mgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/29 15:23:03 by mgoncalv          #+#    #+#             */
/*   Updated: 2022/11/29 15:33:53 by mgoncalv         ###   ########.fr       */
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
	string				_key;
	map<string, string> _cgi;
	bool				_autoIndex;
	string				_root;
	t_methods			_acceptedMethods;
public:
	Config(/* args */);
	~Config();
};



#endif