/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbach <jbach@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 14:02:30 by mgoncalv          #+#    #+#             */
/*   Updated: 2023/01/02 19:02:58 by jbach            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEB_SERV_H
# define WEB_SERV_H

#include <iostream>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
using namespace std;

typedef struct s_methods {
	bool	_get;
	bool	_post;
	bool	_delete;
} t_methods;

vector<string>	ft_split(string str, char c);
int				ft_strcmp_fowardslash(string str1, string str2);
void			ft_remove_invalid_spaces(string *str);
void			ft_remove_double_spaces(string *str);
void			ft_wrap_in_spaces(string *str);
void			ft_wrap_brackets_in_spaces(string *str);
string 			ft_read_file_ignoring_comments(ifstream	_configFile);
void			ft_wrap_target_in_spaces(string *str, string target);
bool 			ft_starts_with(string str, string substring);
int ft_strncmp(string str1, string str2, size_t n);
std::vector<std::string>	ft_split_header(std::string str);

#endif

