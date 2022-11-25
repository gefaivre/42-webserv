/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoncalv <mgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 15:00:52 by mgoncalv          #+#    #+#             */
/*   Updated: 2022/11/23 14:06:21 by mgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include "iostream"

// using namespace std;




int main(int argc, char **argv)
{
	if (argc != 2)
	{
		cerr << "Error: bad arguments" << endl;
		return (1);
	}
	string	str1 = "/etc/www/hello/oi/index.html";
	string	str2 = "/etc/www/hello/tchau/index.html";
	ft_strcmp_fowardslash(str1, str2);
	(void) argv;
	// Parser *config = new Parser(argv[1]);
	// (void) config;
	return (0);
}