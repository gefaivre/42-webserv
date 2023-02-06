/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbach <jbach@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2023/02/06 15:43:16 by jbach            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "Parser.hpp"
#include "Location.hpp"
#include "Config.hpp"

void	Parser::prepareLine()
{
	std::string			line;
	std::string			buffer;
	
	while (getline(_configFile, line))
	{
		
		if (line.size() && line.find_first_not_of(" \n\t\v\f\r") != std::string::npos && line[line.find_first_not_of(" \n\t\v\f\r")] != '#')
			buffer += line;
	}
	ft_remove_invalid_spaces(&buffer);
	ft_wrap_brackets_in_spaces(&buffer);
	ft_remove_double_spaces(&buffer);
	ft_wrap_in_spaces(&buffer);
	_content = buffer;
}

Parser::Parser(void)
{
	b_error = 0;
}

void	Parser::parseEndOfContext(int i)
{
	//Atencao: um server {} e valido?
	//Se tiver uma palavra antes significa que falta um ;
	if(i == 0)
		std::cerr << "error: } invalid" << std::endl;
	else
		_currIdx += 2;
}

Location *Parser::parseNewContext(size_t nextOpenBracket, Server *server)
{
	std::string		target = "location ";
	int 		target_length = target.length();
	Location *location = NULL;
	int directive_value_start;
	// checar se tem uma letra antes ex.: location
	// Checar que tem a palavra location + 2 espacos. Pq o path vai ser variavel
	if (_content.find(target, _currIdx) == _currIdx)
	{
		directive_value_start = _currIdx + target_length;
		location = new Location(_content.substr(directive_value_start, nextOpenBracket - directive_value_start - 1));
		server->addLocation(location);
	
	}
	else
		std::cerr << "error: { invalid" << std::endl;
	_currIdx += (nextOpenBracket - _currIdx) + 2;
	return (location);
}

int	Parser::parseAutoIndex(std::string directive, std::vector<Config *> conf)
{
	bool autoindex;
	if (directive.substr(10) == "off")
		autoindex = false;
	else if (directive.substr(10) == "on")
		autoindex = true;
	else
	{
		std::cerr << "Error: autoindex: invalid value" << std::endl;
		return PARS_ERROR;
	}
	conf.back()->setAutoIndex(autoindex);
	return PARS_OK;
}

int	Parser::parseDirective(size_t nextSemiColon, std::vector<Config *> conf)
{
	if (_content[_currIdx + 1] == ';' || _currIdx == nextSemiColon)
	{
		std::cerr << "error: bad ;" << std::endl;
		return PARS_ERROR;
	}
	else
	{
		std::string directive = _content.substr(_currIdx, nextSemiColon - _currIdx);
		if (ft_starts_with(directive, "listen "))
		{
			int	port = atoi(directive.substr(7, directive.length() - 6).c_str());
			conf.back()->setPort(port);
			_allPorts.push_back(port);
			if(std::count(_allPorts.begin(), _allPorts.end(), port))
				_repeatedPorts.insert(port);
			
		}
		else if (ft_starts_with(directive, "rewrite "))
			std::vector<std::string> vector_rew = ft_split(directive.substr(8, directive.length() - 7), ' ');
		else if (ft_starts_with(directive, "server_name "))
			conf.back()->setName(ft_split(directive.substr(12, directive.length() - 11), ' '));
		else if (ft_starts_with(directive, "index "))
		{
			conf.back()->setIndex(directive.substr(6, directive.length() - 5));
		}
		else if (ft_starts_with(directive, "autoindex "))
		{
			if (parseAutoIndex(directive, conf) == PARS_ERROR)
				return PARS_ERROR;
		}
		else if (ft_starts_with(directive, "root "))
			conf.back()->setRoot(directive.substr(5));
		else if (ft_starts_with(directive, "client_max_body_size "))
			conf.back()->setClientMaxBodySize(atoi(directive.substr(21).c_str()));
		else if (ft_starts_with(directive, "cgi "))
		{
			std::vector<std::string> key_value = ft_split(directive.substr(5), ' ');
			conf.back()->addCgi(key_value[0], key_value[1]);
		}
		else if (ft_starts_with(directive, "accepted_methods "))
		{
			std::vector<std::string> acMethods = ft_split(directive.substr(17), ' ');
			t_methods	methods;
			bzero(&methods, sizeof(t_methods));
			for (size_t i = 0; i < acMethods.size(); i++)
			{
				if (acMethods[i] == "GET")
					methods._get = true;
				else if (acMethods[i] == "POST")
					methods._post = true;
				else if (acMethods[i] == "DELETE")
					methods._delete = true;
				else
				{
					std::cerr << "Error: invalid method" << std::endl;
					return PARS_ERROR;
				}
			}
			conf.back()->setAcceptedMethods(methods);
		}
		_currIdx = nextSemiColon + 2;
	}
	return PARS_OK;
}

