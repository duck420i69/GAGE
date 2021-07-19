#include "pch.h"
#include "Window.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

uint32_t Window::sWidth = 0, Window::sHeight = 0;
GLFWwindow* Window::sWindow = nullptr;
std::string Window::sTitle = "";

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
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
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

