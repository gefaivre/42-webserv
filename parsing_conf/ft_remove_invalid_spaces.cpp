/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_remove_invalid_spaces.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoncalv <mgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 16:10:23 by mgoncalv          #+#    #+#             */
/*   Updated: 2023/02/02 15:18:32 by mgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

void	ft_remove_invalid_spaces(std::string *str)
{
	size_t	find;

	find = str->find_first_of("\n\t\v\f\r");
	while (find != std::string::npos)
	{
		// str->erase(find, 1);
		str->replace(find, 1, " ");
		find = str->find_first_of("\n\t\v\f\r");
	}
}