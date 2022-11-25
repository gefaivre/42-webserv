/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoncalv <mgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 14:29:46 by mgoncalv          #+#    #+#             */
/*   Updated: 2022/11/18 13:40:00 by mgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <vector>

using namespace std;

typedef struct s_methods {
	bool	_get;
	bool	_post;
	bool	_delete;
} t_methods;

class Server
{
private:
	int				_port;
	vector<string>	_name;
	bool			_autoIndex;
	string			_root;
	int				_clientMaxBodySize;
	string			_cgi;
	t_methods		_acceptedMethods;
	
public:

	//set as private
	Server();
	~Server();

	void	setPort(int port);
	int		getPort(void);
	
	void	setName(vector<string> name);
	vector<string> getName(void);

	void	setAutoIndex(bool autoIndex);
	bool	getAutoIndex(void);

	void	setRoot(string root);
	string	getRoot(void);

	void	setClientMaxBodySize(int clientMaxBodySize);
	int		getClientMaxBodySize(void);

	void	setCgi(string cgi);
	string	getCgi(void);

	void 	setAcceptedMethods(t_methods methods);
	t_methods	getAcceptedMethods();
	
};



#endif