/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_remove_invalid_spaces.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoncalv <mgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 16:10:23 by mgoncalv          #+#    #+#             */
/*   Updated: 2022/11/25 16:29:46 by mgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

using namespace std;

void	ft_remove_invalid_spaces(string str)
{
	size_t	find;

	find = str.find("\n\t\v\f\r");
	while (find != string::npos)
	{
		str.erase(find, 1);
		find = str.find("\n\t\v\f\r");
	}
}