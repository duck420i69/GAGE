#include "pch.h"
#include "input.h"

#include <GLFW/glfw3.h>

std::bitset<Input::MAX_KEYS>	Input::sKeys, Input::sPrevKeys;
std::bitset<Input::MAX_BUTTONS> Input::sButtons;

void Input::init(GLFWwindow* pWindow) noexcept
{
	sKeys.reset();
	sPrevKeys.reset();
	sButtons.reset();
	glfwSetKeyCallback(pWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (key != GLFW_KEY_UNKNOWN)
			{
				if (action == GLFW_PRESS)
				{
					sKeys.set(key, true);
				}
				else if (action == GLFW_RELEASE)
				{
					sKeys.set(key, false);
				}
			}
		});
}

void Input::update() noexcept
{
	sPrevKeys = sKeys;
}

bool Input::isKeyDown(int key) noexcept
{
	return sKeys[key];
}

bool Input::isKeyDownOnce(int key) noexcept
{
	return sKeys[key] && !sPrevKeys[key];
}

bool Input::isButtonDown(int button) noexcept
{
	return sButtons[button];
}