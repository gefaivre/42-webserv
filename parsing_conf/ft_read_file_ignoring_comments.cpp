/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_read_file_ignoring_comments.cpp                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoncalv <mgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 18:29:58 by mgoncalv          #+#    #+#             */
/*   Updated: 2023/02/02 15:17:47 by mgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>


std::string	ft_read_file_ignoring_comments(std::ifstream	_configFile)
{
	std::string			line;
	std::string			buffer;
	
	while (std::getline(_configFile, line))
	{
		if (line.size() && line[line.find_first_not_of(" \n\t\v\f\r")] != '#')
			buffer += line;
	}
	return (buffer);
}