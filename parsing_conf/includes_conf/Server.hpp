/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoncalv <mgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 14:29:46 by mgoncalv          #+#    #+#             */
/*   Updated: 2022/12/16 15:50:54 by mgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <vector>
#include "webserv.h"
#include <map>
#include "Config.hpp"

#include "Location.hpp"

using namespace std;

class Server: public Config
{
private:
	// bool					_autoIndex;
	// string					_root;
	// string					_cgi;
	// t_methods				_acceptedMethods;
	map<string, Location>	_locations;
		

	
public:

	//set as private
	Server();
	~Server();

	void			setPort(int port);
	// int				getPort(void);
	
	void			setName(vector<string> name);
	// vector<string>	getName(void);

	// void			setAutoIndex(bool autoIndex);
	// bool			getAutoIndex(void);

	// void			setRoot(string root);
	// string			getRoot(void);

	void			setClientMaxBodySize(int clientMaxBodySize);
	int				getClientMaxBodySize(void);

	// void			setCgi(string cgi);
	// string			getCgi(void);

	// void 			setAcceptedMethods(t_methods methods);
	// t_methods		getAcceptedMethods();


	bool			locationExist(string key);
	void			addLocation(Location *location);
	Location		getLocationByPath(string path);

	void 			setupLocations();
};



#endif