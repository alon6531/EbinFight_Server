#include "Globals.h"

sf::Font Global::font = sf::Font("..\\arial.ttf");
unsigned int Global::win_width = 0;
unsigned int Global::win_height = 0;

bool OpenFile(std::vector<std::string>& lines, const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Globals:ERROR::CANT_OPEN_FILE: " << filename << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            line.erase(0, pos + 1);
        }

        // Trim spaces (optional)
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);

        // Skip empty lines
        if (line.empty()) continue;

        lines.push_back(line);
    }
}