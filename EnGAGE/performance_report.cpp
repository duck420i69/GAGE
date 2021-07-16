#include "pch.h"
#include "performance_report.h"

#include "imgui/imgui.h"

#include <tchar.h>
#include <Pdh.h>
#include <Psapi.h>

static ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
static int numProcessors;
static HANDLE self;

float PerformanceReport::sAccumulatedTime = 0.0f;

void PerformanceReport::init() noexcept
{
	SYSTEM_INFO sysInfo;
	FILETIME ftime, fsys, fuser;

	GetSystemInfo(&sysInfo);
	numProcessors = sysInfo.dwNumberOfProcessors;

	GetSystemTimeAsFileTime(&ftime);
	memcpy(&lastCPU, &ftime, sizeof(FILETIME));

	self = GetCurrentProcess();
	GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
	memcpy(&lastSysCPU, &fsys, sizeof(FILETIME));
	memcpy(&lastUserCPU, &fuser, sizeof(FILETIME));
}

void PerformanceReport::reportToImGui(float delta) noexcept
{
	static double currentCpuUsage = 0.0f;
	static double currentMemoryUsage = 0.0f;
	sAccumulatedTime += delta;
	if (sAccumulatedTime > SECOND)
	{
		currentCpuUsage = getCpuUsage();
		currentMemoryUsage = getMemoryUsage();
		sAccumulatedTime = 0.0f;
	}
	ImGui::Begin("Performance report");
	ImGui::Text("Cpu usage: "); ImGui::SameLine();
	ImGui::ProgressBar(currentCpuUsage, ImVec2(100, 0));
	ImGui::Text("Memory usage: %.6f MB", currentMemoryUsage);
	ImGui::End();
}

double PerformanceReport::getCpuUsage() noexcept
{
	FILETIME ftime, fsys, fuser;
	ULARGE_INTEGER now, sys, user;
	double percent;

	GetSystemTimeAsFileTime(&ftime);
	memcpy(&now, &ftime, sizeof(FILETIME));

	GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
	memcpy(&sys, &fsys, sizeof(FILETIME));
	memcpy(&user, &fuser, sizeof(FILETIME));
	percent = (sys.QuadPart - lastSysCPU.QuadPart) +
		(user.QuadPart - lastUserCPU.QuadPart);
	percent /= (now.QuadPart - lastCPU.QuadPart);
	percent /= numProcessors;
	lastCPU = now;
	lastUserCPU = user;
	lastSysCPU = sys;

	return (double)percent;
}

double PerformanceReport::getMemoryUsage() noexcept
{
	PROCESS_MEMORY_COUNTERS_EX pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
	return (double)(pmc.WorkingSetSize / 1024000.0);
}
