/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp_fowardslash.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoncalv <mgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 14:06:09 by mgoncalv          #+#    #+#             */
/*   Updated: 2023/02/02 15:26:18 by mgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.h"

int	ft_strcmp_fowardslash(std::string str1, std::string str2)
{
	int	foward = 0;
	int i = 0;
	while (str1[i] && str2[i] && str1[i] == str2[i])
	{
		if (str1[i] == '/')
			foward++;
		i++;
	}
	return (foward);
}