#include "pch.h"
#include "window.h"


GLFWwindow* Window::sWindow = nullptr;
uint16_t	Window::sWidth = 0, Window::sHeight = 0;
std::string	Window::sTitle = "";


void Window::createWindow(uint16_t width, uint16_t height, const std::string& title) noexcept
{
    Logger::info("Creating window(width: {}, height: {}, title: {})", width, height, title);
    sWidth = width;
    sHeight = height;

    if (!glfwInit())
    {
        assert(!"Failed to init glfw !");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#   ifndef NDEBUG
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#   endif // !NDEBUG
    
    sWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!sWindow)
    {
        glfwTerminate();
        assert(!"Window creation failed !");
    }

    glfwMakeContextCurrent(sWindow);
    glfwSwapInterval(1);

    //Callbacks
    glfwSetFramebufferSizeCallback(sWindow, [](auto* window, auto width, auto height)
    {
        sWidth = width;
        sHeight = height;
    });
    
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

GLFWwindow* Window::getWindow() noexcept
{
    return sWindow;
}

uint16_t Window::getWidth() noexcept
{
    return sWidth;
}

uint16_t Window::getHeight() noexcept
{
    return sHeight;
}

void Window::shutdown() noexcept
{
    glfwDestroyWindow(sWindow);
    glfwTerminate();
    Logger::info("Window is shutting down !");
}
