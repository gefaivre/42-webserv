#ifndef UTILS_HPP
#define UTILS_HPP


#include "general_includes.hpp"
using namespace std;

typedef struct	s_requestData
{
	std::string methode;
	std::string protocol;
	std::string Connection;
	std::string path;
	std::string	fileToSend;
	bool		isIndex;

}				t_requestData;

typedef struct	s_headerData
{
	std::string protocol;
	std::string	statusCode;
	std::string statusMessage;
	std::string contentLength;
	std::string contentType;
	std::string	date;
}				t_headerData;



int fileExist(std::string file_path);

int isDirectory(std::string file_path);

int	ft_define_error(string str);


#endif