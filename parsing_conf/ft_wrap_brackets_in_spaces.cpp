/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wrap_brackets_in_spaces.cpp                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoncalv <mgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 17:17:25 by mgoncalv          #+#    #+#             */
/*   Updated: 2023/02/02 15:37:45 by mgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

void	ft_wrap_target_in_spaces(std::string *str, std::string target)
{
	size_t	find;

	find = str->find(target);
	while (find != std::string::npos)
	{

	//TODO: aqui temos que tomar cuidado pois se no primeiro if ele adiciona um espaco. O index muda;
	//Por enquanto corrigimos isso com o mais um, mas na verdade precisamos fazer find++ se ele entrar no primeiro if;
		if (find == 0 && (*str)[find] && (*str)[find] != ' ')
		{
			str->insert(0, 1, ' ');
		}
		else if (find > 0 && (*str)[find - 1] && (*str)[find - 1] != ' ')
		{
			str->insert(find, " ");
		}
		find = str->find(target, find + target.length() + 1);
		
	}
	find = str->find(target);
	while (find != std::string::npos)
	{

		if ((*str)[find + target.length()] != ' ')
		{
			str->insert(find + target.length(), " ");
			find++;
		}
		find = str->find(target, find + target.length() + 1);
	}


}

void	ft_wrap_brackets_in_spaces(std::string *str)
{
	ft_wrap_target_in_spaces(str, "{");
	ft_wrap_target_in_spaces(str, "}");
}


// int	main()
// {
// 	string str1 = "amateusa";
// 	ft_wrap_target_in_spaces(&str1, "amateusa");
// 	cout << str1 << endl;
// }