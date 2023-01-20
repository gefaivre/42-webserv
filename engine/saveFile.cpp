#include "saveFile.hpp"


std::string	ft_remove_not_print(std::string str)
{
	std::string::iterator it;
	std::string str2;
	for (it = str.begin(); it != str.end(); it++)
	{
		if (isprint(*it))
			str2 += *it;
	}
	std::cout << "str2 = ." << str2 << "."<< std::endl;
	return (str2);
}

std::string findBodyKey(std::vector<std::string> vector, size_t i)
{
	std::string key;
	if (vector[i].find("Content-Disposition") != std::string::npos)
	{
		size_t colon_equal = vector[i].find("=");
		size_t size_type = 0;
		if (colon_equal != std::string::npos)
		{
			for (int tmp = colon_equal + 2; vector[i][tmp] != '\"'; tmp++)
				size_type++;
			key = vector[i].substr((colon_equal + 2),size_type);
		}
	}
	return (key);
}
std::string findBodyValue(std::vector<std::string> vector, size_t i, std::string value, std::string key)
{
	size_t colon_r = vector[i].find("\r");
	if (colon_r != std::string::npos)
	{
		if (colon_r == 0)
			vector[i].clear();
	}
	size_t content_type = vector[i].find("Content-Type");
	if (content_type != std::string::npos)
		vector[i].clear();		
	value = vector[i];
	if (key == "file")
		value += "\n";
	return (value);
}

std::map<std::string, std::string>  transformBodyStringtoMap(std::map<std::string, std::string> requestmapBody, std::map<std::string, std::string> requestmap, std::string requestBody)
{
	std::string value;
	std::string key;
    std::vector<std::string> vector;
	// std::string boundary;
	try
	{
		vector = ft_split_vector_string(requestBody, '\n');
		std::string boundary = ft_find_boundary(requestmap);
		for (size_t i = 1; i < vector.size(); i++)
		{
			size_t colon_boundary = vector[i].find(boundary);
			if (colon_boundary == std::string::npos)
			{
				if (vector[i].find("Content-Disposition") != std::string::npos)
					key = findBodyKey(vector, i);
				else
				{
					value += findBodyValue(vector, i, value, key);
					if (key != "file")
						value = ft_remove_not_print(value);
					// if (key == "file")
					// {
					// 	std::cout << RED<<vector[i] << "FILE = " << vector[i].find("filename")<<reset<< std::endl;
					// }
				}
			}
			else
			{
				requestmapBody.insert(std::pair<std::string, std::string>(key, value));
				value.clear();
				key.clear();
			}
		}
	}
	catch (std::exception &e)
	{
		vector = ft_split_vector_string(requestBody, '&');
		for (size_t i = 0; i < vector.size(); i++)
		{
			size_t colon_equal = vector[i].find('=');
			requestmapBody.insert(std::pair<std::string, std::string>(vector[i].substr(0, colon_equal), vector[i].substr(colon_equal + 1)));
		}
		return (requestmapBody);
	}
	return (requestmapBody);
}




void saveFile(std::map<std::string, std::string> requestmap, std::string requestBody)
{
	//TODO: remettre les spaces ds le file 
	std::cout << "** Save file **" << std::endl;
	std::map<std::string, std::string> requestmapBody;
	requestmapBody = transformBodyStringtoMap(requestmapBody, requestmap, requestBody);

	int isempty = 1;
	std::map<std::string,std::string>::iterator it_file;
	std::map<std::string,std::string>::iterator it_name;
	it_file = requestmapBody.find("file");
	it_name = requestmapBody.find("name");
	if (it_file != requestmapBody.end())
	{
		for (size_t i = 0; it_file->second[i];i++)
		{
			if (isprint(it_file->second[i]))
			{
				isempty = 0;
				break;
			}
		}
		if (!isempty)
		{
			std::cout <<YEL <<  "FILE = " << it_name->second<< WHT<< std::endl;
			//open a file in write mode
			ofstream outfile;
			std::string new_path = "www/new_files/";
			mkdir(new_path.c_str(), 0777);
			outfile.open(new_path.append(it_name->second).c_str());
			//write the string
			outfile << it_file->second.substr(2, it_file->second.size() - 3);
			outfile.close();
		}
	}
}