#include "pch.h"
#include "Logger.h"

#include <spdlog/sinks/stdout_color_sinks.h>

std::shared_ptr<spdlog::logger> Logger::sLogger = spdlog::stdout_color_mt("EnGAGE");

