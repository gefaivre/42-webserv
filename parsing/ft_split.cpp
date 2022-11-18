/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoncalv <mgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 13:42:55 by mgoncalv          #+#    #+#             */
/*   Updated: 2022/11/18 14:04:34 by mgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.h"

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
	return (arr);
}