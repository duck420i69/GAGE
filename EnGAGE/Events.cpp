#include "pch.h"
#include "Events.h"

#include "Window.h"

#include <GLFW/glfw3.h>

float			Events::sXPos = 0;
float			Events::sYPos = 0;
float			Events::sLastXPos = 0;
float			Events::sLastYPos = 0;
std::bitset<5>	Events::sButtonsPressed = { 0 };
bool			Events::sDragging = false;

std::bitset<400> Events::sKeysPressed = { 0 };
std::bitset<400> Events::sPrevKeys = { 0 };


void Events::Init() noexcept
{
	glfwSetErrorCallback(
		[](int error_code, const char* description)
		{
			std::stringstream ss;
			ss << std::string(description) << "\n";
			std::string result = ss.str();
			//TODO: Logger here
		});
	glfwSetCursorPosCallback(Window::GetWindow(),
		[](GLFWwindow* w, double x, double y)
		{
			sXPos = (float)x;
			sYPos = (float)y;
			sDragging = sButtonsPressed[MOUSE_BUTTON_LEFT] || sButtonsPressed[MOUSE_BUTTON_RIGHT];
		});
	glfwSetMouseButtonCallback(Window::GetWindow(),
		[](GLFWwindow* window, int button, int action, int mods)
		{
			if (action == GLFW_PRESS)
				sButtonsPressed[button] = true;
			else if (action == GLFW_RELEASE) {
				sButtonsPressed[button] = false;
				sDragging = false;
			}
		});
	glfwSetKeyCallback(Window::GetWindow(),
		[](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (action == GLFW_PRESS)
				sKeysPressed[key] = true;
			else if (action == GLFW_RELEASE)
				sKeysPressed[key] = false;
		});
	glfwSetFramebufferSizeCallback(Window::GetWindow(),
		[](GLFWwindow* w, int width, int height)
		{
			Window::sWidth = width;
			Window::sHeight = height;
		});
}

void Events::Update() noexcept
{
	sLastXPos = sXPos;
	sLastYPos = sYPos;
	sPrevKeys = sKeysPressed;
}

bool Events::IsButtonDown(int button) noexcept
{
	return sButtonsPressed[button];
}

bool Events::IsDragging() noexcept
{
	return sDragging;
}

float Events::GetX() noexcept
{
	return sXPos;
}

float Events::GetY() noexcept
{
	return sYPos;
}

float Events::GetDX() noexcept
{
	return sXPos - sLastXPos;
}

float Events::GetDY() noexcept
{
	return sYPos - sLastYPos;
}

bool Events::IsKeyDown(int key) noexcept
{
	return sKeysPressed[key];
}

bool Events::IsKeyDownOnce(int key) noexcept
{
	return sKeysPressed[key] && !sPrevKeys[key];
}

