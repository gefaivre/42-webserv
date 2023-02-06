/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbach <jbach@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 14:21:20 by mgoncalv          #+#    #+#             */
/*   Updated: 2023/02/06 15:24:59 by jbach            ###   ########.fr       */
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
#include <set>

#define PARS_ERROR 0
#define PARS_OK 1

class Parser
{
private:
	std::ifstream		_configFile;
	std::string			_fileName;
	std::stack<std::string>	_context;
	std::string			_content;
	size_t			_currIdx;
	std::vector<Server *>	_servers;
	std::vector<int>			_allPorts;
	std::set<int>			_repeatedPorts;
	int checkConfigName(char *configName);

	bool b_error;

public:
	Parser(void);
	~Parser(void);

	int set(char *configName);


	void	prepareLine();
	void	checkHttpContext();
	std::string	ft_read_file_ignoring_comments();
	void	parseEndOfContext(int i);
	Location *parseNewContext(size_t nextOpenBracket, Server *server);
	int	parseAutoIndex(std::string directive, std::vector<Config *> conf);
	int	parseDirective(size_t nextSemiColon, std::vector<Config *> conf);
	Server	*getServerConf(void);
	std::vector<Server *>	*getServers(void);
	int				verifyServerName();
};



#endif