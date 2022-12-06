/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_starts_with.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mateus <mateus@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 13:55:55 by mateus            #+#    #+#             */
/*   Updated: 2022/12/05 14:00:24 by mateus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "webserv.h"

using namespace std;

bool ft_starts_with(string str, string substring)
{
    return (!ft_strncmp(str, substring, substring.length()));
}