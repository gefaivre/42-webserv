/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_header.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbach <jbach@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 13:42:55 by mgoncalv          #+#    #+#             */
/*   Updated: 2023/01/19 17:49:48 by jbach            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>

std::vector<std::string>	ft_split_header(std::string str)
{
	size_t			start;
	size_t			end;
	std::vector<std::string>	arr;
	std::string one_line;
	
	char c = '\n';
	start = 0;
	// str.erase(str.find("\r"), str.size());
	while ((end = str.find(c, start)) && end < str.length())
	{
		one_line = str.substr(start, end - start);
		if (one_line[one_line.size() - 1] == '\r')
			one_line = one_line.substr(0, one_line.size() - 1);
		if (!one_line.empty())
			arr.push_back(one_line);
		start = end + 1;
	}
	if (str[str.length() - 1] != c)
	{
		one_line = str.substr(start, str.length() - start);
		if (one_line[one_line.size() - 1] == '\r')
			one_line = one_line.substr(0, one_line.size() - 1);
		if (!one_line.empty())
			arr.push_back(one_line);
	}
	return (arr);
}
