/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gefaivre <gefaivre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 14:21:20 by mgoncalv          #+#    #+#             */
/*   Updated: 2023/01/04 08:00:23 by gefaivre         ###   ########.fr       */
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

#include "Server1.hpp"

using namespace std;

class Parser
{
private:
	ifstream		_configFile;
	string			_fileName;
	stack<string>	_context;
	string			_content;
	size_t			_currIdx;
	vector<Server1 *>	_servers;
	
public:
	Parser(void);
	~Parser(void);

	Parser(char *configName);

	void	prepareLine();
	void	checkHttpContext();
	string	ft_read_file_ignoring_comments();
	void	parseEndOfContext(int i);
	Location *parseNewContext(size_t nextOpenBracket, Server1 *server);
	void	parseAutoIndex(string directive, vector<Config *> conf);
	void	parseDirective(size_t nextSemiColon, vector<Config *> conf);
	Server1	*getServerConf(void);
	vector<Server1 *>	getServers(void);
};



#endif