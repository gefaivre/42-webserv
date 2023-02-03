/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoncalv <mgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 14:21:20 by mgoncalv          #+#    #+#             */
/*   Updated: 2023/02/03 18:15:19 by mgoncalv         ###   ########.fr       */
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
public:
	Parser(void);
	~Parser(void);

	Parser(char *configName);

	void	prepareLine();
	void	checkHttpContext();
	std::string	ft_read_file_ignoring_comments();
	void	parseEndOfContext(int i);
	Location *parseNewContext(size_t nextOpenBracket, Server *server);
	void	parseAutoIndex(std::string directive, std::vector<Config *> conf);
	void	parseDirective(size_t nextSemiColon, std::vector<Config *> conf);
	Server	*getServerConf(void);
	std::vector<Server *>	*getServers(void);
	void				verifyServerName();
};



#endif