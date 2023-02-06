#ifndef UTILS_HPP
#define UTILS_HPP

#include "define.hpp"
#include <iostream>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sstream>
#include <limits.h>
#include <map>



typedef struct	s_requestData
{
	std::string methode;
	std::string protocol;
	std::string Connection;
	std::string path;
	std::string pathKey;
	std::string	fileToSend;
	std::string _cgiResponse;
	std::string	getParams;
	bool		isCgi;
	bool		isIndex;
	bool		isRedirection;

}				t_requestData;

typedef struct	s_headerData
{
	std::string protocol;
	std::string	statusCode;
	std::string statusMessage;
	std::string contentLength;
	std::string contentType;
	std::string	date;
	std::string	connection;
	bool		isRedirection;
}				t_headerData;



int fileExist(std::string file_path);

std::vector<std::string>	ft_split_vector_string(std::string str, char c);
std::vector<std::string>	ft_split_vector_string_file(std::string str, char c);


int isDirectory(std::string file_path);

int	ft_define_error(std::string str);

std::string itos(int nb);

std::string ft_pwd();

std::vector<std::string>	ft_split_chunked_request(std::string str);
std::vector<std::string>	ft_split_vector_string_file(std::string str, char c, std::string boundary);
std::string getRequestFile(std::string firstReq, std::string *getParams);

std::string	ft_find_boundary_utils(std::map<std::string, std::string> requestmap);


std::string itos(int nb);

std::string ft_pwd();

std::vector<std::string>	ft_split_chunked_request(std::string str);
std::vector<std::string>	ft_split_vector_string_file(std::string str, char c, std::string boundary);
std::string getRequestFile(std::string firstReq, std::string *getParams);

std::string	ft_find_boundary_utils(std::map<std::string, std::string> requestmap);

void	ft_exit_parsing(std::string message);


#endif