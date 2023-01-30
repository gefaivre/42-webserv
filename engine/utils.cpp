#include "utils.hpp"

// using namespace std;

int fileExist(string file_path)
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
	while (end < str.length())
	{
		if (file_end != 0)
		{
			end = (file_end - file_start) + 2;
			file_end = 0;
		}
		else
			end = str.find(c, start);
		std::cout<< YEL<< "START =" << start <<reset<<std::endl;
		std::cout<< YEL<< "END =" << end <<reset<<std::endl;
		// if (start > 172532)
		// 	exit(1);

		std::string smallstr = str.substr(start, end - start);
		if (start > 100 && start < 300)
			std::cout << "." << smallstr << "." << std::endl;
		arr.push_back(smallstr);

		start = end + 1;

		size_t colon_equal = smallstr.find("Content-Type:");
		// --> premier \n apres colon_equal
		//substr entre \n + 1 jusque a find (boundary)
		//Enlever le premier letre si \n et dernier letre si \n
		//start boundary
		if (colon_equal != std::string::npos)
		{
			// std::cout << "Boundary" << boundary <<std::endl;
			colon_equal = str.find("Content-Type:");
			file_start = str.find('\n', colon_equal);
			file_end = str.find(boundary, file_start);
			// std::cout<< "str = "<< str <<std::endl;
			// std::cout << "ICI: -->" << str.substr(file_start + 1, (file_end - file_start) + 1)<<"End"<< std::endl;
			end = (file_end - file_start) + 2;
			start = file_start +1;
			std::cout<< YEL<< "start =" << start <<reset<<std::endl;
			std::cout<< YEL<< "COLO eND =" << end <<reset<<std::endl;
			// arr.push_back(str.substr(file_start + 1, (file_end - file_start) + 1));
		}

	}
	if (str[str.length() - 1] != c)
		arr.push_back(str.substr(start, str.length() - start));
	return (arr);
}

int isDirectory(string file_path)
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

int	ft_define_error(string str)
{
	string std_str(str);
	cout << std_str << endl;
	cout << strerror(errno) << endl;
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
		pos_equal = it->second.find_last_of('=');
		if (pos_equal == std::string::npos)
			throw std::exception();
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