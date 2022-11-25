/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wrap_brackets_in_spaces.cpp                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoncalv <mgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 17:17:25 by mgoncalv          #+#    #+#             */
/*   Updated: 2022/11/25 18:00:56 by mgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

using namespace std;

void	ft_wrap_target_in_spaces(string *str, string target)
{
	size_t	find;

	find = str->find(target);
	while (find != string::npos)
	{

	//TODO: aqui temos que tomar cuidado pois se no primeiro if ele adiciona um espaco. O index muda;
	//Por enquanto corrigimos isso com o mais um, mas na verdade precisamos fazer find++ se ele entrar no primeiro if;
		if (find > 0 && (*str)[find - 1] != ' ')
			str->insert(find, " ");
	//TODO: cuidado: aqui assumimos que target tem somente uma letra
		if (find > 0 && (*str)[find + 1] != ' ')
			str->insert(find + 1, " ");
		find = str->find(target, find + 1);
	}
}

void	ft_wrap_brackets_in_spaces(string *str)
{
	ft_wrap_target_in_spaces(str, "{");
	ft_wrap_target_in_spaces(str, "}");
}
