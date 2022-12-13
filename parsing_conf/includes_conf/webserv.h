/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbach <jbach@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 14:02:30 by mgoncalv          #+#    #+#             */
/*   Updated: 2022/12/13 15:47:23 by jbach            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEB_SERV_H
# define WEB_SERV_H

#include <iostream>
#include <vector>

using namespace std;

vector<string>	ft_split(string str, char c);

char **ft_split(char *str);

#endif

