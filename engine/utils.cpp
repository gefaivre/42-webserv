#include "utils.hpp"

// using namespace std;

int fileExist(string file_path)
{
	struct stat sb;
	int status = stat(file_path.c_str(), &sb);

	return (!(status));
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
