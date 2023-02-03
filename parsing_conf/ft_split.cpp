/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoncalv <mgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 13:42:55 by mgoncalv          #+#    #+#             */
/*   Updated: 2023/02/02 15:18:48 by mgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "webserv.h"
#include <vector>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>

std::vector<std::string>	ft_split(std::string str, char c)
{
	size_t			start;
	size_t			end;
	std::vector<std::string>	arr;
	
	start = 0;
	while ((end = str.find(c, start)) && end < str.length())
	{
		arr.push_back(str.substr(start, end - start));
		start = end + 1;
	}
	if (str[str.length() - 1] != c)
		arr.push_back(str.substr(start, str.length() - start));
	return (arr);
}

char **ft_split(const char *str)
{
	std::vector<std::string> arr = ft_split(str, ' ');
	char **result = (char **) malloc(sizeof(char *) * arr.size());
	for (size_t i = 0; i < arr.size(); i++)
	{
		result[i] = &arr[i][0];
	}
	return (result);
}