#include "Globals.h"


const std::string& Package::Pack(const std::string& action, const std::string& data)
{
    return action + "|" + data;
}

std::map<std::string, std::string> Package::Unpack(const std::string& massage)
{
    std::map<std::string, std::string> result;

    size_t pos = massage.find('|');
    if (pos != std::string::npos) {
        result["action"] = massage.substr(0, pos);
        result["data"] = massage.substr(pos + 1);
    }
    else {
        result["action"] = "error";
        result["data"] = "error";
    }
    return result;
}

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