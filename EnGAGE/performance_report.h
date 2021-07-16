#pragma once


class PerformanceReport
{
	static constexpr float SECOND = 1000;
	static float sAccumulatedTime;
public:
	static void init() noexcept;
	static void reportToImGui(float delta) noexcept;
private:
	static double getCpuUsage() noexcept;
	static double getMemoryUsage() noexcept;
};