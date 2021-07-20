#include "pch.h"
#include "Utils.h"

std::vector<std::string> Utils::SplitString(std::string str, const std::string& delim)
{
    size_t pos = 0;
    std::vector<std::string> tokens;
    while ((pos = str.find(delim)) != std::string::npos)
    {
        std::string token;
        token = str.substr(0, pos);
        tokens.push_back(token);
        str.erase(0, pos + delim.length());
    }
    return tokens;
}
