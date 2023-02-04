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
	return (str2);
}

std::string findBodyKey(std::vector<std::string> vector, size_t i)
{
	std::string key;
	if (vector[i].find("Content-Disposition") != std::string::npos)
	{
		size_t colon_equal = vector[i].find("filename=");
		size_t size_type = 0;
		if (colon_equal != std::string::npos)
		{
			colon_equal += strlen("filename=\"");
			for (int tmp = colon_equal; vector[i][tmp] != '\"'; tmp++)
				size_type++;
			key = vector[i].substr((colon_equal),size_type);
		}
	}
	return (key);
}
std::string findBodyValue(std::vector<std::string> vector, size_t i, std::string value)
{
	size_t content_type = vector[i].find("Content-Type");
	if (content_type == std::string::npos)
	{
		if (!vector[i].empty())
			value = vector[i];
	}
	return (value);
}

std::map<std::string, std::string>  transformBodyStringtoMap(std::map<std::string, std::string> requestmapBody, std::map<std::string, std::string> requestmap, std::string requestBody)
{
	std::string value;
	std::string key;
    std::vector<std::string> vector;
	try
	{
		try
		{
			std::string boundary = ft_find_boundary_utils(requestmap);
			vector = ft_split_vector_string_file(requestBody, '\r', boundary);
			for (size_t i = 0; i < vector.size(); i++)
			{
				size_t colon_boundary = vector[i].find(boundary);
				if (colon_boundary == std::string::npos)
				{
					if (vector[i].find("Content-Disposition") != std::string::npos)
						key = findBodyKey(vector, i);
					else
					{
						if (!key.empty())
						{
							value += findBodyValue(vector, i, value);
						}
					}
				}
				else
				{
					if (!key.empty())
					{
						requestmapBody.insert(std::pair<std::string, std::string>(key, value));
						value.clear();
						key.clear();
					}
				}
			}
		}
		catch (std::exception &e)
		{
			return (requestmapBody);
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




bool saveFile(std::map<std::string, std::string> requestmap, std::string requestBody)
{
	std::map<std::string, std::string> requestmapBody;
	requestmapBody = transformBodyStringtoMap(requestmapBody, requestmap, requestBody);
	std::map<std::string,std::string>::iterator it;
	if (requestmapBody.empty())
		return (false);

	for (it = requestmapBody.begin(); it != requestmapBody.end(); it++)
	{
		std::ofstream outfile;
		std::string new_path = "html/new_files/";
		mkdir(new_path.c_str(), 0777);
		outfile.open(new_path.append(it->first).c_str());
		outfile << it->second.substr(2, it->second.size());
		outfile.close();
	}
	return true;
}