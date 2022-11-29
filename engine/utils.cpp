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

	// cout << ">------------------isDirectory" << endl;
	// cout << "file_path\t=\t" << file_path << endl;
	if (file_path.size() > 0 && file_path[file_path.size() - 1] == '/')
		file_path.erase(file_path.size() - 1);

	struct stat sb;
	if (stat(file_path.c_str(), &sb) == -1)
	{
		// cout << "<---------------End-isDirectory-1 (file not found)" << endl;
		return (0);

	}

	if (S_ISDIR(sb.st_mode))
	{
		// cout << "<---------------End-isDirectory-2 (is a directory)" << endl;
		return (1);
	}
	// cout << "<---------------End-isDirectory-3 (is not a directory)" << endl;
	return (0);
}

int	ft_define_error(string str)
{
	string std_str(str);
	cout << std_str << endl;
	cout << strerror(errno) << endl;
	return(errno);
}