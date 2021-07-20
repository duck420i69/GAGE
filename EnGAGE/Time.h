#pragma once

class Time
{
	static double sStartTime;
public:
	static void Init() noexcept;

	static double GetTime() noexcept;
};