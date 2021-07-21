#include "pch.h"
#include "Utils.h"

std::vector<std::string> Utils::SplitString(std::string str, const std::string& delim) noexcept
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

std::string Utils::LoadFile(const std::string& path) noexcept(false)
{
    std::stringstream ss;
    std::ifstream f;
    f.exceptions(std::ios::failbit | std::ios::badbit);

    f.open(path);
    ss << f.rdbuf();
    return ss.str();
}

