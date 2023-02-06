/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbach <jbach@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 14:02:30 by mgoncalv          #+#    #+#             */
/*   Updated: 2023/02/06 14:24:46 by jbach            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEB_SERV_H
# define WEB_SERV_H

#include <iostream>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

typedef struct s_methods {
	bool	_get;
	bool	_post;
	bool	_delete;
} t_methods;

std::vector<std::string>	ft_split(std::string str, char c);
int				ft_strcmp_fowardslash(std::string str1, std::string str2);
void			ft_remove_invalid_spaces(std::string *str);
void			ft_remove_double_spaces(std::string *str);
void			ft_wrap_in_spaces(std::string *str);
void			ft_wrap_brackets_in_spaces(std::string *str);
std::string 			ft_read_file_ignoring_comments(std::ifstream	_configFile);
void			ft_wrap_target_in_spaces(std::string *str, std::string target);
bool 			ft_starts_with(std::string str, std::string substring);
int ft_strncmp(std::string str1, std::string str2, size_t n);
std::vector<std::string>	ft_split_header(std::string str);

#endif

