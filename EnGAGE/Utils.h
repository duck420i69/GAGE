#pragma once


class Utils
{
public:
	static std::vector<std::string> SplitString(std::string str, const std::string& delim) noexcept;
	static std::string LoadFile(const std::string& path) noexcept(false);
};