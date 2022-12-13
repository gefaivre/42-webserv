/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbach <jbach@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 13:42:55 by mgoncalv          #+#    #+#             */
/*   Updated: 2022/12/13 15:50:45 by jbach            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "webserv.h"
#include <vector>
#include <iostream>
#include <unistd.h>

vector<string>	ft_split(string str, char c)
{
	size_t			start;
	size_t			end;
	vector<string>	arr;
	
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

char **ft_split(char *str)
{
	vector<string> arr = ft_split(str, ' ');
	char **result = malloc(sizeof(char *) * arr.lenght());
	for (int i = 0; i < arr.lenght; i++)
	{
		result[i] = arr[i].c_str();
	}
	return (result);
}