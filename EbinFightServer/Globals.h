#pragma once

#include <iostream>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>



struct Package {

	static const std::string& Pack(const std::string& action, const std::string& data);
	static std::map<std::string, std::string> Unpack(const std::string& massage);
};

