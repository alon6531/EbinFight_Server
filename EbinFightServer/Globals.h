#pragma once

#include <iostream>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include <map>

bool OpenFile(std::vector<std::string>& lines, const std::string& file_name);

struct Package {

	static const std::string& Pack(const std::string& action, const std::string& data);
	static std::map<std::string, std::string> Unpack(const std::string& massage);
};

