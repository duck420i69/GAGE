#include "pch.h"
#include "logger.h"
#include "spdlog/sinks/stdout_color_sinks.h"


Shared<spdlog::logger> Logger::sLogger = spdlog::stdout_color_mt("EnGAGE");
