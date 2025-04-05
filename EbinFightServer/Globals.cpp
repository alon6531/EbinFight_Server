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
