#include "utils.hpp"

// using namespace std;

int fileExist(std::string file_path)
{
	struct stat sb;
	int status = stat(file_path.c_str(), &sb);

	return (!(status));
}

std::vector<std::string>	ft_split_vector_string(std::string str, char c)
{
	size_t			start;
	size_t			end;
	std::vector<std::string>	arr;
	
	start = 0;
	while ((end = str.find(c, start)) && end < str.length())
	{
		arr.push_back(str.substr(start, end - start));
		start = end + 1;
	}
	if (str[str.length() - 1] != c)
		arr.push_back(str.substr(start, str.length() - start));
	return (arr);
}


std::vector<std::string>	ft_split_vector_string_file(std::string str, char c, std::string boundary)
{
	size_t			start;
	size_t			end = 0;
	std::vector<std::string>	arr;
	
	start = 0;
	size_t file_end = 0;
	size_t file_start = 0;
	size_t colon_equal = 0;
	size_t colon_no_null = 0;
	while (end < str.length())
	{
		if (file_end != 0)
		{
			end = file_end -1;
			start = file_start + 1;
			std::string smallstr = str.substr(start, end - start);
			file_end = 0;
		}
		else
		{
			std::cout <<"start = " << start << std::endl;
			end = str.find(c, start);
		}
		std::string smallstr = str.substr(start, end - start);
		std::cout << YEL << smallstr << reset << std::endl;
		arr.push_back(smallstr);
		start = end + 1;

		colon_equal = smallstr.find("Content-Type:");
		if (colon_equal != std::string::npos)
		{
			colon_equal = str.find("Content-Type:", colon_no_null + 1);
			colon_no_null = colon_equal;
			file_start = str.find('\n', colon_equal);
			file_end = str.find(boundary, file_start) - 3;
		}
		// exit(1);


	}
	if (str[str.length() - 1] != c)
		arr.push_back(str.substr(start, str.length() - start));
	return (arr);
}

int isDirectory(std::string file_path)
{

	if (file_path.size() > 0 && file_path[file_path.size() - 1] == '/')
		file_path.erase(file_path.size() - 1);

	struct stat sb;
	if (stat(file_path.c_str(), &sb) == -1)
	{
		return (0);

	}
	if (S_ISDIR(sb.st_mode))
	{
		return (1);
	}
	return (0);
}

int	ft_define_error(std::string str)
{
	std::string std_str(str);
	std::cout << std_str << std::endl;
	std::cout << strerror(errno) << std::endl;
	return(errno);
}

std::string itos(int nb)
{
	std::stringstream strs;
	strs << nb;
	std::string temp_str = strs.str();
	return (temp_str);
}

std::string ft_pwd()
{
	char buffer[PATH_MAX+ 1];

	if (getcwd(buffer, PATH_MAX))
	{
		std::string buf = buffer;
		return (buf);
	}
	else
		return ("PWD: Failed to retrieve current directory");
}


std::vector<std::string>	ft_split_chunked_request(std::string str)
{
	size_t			start;
	size_t			end;
	std::vector<std::string>	arr;
	std::string one_line;
	
	char c = '\n';
	start = 0;
	// str.erase(str.find("\r"), str.size());
	while ((end = str.find(c, start)) && end < str.length())
	{
		one_line = str.substr(start, end - start);
		if (one_line[one_line.size() - 1] == '\r')
			one_line = one_line.substr(0, one_line.size() - 1);
		arr.push_back(one_line);
		start = end + 1;
	}
	if (str[str.length() - 1] != c)
	{
		one_line = str.substr(start, str.length() - start);
		if (one_line[one_line.size() - 1] == '\r')
			one_line = one_line.substr(0, one_line.size() - 1);
		arr.push_back(one_line);
	}
	return (arr);
}

std::string	ft_find_boundary_utils(std::map<std::string, std::string> requestmap)
{
	std::string boundary;
	size_t pos_equal = 0;
	std::map<std::string,std::string>::iterator it;
	it = requestmap.find("Content-Type");
	if (it == requestmap.end())
		it = requestmap.find("content-type");
	if (it != requestmap.end())
	{
		std::cout << "it->first" << it->first<< std::endl;
		std::cout << "it->second" << it->second<< std::endl;
		pos_equal = it->second.find_last_of('=');
		if (pos_equal == std::string::npos)
			throw std::exception();
		else if (pos_equal == 0)
			return (NULL);
		boundary = it->second.substr(pos_equal + 1);
	}
	// std::cout << "Bound = ." << boundary + '\n' + '0'<<"." << std::endl;
	return (boundary);
}

std::string getRequestFile(std::string firstReq, std::string *getParams)
{
	size_t pos_space = firstReq.find_last_of(' ');
	size_t pos_slash = firstReq.find_first_of('/');
	std::string requestFile;

	requestFile = firstReq.substr(pos_slash + 1, pos_space - (pos_slash + 1));
	if (getParams != NULL)
		*getParams = requestFile.substr(requestFile.find_first_of('?') + 1);
	requestFile =requestFile.substr(0, requestFile.find_first_of('?'));
	return (requestFile);
}