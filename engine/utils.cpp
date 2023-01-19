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
