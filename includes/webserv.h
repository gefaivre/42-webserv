/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoncalv <mgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 14:02:30 by mgoncalv          #+#    #+#             */
/*   Updated: 2022/11/29 14:25:46 by mgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEB_SERV_H
# define WEB_SERV_H

#include <iostream>
#include <vector>

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
#endif

