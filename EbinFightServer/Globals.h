#pragma once

#include <iostream>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <stack>
#include <vector>
#include <fstream>
#include "json.hpp"
#include <map>


using json = nlohmann::json;

bool OpenFile(std::vector<std::string>& lines, const std::string& file_name);

class Global
{
public:
	static sf::Font font;
	static unsigned int win_width;
	static unsigned int win_height;
};

