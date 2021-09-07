#include "pch.h"
#include "Window.h"

#include <imgui.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

uint32_t Window::sWidth = 0, Window::sHeight = 0;
GLFWwindow* Window::sWindow = nullptr;
std::string Window::sTitle = "";
bool Window::sCursorEnabled = true;

void Window::Create(uint32_t width, uint32_t height, const std::string& title) noexcept
{
	sWidth = width; sHeight = height; sTitle = title;
	if (!glfwInit())
		assert(!"That bai khoi dong glfw !");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifndef NDEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#else
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_FALSE);
#endif // !NDEBUG

	if (!(sWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr)))
	{
		assert(!"That bai khoi tao window !");
	}
	glfwMakeContextCurrent(sWindow);
	glfwSwapInterval(1);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glfwSetInputMode(sWindow, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
}

void Window::Destroy() noexcept
{
	glfwDestroyWindow(sWindow);
	glfwTerminate();
}

bool Window::IsCloseRequested() noexcept
{
	return glfwWindowShouldClose(sWindow);
}

void Window::Update() noexcept
{
	glfwSwapBuffers(sWindow);
	glfwPollEvents();
}

const uint32_t& Window::GetWidth() noexcept
{
	return sWidth;
}

const uint32_t& Window::GetHeight() noexcept
{
	return sHeight;
}

GLFWwindow* Window::GetWindow() noexcept
{
	return sWindow;
}

void Window::EnableCursor() noexcept
{
	sCursorEnabled = true;
	glfwSetInputMode(sWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
}

void Window::ToggleCursor() noexcept
{
	sCursorEnabled = !sCursorEnabled;

	if (sCursorEnabled)
		EnableCursor();
	else
		DisableCursor();
}

void Window::DisableCursor() noexcept
{
	sCursorEnabled = false;
	glfwSetInputMode(sWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
}

bool Window::IsCursorEnabled() noexcept
{
	return sCursorEnabled;
}

