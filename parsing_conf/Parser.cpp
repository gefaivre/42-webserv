/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbach <jbach@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 14:58:25 by mgoncalv          #+#    #+#             */
/*   Updated: 2023/01/18 13:38:33 by jbach            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include "Location.hpp"
#include "Config.hpp"

void	Parser::prepareLine()
{
	string			line;
	string			buffer;
	
	while (getline(_configFile, line))
		if (line.size() && line[line.find_first_not_of(" \n\t\v\f\r")] != '#')
			buffer += line;
	ft_remove_invalid_spaces(&buffer);
	ft_wrap_brackets_in_spaces(&buffer);
	ft_remove_double_spaces(&buffer);
	ft_wrap_in_spaces(&buffer);
	_content = buffer;
	cout << buffer << endl;
}

Parser::Parser(void)
{
}

void	Parser::parseEndOfContext(int i)
{
	//Atencao: um server {} e valido?
	//Se tiver uma palavra antes significa que falta um ;
	if(i == 0)
		cerr << "error: } invalid" << endl;
	else
		_currIdx += 2;
}

Location *Parser::parseNewContext(size_t nextOpenBracket, Server *server)
{
	string		target = "location ";
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
		cout << "	Location:" << location->getKey() << endl;
	}
	else
		cerr << "error: { invalid" << endl;
	_currIdx += (nextOpenBracket - _currIdx) + 2;
	return (location);
}

void	Parser::parseAutoIndex(string directive, vector<Config *> conf)
{
	bool autoindex;
	cout << "	Autoindex: " << directive.substr(10) << "."<< endl;
	if (directive.substr(10) == "off")
		autoindex = false;
	else if (directive.substr(10) == "on")
		autoindex = true;
	else
	{
		cerr << "Error: autoindex: invalid value" << endl;
		exit (1);
	}
	conf.back()->setAutoIndex(autoindex);
}

void	Parser::parseDirective(size_t nextSemiColon, vector<Config *> conf)
{
			if (_content[_currIdx + 1] == ';' || _currIdx == nextSemiColon)
			{
				cerr << "error: bad ;" << endl;
				exit(1);
			}
			else
			{
				string directive = _content.substr(_currIdx, nextSemiColon - _currIdx);
				if (ft_starts_with(directive, "listen "))
					conf.back()->setPort(atoi(directive.substr(7, directive.length() - 6).c_str()));
				else if (ft_starts_with(directive, "server_name "))
					conf.back()->setName(ft_split(directive.substr(12, directive.length() - 11), ' '));
				else if (ft_starts_with(directive, "autoindex "))
					parseAutoIndex(directive, conf);
				else if (ft_starts_with(directive, "root "))
					conf.back()->setRoot(directive.substr(5));
				else if (ft_starts_with(directive, "client_max_body_size "))
					conf.back()->setClientMaxBodySize(atoi(directive.substr(21).c_str()));
				else if (ft_starts_with(directive, "cgi "))
				{
					vector<string> key_value = ft_split(directive.substr(5), ' ');
					conf.back()->addCgi(key_value[0], key_value[1]);
				}
				else if (ft_starts_with(directive, "accepted_methods "))
				{
					vector<string> acMethods = ft_split(directive.substr(17), ' ');
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
							cerr << "Error: invalid method" << endl;
							exit(1);
						}
					}
					conf.back()->setAcceptedMethods(methods);
					cout << "	Accepted_methods: "<< directive.substr(17) << "." << endl;
				}
				_currIdx = nextSemiColon + 2;
			}
			
}

Server	*Parser::getServerConf(void)
{
	int i = 0;

		
	vector<Config *> conf;
	string sTarget = "server {";
	Server *server = new Server();
	conf.push_back(server);
	cout << "server:" << endl;
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
			parseDirective(next, conf);
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

Parser::Parser(char *configName)
{
	vector<Server *> servers;
	_currIdx = 1;
	_configFile.open(configName);
	if (!_configFile.is_open())
	{
		cerr << "Cannot open file !" << endl;
		exit (1);
	}
	prepareLine();
	while (_content.find("server {", _currIdx) == _currIdx)
		servers.push_back(getServerConf());
	this->_servers = servers;
}

Parser::~Parser(void)
{
	_configFile.close();
}

vector<Server*>	Parser::getServers(void)
{
	return (_servers);
}