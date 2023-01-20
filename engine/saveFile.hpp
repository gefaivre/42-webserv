#ifndef SAVEFILE_HPP
#define SAVEFILE_HPP

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include "utils.hpp"

void saveFile(std::map<std::string, std::string> requestmap, std::string requestBody);
std::map<std::string, std::string>  transformBodyStringtoMap(std::map<std::string, std::string> requestmapBody, std::map<std::string, std::string> requestmap, std::string requestBody);

#endif