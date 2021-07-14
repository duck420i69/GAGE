#pragma once

#include "spdlog/logger.h"

class Logger
{
	static std::shared_ptr<spdlog::logger> sLogger;
public:
	template<typename Str, typename... Args>
	static void info(Str str, Args&&... message)
	{
		sLogger->info(str, message...);
	}

	template<typename Str, typename... Args>
	static void warn(Str str, Args&&... message)
	{
		sLogger->warn(str, message...);
	}

	template<typename Str, typename... Args>
	static void error(Str str, Args&&... message)
	{
		sLogger->error(str, message...);
	}

};