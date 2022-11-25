/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoncalv <mgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 14:58:25 by mgoncalv          #+#    #+#             */
/*   Updated: 2022/11/23 15:59:00 by mgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include "Location.hpp"

void	Parser::prepareLine()
{
	string			line;
	size_t			find;
	string			buffer;
	static string	context;

	
	while (getline(_configFile, line))
	{
		if (line[line.find_first_not_of(" \n\t\v\f\r")] != '#')
			buffer += line;
	}
	
	//REMOVE INVALID SPACES
	find = buffer.find("\n\t\v\f\r");
	while (find != string::npos)
	{
		buffer.erase(find, 1);
		find = buffer.find("\n\t\v\f\r");
	}

	
	//ADD SPACE TO BRACKETS
	//Problema: se eu insiro um espaco antes o index da chave muda
	find = buffer.find("{");
	while (find != string::npos)
	{
		if (find > 0 && buffer[find - 1] != ' ')
			buffer.insert(find, " ");
		if (find > 0 && buffer[find + 1] != ' ')
			buffer.insert(find + 1, " ");
		find = buffer.find("{", find + 1);
	}
	
	find = buffer.find("}");
	while (find != string::npos)
	{
		if (find > 0 && buffer[find - 1] != ' ')
			buffer.insert(find, " ");
		if (find > 0 && buffer[find + 1] != ' ')
			buffer.insert(find + 1, " ");
		find = buffer.find("}", find + 1);
	}

	//REMOVE DOUBLE SPACES
	find = buffer.find("  ");
	while (find != string::npos)
	{
		buffer.erase(find, 1);
		find = buffer.find("  ");
	}


	//TRIM LINE
	// if (buffer[0] == ' ')
	// 	buffer.erase(0,1);
	// if (buffer[buffer.length() - 1] == ' ')
	// 	buffer.erase(buffer.length() - 1, 1);

	//ADD SPACE START AND SPACE END
	if (buffer[0] != ' ')
		buffer.insert(0, " ");
	if (buffer[buffer.length() - 1] != ' ')
		buffer.insert(buffer.length(), " ");
	
	cout << buffer << endl;

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
void	Parser::getServerConf(void)
{
	int level = 0;
	int i = 0;
	Location	*location;

	string sTarget = "server {";
	Server	server = Server();
	cout << "server:" << endl;
	// _context.push("server");
	level++;
	_currIdx += sTarget.length() + 1;
	while (level)
	{
		//Split this function:
		size_t	nextOpenBracket = _content.find("{", _currIdx);
		size_t	nextCloseBracket = _content.find("}",_currIdx);
		size_t	nextSemicolon = _content.find(";", _currIdx);


		// cout << "current = ."<< _content.substr(_currIdx, 100)<< "." << endl;
		if (nextOpenBracket < nextCloseBracket && nextOpenBracket < nextSemicolon)
		{

			string target = "location ";
			
			//checar se tem uma letra antes ex.: location
			//Checar que tem a palavra location + 2 espacos. Pq o path vai ser variavel
			if (_content.find(target, _currIdx) == _currIdx)
			{
				location = new Location(_content.substr(_currIdx + 9, nextOpenBracket - _currIdx - 10));
				cout << "	Location:" << location->getKey() << endl;
				level++;
			}
			else
				cerr << "error: { invalid" << endl;
			_currIdx += (nextOpenBracket - _currIdx) + 2;
		}
		if (nextCloseBracket < nextOpenBracket && nextCloseBracket < nextSemicolon)
		{
			//Atencao: um server {} e valido?
			//Se tiver uma palavra antes significa que falta um ;
			if(i == 0)
				cerr << "error: } invalid" << endl;
			else
			{
				level--;
				_currIdx += 2;
			}
		}
		if (nextSemicolon < nextOpenBracket && nextSemicolon < nextCloseBracket)
		{
			//Se tiver uma palavra antes temos uma directive;
			//Se nao, temos um ; solto
			// cout << "; found && " << _content[_currIdx + 1] << endl;
			
			if (_content[_currIdx + 1] == ';' || _currIdx == nextSemicolon)
			{
				cerr << "error: bad ;" << endl;
				exit(1);
			}
			else
			{
				string directive = _content.substr(_currIdx, nextSemicolon - _currIdx);
				
				//Tentar botar um limite nessa funcao find para nao percorrer toda a string
				if (directive.find("listen ") == 0)
				{
					//Check if already have a port
					server.setPort(atoi(directive.substr(7, directive.length() - 6).c_str()));
					cout << "	Port:" << server.getPort() <<"." << endl;
				}
				else if (directive.find("server_name ") == 0)
				{
					if (level > 1)
					{
						cerr << "error: server_name in wrong context" << endl;
						exit(1);
					}
					//Aqui precisamos de um split
					// name.push_back(directive.substr(12, directive.length() - 11));
					
					server.setName(ft_split(directive.substr(12, directive.length() - 11), ' '));
					// cout << "Server name:" <<  server._name <<"." << endl;
				}
				else if (directive.find("autoindex ") == 0)
				{
					cout << "	Autoindex: " << directive.substr(10) << "."<< endl;
					if (directive.substr(10) == "off")
						server.setAutoIndex(false);
					else if (directive.substr(10) == "on")
						server.setAutoIndex(true);
					else
					{
						cerr << "Error: autoindex: invalid value" << endl;
						exit (1);
					}
				}
				else if (directive.find("root ") == 0 && level == 2)
				{
					cout << "		Root:"<< directive.substr(5) << endl;
				}
				else if (directive.find("client_max_body_size ") == 0)
				{
					cout << "	Client_max_body_size: "<< directive.substr(21) << endl;
					server.setClientMaxBodySize(atoi(directive.substr(21).c_str()));
				}
				else if (directive.find("accepted_methods ") == 0)
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
						server.setAcceptedMethods(methods);
					cout << "	Accepted_methods: "<< directive.substr(17) << "." << endl;
				}
				_currIdx = nextSemicolon + 2;
			}
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
