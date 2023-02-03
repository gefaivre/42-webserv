/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_remove_double_spaces.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoncalv <mgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 16:21:48 by mgoncalv          #+#    #+#             */
/*   Updated: 2023/02/02 15:17:58 by mgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

void	ft_remove_double_spaces(std::string *str)
{
	size_t	find;

	find = str->find("  ");
	while (find != std::string::npos)
	{
		str->erase(find, 1);
		find = str->find("  ");
	}
}