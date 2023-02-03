/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoncalv <mgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 13:16:29 by mateus            #+#    #+#             */
/*   Updated: 2023/02/02 15:19:06 by mgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

int ft_strcmp(std::string str1, std::string str2)
{
    size_t i;

    i = 0;
    while (str1[i] && str1[i] == str2[i])
        i++;
    return (str1[i] - str2[i]);
}