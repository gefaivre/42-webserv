/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoncalv <mgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 14:21:20 by mgoncalv          #+#    #+#             */
/*   Updated: 2022/12/16 15:26:00 by mgoncalv         ###   ########.fr       */
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
	string			_fileName;
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
	string	ft_read_file_ignoring_comments();
	void	parseEndOfContext(int i);
	Location *parseNewContext(size_t nextOpenBracket, Server *server);
	void	parseAutoIndex(string directive, vector<Config *> conf);
	void	parseDirective(size_t nextSemiColon, vector<Config *> conf);
	Server	*getServerConf(void);
};



#endif