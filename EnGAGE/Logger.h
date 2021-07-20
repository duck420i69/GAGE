#pragma once

#include <spdlog/logger.h>

class Logger
{
	static std::shared_ptr<spdlog::logger> sLogger;
public:
	template<typename String, typename... Args>
	static void info(const String& str, Args&&... args)
	{
		sLogger->info(str, std::forward<Args>(args)...);
	}

	template<typename String, typename... Args>
	static void warn(const String& str, Args&&... args)
	{
		sLogger->warn(str, std::forward<Args>(args)...);
	}


	template<typename String, typename... Args>
	static void error(const String& str, Args&&... args)
	{
		sLogger->error(str, std::forward<Args>(args)...);
	}
};