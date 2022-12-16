/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_remove_double_spaces.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoncalv <mgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 16:21:48 by mgoncalv          #+#    #+#             */
/*   Updated: 2022/11/25 16:30:10 by mgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

using namespace std;

void	ft_remove_double_spaces(string *str)
{
	size_t	find;

	find = str->find("  ");
	while (find != string::npos)
	{
		str->erase(find, 1);
		find = str->find("  ");
	}
}