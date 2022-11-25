/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wrap_in_spaces.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoncalv <mgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 16:38:14 by mgoncalv          #+#    #+#             */
/*   Updated: 2022/11/25 17:41:03 by mgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

using namespace std;


void	ft_wrap_in_spaces(string *str)
{
	//TODO: podemos usar a funcao wrap target in space, o target seria a propria string;
	//Contra: menos performance pois tem que comparar a string inteira.
	if ((*str)[0] != ' ')
		str->insert(0, " ");
	if ((*str)[str->length() - 1] != ' ')
		str->insert(str->length(), " ");
}