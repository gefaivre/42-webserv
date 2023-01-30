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
	// size_t colon_r = vector[i].find("\r");
	// if (colon_r != std::string::npos)
	// {
	// 	if (colon_r == 0)
	// 		vector[i].clear();
	// }
	size_t content_type = vector[i].find("Content-Type");
	// if (content_type != std::string::npos)
	// 	vector[i].clear();
	// else
	if (content_type == std::string::npos)
	{
		// std::cout << "Start check:"<< std::endl;
		// for (size_t i = 0; i < vector.size(); i++)
		// {
		// 	std::cout << vector[i] << std::endl;
		// 	for (size_t j = 0; j < vector[i].size(); j++)
		// 	{
		// 		std::cout << (int) vector[i][j] << ".";
		// 	}
		// 	std::cout << "***"<< std::endl;
		// }
		// std::cout << "End check:"<< std::endl;
	// std::cout << "vector = ." << vector[i] << "."<<std::endl;
		if (!vector[i].empty())
		// if (!vector[i].empty() && !(vector[i].size() == 1 && vector[i][0] == '\n'))
			value = vector[i];
		// std::cout << "value = " << value << "." <<std::endl;
		// std::cout << "Vector = " << vector[i] << ". size = " << colon_r << std::endl;
		// if (colon_r != 0)
	}
	// if (value.empty() && colon_r != 0)
	// 	value = "\n";
	// std::cout << "VALEUR = ." << value << "." << std::endl;
	return (value);
}

std::map<std::string, std::string>  transformBodyStringtoMap(std::map<std::string, std::string> requestmapBody, std::map<std::string, std::string> requestmap, std::string requestBody)
{
	std::string value;
	std::string key;
    std::vector<std::string> vector;
	try
	{
		// for (size_t i = 0; i < vector.size(); i++)
		// 	std::cout << YEL << vector[i] << reset <<std::endl;
	
		std::string boundary = ft_find_boundary_utils(requestmap);
		std::cout <<boundary <<std::endl;
		vector = ft_split_vector_string_file(requestBody, '\r', boundary);
		std::cout <<"REQUEST BODY = "<<requestBody <<std::endl;
		for (size_t i = 0; i < vector.size(); i++)
		{
			// std::cout<< RED << "VECOT = " <<  vector[i]<< "."<<reset << std::endl;
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
						// std::cout << YEL<< " *******" << reset<< std::endl;
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
	//TODO: remettre les spaces ds le file 
	std::cout << "** Save file **" << std::endl;
	// exit(1);
	std::map<std::string, std::string> requestmapBody;
	requestmapBody = transformBodyStringtoMap(requestmapBody, requestmap, requestBody);
	std::map<std::string,std::string>::iterator it;
	if (requestmapBody.empty())
		return (false);
	// for (it = requestmapBody.begin(); it != requestmapBody.end(); it++)
	// {
	// 	std::cout << "first = " << it->first << "second = " << it->second << std::endl;
	// }
	for (it = requestmapBody.begin(); it != requestmapBody.end(); it++)
	{
		ofstream outfile;
		std::string new_path = "www/new_files/";
		mkdir(new_path.c_str(), 0777);
		outfile.open(new_path.append(it->first).c_str());
		//write the string
		// outfile << it->second;
		// std::cout << YEL << "first = " << it->first << "====second = " << it->second.substr(1, it->second.size()) << reset<< std::endl;
		outfile << it->second.substr(2, it->second.size());
		// outfile << it->second.substr(1, it->second.size() - 2);
		outfile.close();
	}
	// std::map<std::string,std::string>::iterator it_file;
	// std::map<std::string,std::string>::iterator it_name;
	// it_file = requestmapBody.find("file");
	// it_name = requestmapBody.find("name");
	// if (it_file != requestmapBody.end())
	// {
	// 	// for (size_t i = 0; it_file->second[i];i++)
	// 	// {
	// 	// 	if (isprint(it_file->second[i]))
	// 	// 	{
	// 	// 		isempty = 0;
	// 	// 		break;
	// 	// 	}
	// 	// }
	// 	// if (!isempty)
	// 	// {
	// 		std::cout <<YEL <<  "FILE = " << it_name->second<< WHT<< std::endl;
	// 		//open a file in write mode
	// 		ofstream outfile;
	// 		std::string new_path = "www/new_files/";
	// 		mkdir(new_path.c_str(), 0777);
	// 		outfile.open(new_path.append(it_name->second).c_str());
	// 		//write the string
	// 		outfile << it_file->second.substr(2, it_file->second.size() - 3);
	// 		outfile.close();
	// 	// }
	// }
	return true;
}