#ifndef UTILS_HPP
#define UTILS_HPP


#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
	
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

}				t_headerData;



int fileExist(std::string file_path);

int isDirectory(std::string file_path);

#endif