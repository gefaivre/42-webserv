/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateus <mateus@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 13:30:12 by mateus            #+#    #+#             */
/*   Updated: 2022/12/05 13:55:25 by mateus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

using namespace std;

int ft_strncmp(string str1, string str2, size_t n)
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