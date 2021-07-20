#include "pch.h"
#include "Time.h"

#include <GLFW/glfw3.h>

double Time::sStartTime = 0;

void Time::Init() noexcept
{
	sStartTime = glfwGetTime();
}

double Time::GetTime() noexcept
{
	return glfwGetTime() - sStartTime;
}
