/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_read_file_ignoring_comments.cpp                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateus <mateus@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 18:29:58 by mgoncalv          #+#    #+#             */
/*   Updated: 2022/11/30 14:31:16 by mateus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>

using namespace std;

string	ft_read_file_ignoring_comments(ifstream	_configFile)
{
	string			line;
	string			buffer;
	
	while (getline(_configFile, line))
	{
		if (line.size() && line[line.find_first_not_of(" \n\t\v\f\r")] != '#')
			buffer += line;
	}
	return (buffer);
}