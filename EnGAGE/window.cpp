#include "pch.h"
#include "window.h"

GLFWwindow* Window::sWindow = nullptr;
U16			Window::sWidth = 0, Window::sHeight = 0;
String		Window::sTitle = "";


void Window::createWindow(U16 width, U16 height, StringRef title) noexcept
{
    Logger::info("Creating window(width: {}, height: {}, title: {})", width, height, title);
    if (!glfwInit())
    {
        assert(!"Failed to init glfw !");
    }

    
    sWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!sWindow)
    {
        glfwTerminate();
        assert(!"Window creation failed !");
    }

    glfwMakeContextCurrent(sWindow);
}

bool Window::shouldClose() noexcept
{
    return glfwWindowShouldClose(sWindow);
}

void Window::swapBuffers() noexcept
{
    glfwSwapBuffers(sWindow);
}

void Window::pollEvents() noexcept
{
    glfwPollEvents();
}

void Window::shutdown() noexcept
{
    glfwDestroyWindow(sWindow);
    glfwTerminate();
    Logger::info("Window is shutting down !");
}
