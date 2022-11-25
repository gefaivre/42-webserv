#include "utils.hpp"



int fileExist(std::string file_path)
{
	struct stat sb;
	int status = stat(file_path.c_str(), &sb);

	return (!(status));
}

int isDirectory(std::string file_path)
{

	// std::cout << ">------------------isDirectory" << std::endl;
	// std::cout << "file_path\t=\t" << file_path << std::endl;
	if (file_path.size() > 0 && file_path[file_path.size() - 1] == '/')
		file_path.erase(file_path.size() - 1);

	struct stat sb;
	if (stat(file_path.c_str(), &sb) == -1)
	{
		// std::cout << "<---------------End-isDirectory-1 (file not found)" << std::endl;
		return (0);

	}

	if (S_ISDIR(sb.st_mode))
	{
		// std::cout << "<---------------End-isDirectory-2 (is a directory)" << std::endl;
		return (1);
	}
	// std::cout << "<---------------End-isDirectory-3 (is not a directory)" << std::endl;
	return (0);
}