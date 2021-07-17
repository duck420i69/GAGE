#include "pch.h"
#include "clock.h"

#include <GLFW/glfw3.h>



TClock::TClock() noexcept :
	mDelta(0.0f),
	mPrevTime(0.0f)
{
}


void TClock::init() noexcept
{
	mPrevTime = glfwGetTime();
}

void TClock::tick() noexcept
{
	mDelta = glfwGetTime() - mPrevTime;
	mPrevTime = glfwGetTime();
}
TClock& TClock::get() noexcept
{
	static TClock instance;
	return instance;
}
