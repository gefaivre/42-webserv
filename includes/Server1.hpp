/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server1.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoncalv <mgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 14:29:46 by mgoncalv          #+#    #+#             */
/*   Updated: 2022/12/16 16:16:33 by mgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Server1_HPP
# define Server1_HPP

#include <iostream>
#include <vector>
#include "webserv.h"
#include <map>
#include "Config.hpp"

#include "Location.hpp"

using namespace std;

class Server1: public Config
{
private:
	// bool					_autoIndex;
	// string					_root;
	// string					_cgi;
	// t_methods				_acceptedMethods;
	map<string, Location>	_locations;
		

	
public:

	//set as private
	Server1();
	~Server1();

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

//:8020/all_test/oi/index.html
//:8020/all_test/oi