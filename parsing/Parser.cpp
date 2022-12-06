/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateus <mateus@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 14:58:25 by mgoncalv          #+#    #+#             */
/*   Updated: 2022/12/05 14:06:34 by mateus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include "Location.hpp"

void	Parser::prepareLine()
{
string			line;
	string			buffer;
	
	while (getline(_configFile, line))
	{
		if (line.size() && line[line.find_first_not_of(" \n\t\v\f\r")] != '#')
			buffer += line;
	}
	ft_remove_invalid_spaces(&buffer);
	ft_wrap_brackets_in_spaces(&buffer);
	ft_remove_double_spaces(&buffer);
	// TRIM LINE
	// if (buffer[0] == ' ')
	// 	buffer.erase(0,1);
	// if (buffer[buffer.length() - 1] == ' ')
	// 	buffer.erase(buffer.length() - 1, 1);

	// ADD SPACE START AND SPACE END
	ft_wrap_in_spaces(&buffer);

	_content = buffer;
	cout << buffer << endl;
}

// void	Parser::checkHttpContext()
// {
// 	size_t			find;
// 	string			target = " http {";
	
// 	find = _content.find(target);
// 	if (find != 0)
// 		cerr << "Error: missing http context" << endl;
		
// 	_context.push("http");
// 	_currIdx = find + target.length();
// 	if (_content[_currIdx] == ' ')
// 		_currIdx++;
// 	find = _content.find(target, find + 1);
// 	if (find != string::npos)
// 		cerr << "Error: http context duplicated" << endl;
// 	cout << "Index: " << _currIdx << endl;
// }

Parser::Parser(void)
{
}

//TODO: remove space before ;
//TODO: se fizermos a classe server e location herdarem da mesma classe base podemos criar uma funcao que aceita as duas usando polimorfismo.
//TODO: ou podemos usar uma classe template

void	Parser::parseEndOfContext(int i, int *level)
{
	//Atencao: um server {} e valido?
	//Se tiver uma palavra antes significa que falta um ;
	if(i == 0)
		cerr << "error: } invalid" << endl;
	else
	{
		*level = *level - 1;
		_currIdx += 2;
	}
}

Location *Parser::parseNewContext(size_t nextOpenBracket, Server *server, int *level)
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
		*level = *level + 1;
	}
	else
		cerr << "error: { invalid" << endl;
	_currIdx += (nextOpenBracket - _currIdx) + 2;
	return (location);
}

//TODO: colocar as funcoes set port e get port na classe location.
// get port vai retornar a porta do server parent. set port vai dar gerar uma excessao.
void	Parser::parseDirective(size_t nextSemiColon, Server *server, Location *location, int level)
{
	//Se tiver uma palavra antes temos uma directive;
			//Se nao, temos um ; solto
			// cout << "; found && " << _content[_currIdx + 1] << endl;
			
			if (_content[_currIdx + 1] == ';' || _currIdx == nextSemiColon)
			{
				cerr << "error: bad ;" << endl;
				exit(1);
			}
			else
			{
				string directive = _content.substr(_currIdx, nextSemiColon - _currIdx);
				//Checar aqui o level. if level = 1 chamar a funcao que ajeita o location.
				
				//Tentar botar um limite nessa funcao find para nao percorrer toda a string
				if (ft_starts_with(directive, "listen "))
				{
					//Check if already have a port
					server->setPort(atoi(directive.substr(7, directive.length() - 6).c_str()));
					cout << "	Port:" << server->getPort() <<"." << endl;
				}
				else if (ft_starts_with(directive, "server_name "))
				{
					if (level > 1)
					{
						cerr << "error: server_name in wrong context" << endl;
						exit(1);
					}
					//Aqui precisamos de um split
					// name.push_back(directive.substr(12, directive.length() - 11));
					
					server->setName(ft_split(directive.substr(12, directive.length() - 11), ' '));
					// cout << "Server name:" <<  server->_name <<"." << endl;
				}
				else if (ft_starts_with(directive, "autoindex "))
				{
					bool	autoindex;
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
					if (level == 1)
						server->setAutoIndex(autoindex);
					else if (level == 2)
						location->setAutoIndex(autoindex);
				}
				else if (ft_starts_with(directive, "root "))
				{
					if (level == 1)
					{
						server->setRoot(directive.substr(5));
						cout << "	Root: "<< server->getRoot() << endl;
					}
					else if (level == 2)
					{
						location->setRoot(directive.substr(5));
						cout << "		Root: "<< location->getRoot() << endl;
					}
				}
				else if (ft_starts_with(directive, "client_max_body_size "))
				{
					cout << "	Client_max_body_size: "<< directive.substr(21) << endl;
					server->setClientMaxBodySize(atoi(directive.substr(21).c_str()));
				}
				else if (ft_starts_with(directive, "accepted_methods "))
				{
					vector<string> acMethods = ft_split(directive.substr(17), ' ');
					t_methods	methods;
					methods._delete = false;
					methods._get = false;
					methods._post = false;
					for (size_t i = 0; i < acMethods.size(); i++)
					{
						if (acMethods[i] == "GET")
							methods._get = true;
						else if (acMethods[i] == "POST")
							methods._get = true;
						else if (acMethods[i] == "DELETE")
							methods._delete = true;
						else
						{
							cerr << "Error: invalid method" << endl;
							exit(1);
						}
					}
					if (level == 1)
						server->setAcceptedMethods(methods);
					else if (level == 2)
						location->setAcceptedMethods(methods);
					cout << "	Accepted_methods: "<< directive.substr(17) << "." << endl;
				}
				_currIdx = nextSemiColon + 2;
			}
}

void	Parser::getServerConf(void)
{
	int level = 0;
	int i = 0;
	Location	*location;

	//TODO: veridicar que a primeira coisa que aparece é server {}
	//TODO: criar um vetor de CONF, o primeiro eu vou fazer = new Server, depois vai ser com as locations
	string sTarget = "server {";
	Server	*server = new Server();
	cout << "server:" << endl;
	// _context.push("server");
	level++;
	_currIdx += sTarget.length() + 1;
	while (level)
	{
		size_t next = _content.find_first_of("{};", _currIdx);
		switch (_content[next])
		{
		case '{':
			location = parseNewContext(next, server, &level);
			break;
		case '}':
			parseEndOfContext(i, &level);
			break;
		case ';':
			parseDirective(next, server, location, level);
			break;
		default:
			// End of line
			break;
		}
		i++;
	}
	
	// else if (_content.find("}") == _currIdx){
	// 	cerr << "http context is empty" << endl;
	// }
	// else {
	// 	cout << "Directive in http context " << endl;
	// }
	// _currIdx -= 1;
	// cout << "Rest:" <<_content.substr(_currIdx - 1, 1000) << endl;
}

Parser::Parser(char *configName)
{
	_currIdx = 1;
	_configFile.open(configName);
	if (!_configFile.is_open())
	{
		cerr << "Cannot open file !" << endl;
		return ;
	}
	prepareLine();
	while (_content.find("server {", _currIdx) == _currIdx)
		getServerConf();
	
}

Parser::~Parser(void)
{
	_configFile.close();
}
