/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateus <mateus@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 13:16:29 by mateus            #+#    #+#             */
/*   Updated: 2022/12/05 13:26:47 by mateus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

using namespace std;

int ft_strcmp(string str1, string str2)
{
    size_t i;

    i = 0;
    while (str1[i] && str1[i] == str2[i])
        i++;
    return (str1[i] - str2[i]);
}