#pragma once

#include "spdlog/logger.h"

class Logger
{
	static std::shared_ptr<spdlog::logger>	sLogger;
	static std::stringstream				sConsoleBuffer;
public:
	static void init() noexcept;
	static void renderImGui() noexcept;

	template<typename Str, typename... Args>
	static void info(Str str, Args&&... message) noexcept
	{
		sLogger->info(str, message...);
	}

	template<typename Str, typename... Args>
	static void warn(Str str, Args&&... message) noexcept
	{
		sLogger->warn(str, message...);
	}

	template<typename Str, typename... Args>
	static void error(Str str, Args&&... message) noexcept
	{
		sLogger->error(str, message...);
	}
private:
	static void dumpToFile() noexcept;
};