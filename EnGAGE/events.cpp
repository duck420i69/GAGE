#include "pch.h"
#include "events.h"
#include "engine.h"
#include <GLFW/glfw3.h>

void TEvents::init() noexcept
{
    GLFWwindow& window = Engine.getWindow();
    glfwSetWindowCloseCallback(&window, [](auto window)
    {
            Engine.quit();
    });
}

void TEvents::pollEvents() noexcept
{
    glfwPollEvents();
}

TEvents& TEvents::get() noexcept
{
    static TEvents instance;
    return instance;
}