Server	*Parser::getServerConf(void)
{
	int i = 0;

		
	std::vector<Config *> conf;
	std::string sTarget = "server {";
	Server *server = new Server();
	conf.push_back(server);
	_currIdx += sTarget.length() + 1;
	while (conf.size())
	{
		size_t next = _content.find_first_of("{};", _currIdx);
		switch (_content[next])
		{
		case '{':
			conf.push_back(parseNewContext(next,(Server *) conf[0]));
			break;
		case '}':
			parseEndOfContext(i);
			conf.pop_back();
			break;
		case ';':
			if (parseDirective(next, conf) == PARS_ERROR)
			{
				b_error = 1;
				return server;
			}
			break;
		default:
			// End of line
			// erro pois chegou no fim da linha antes de fechar todos os contexts;
			break;
		}
		i++;
	}
	server->setupLocations();
	return (server);
}


int Parser::verifyServerName()
{
	std::set<int>::iterator it;
	for (it = _repeatedPorts.begin(); it != _repeatedPorts.end(); ++it) {
		int port = *it;
		std::set<std::string> used_names;
		size_t sum = 0;
		for(std::vector<Server *>::iterator itV = _servers.begin(); itV != _servers.end(); ++itV) {
			if ((*itV)->getPort() == port && (*itV)->getName().size() == 0)
			{
				std::cout << "Cannot accepted two servers with same port if server_name is not set in both servers!" <<std::endl;
				return PARS_ERROR;
			}
			std::vector<std::string> server_names = (*itV)->getName();
			sum += server_names.size();
			std::vector<std::string>::iterator nameIt;
			for (nameIt = server_names.begin(); nameIt != server_names.end(); ++nameIt)
			{
				used_names.insert(*nameIt);
			}
			if (sum != used_names.size())
			{
				std::cout << "Cannot accepted two servers with same names!" << std::endl;
				return PARS_ERROR;
			}
		}
	}
	return PARS_OK;
}

int Parser::checkConfigName(char *configName)
{

	std::string sName(configName);

	if (isDirectory(sName))
	{
		std::cout << "Config file can't be a directory" << std::endl;
		return PARS_ERROR;
	}
	if (!(sName.size() > 5 && (sName.compare((sName.size() - 5), 5, std::string(".conf")) == 0)))
	{
		std::cout << "Config file must be a .conf file" << std::endl;
		return PARS_ERROR;
	}
		
	return PARS_OK;
	
}

int Parser::set(char *configName)
{
	std::vector<Server *> servers;

	if (checkConfigName(configName) == PARS_ERROR)
		return PARS_ERROR;
		
	_currIdx = 1;
	_configFile.open(configName);
	if (!_configFile.is_open())
	{
		std::cerr << "Cannot open file !" << std::endl;
		return PARS_ERROR;
	}
	prepareLine();
	while (_content.find("server {", _currIdx) == _currIdx)
		servers.push_back(getServerConf());
	if (b_error == 1)
		return PARS_ERROR;
	this->_servers = servers;
	if (servers.size() > 1)
	{
		if (verifyServerName() == PARS_ERROR)
			return PARS_ERROR;
	}
	_configFile.close();
	return PARS_OK;
}

Parser::~Parser(void)
{
	for (std::vector<Server *>::iterator it = _servers.begin(); it != _servers.end();) {
		std::cout << "PORT was destructed: " << (*it) << std::endl;
		delete (*it);
		it = _servers.erase(it);
	}
	_servers.clear();
	_configFile.close();
}

std::vector<Server*>	*Parser::getServers(void)
{
	for (std::vector<Server *>::iterator it = _servers.begin(); it != _servers.end(); it++) {
		(*it)->_servers = &_servers;
	}
	return (&_servers);
}