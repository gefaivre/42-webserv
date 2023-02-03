/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoncalv <mgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 13:30:12 by mateus            #+#    #+#             */
/*   Updated: 2023/02/02 15:19:12 by mgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

int ft_strncmp(std::string str1, std::string str2, size_t n)
{
    size_t i;

    i = 0;
    while (n--)
    {
        if (str1[i] != str2[i] || str1[i] == 0)
            return (str1[i] - str2[i]);
        i++;
    }
    return (0);
}