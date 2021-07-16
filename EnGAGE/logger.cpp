#include "pch.h"
#include "logger.h"
#include "spdlog/sinks/ostream_sink.h"

#include "imgui/imgui.h"

std::shared_ptr<spdlog::logger> Logger::sLogger;
std::stringstream				Logger::sConsoleBuffer;

void Logger::init() noexcept
{
	try
	{
		auto sink = std::make_shared<spdlog::sinks::ostream_sink_st>(sConsoleBuffer);
		sLogger = std::make_shared<spdlog::logger>("EnGAGE", sink);	
	}
	catch (std::bad_alloc& e)
	{
		Logger::error("Exception thrown: {}", e.what());
	}
}

void Logger::renderImGui() noexcept
{
	std::string str = sConsoleBuffer.str();
	ImGui::Begin("Console from logger");
	if (ImGui::Button("Clear"))
	{
		sConsoleBuffer.str("");
	}
	ImGui::SameLine();
	if (ImGui::Button("Dump to file"))
	{
		dumpToFile();
	}
	ImGui::BeginChild("text", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
	ImGui::TextUnformatted(str.c_str());
	ImGui::SetScrollHereY(1.0f);
	ImGui::EndChild();
	ImGui::End();
}

void Logger::dumpToFile() noexcept
{
	std::ofstream log("log", std::ios_base::out);

	log << sConsoleBuffer.str();
	log.close();
}