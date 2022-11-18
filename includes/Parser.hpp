/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoncalv <mgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 14:21:20 by mgoncalv          #+#    #+#             */
/*   Updated: 2022/11/18 14:06:20 by mgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

#include <iostream>
#include <stack>
#include <fstream>
#include <string>
#include <vector>

#include <stdlib.h>

#include "webserv.h"

#include "Server.hpp"

using namespace std;

class Parser
{
private:
	ifstream		_configFile;
	stack<string>	_context;
	string			_content;
	size_t			_currIdx;
	vector<Server>	_servers;
	
public:
	Parser(void);
	~Parser(void);

	Parser(char *configName);

	void	prepareLine();
	void	checkHttpContext();
	void	getServerConf();
};



#endif