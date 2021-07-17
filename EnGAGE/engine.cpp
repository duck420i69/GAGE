#include "pch.h"
#include "engine.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

TEngine::TEngine() noexcept :
	mRun(true),
	mWidth(1280), mHeight(720),
	mTitle("EnGAGE"),
	mWindow(nullptr)
{

    assert(glfwInit() && "Failed to init glfw !");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#   ifndef NDEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#   endif // !NDEBUG

    mWindow = glfwCreateWindow(mWidth, mHeight, mTitle.c_str(), nullptr, nullptr);
    assert(mWindow && "Window creation failed !");

    glfwMakeContextCurrent(mWindow);
    glfwSwapInterval(1);

}

TEngine::~TEngine() noexcept
{
    glfwDestroyWindow(mWindow);
    glfwTerminate();
}

void TEngine::init() noexcept
{
}

void TEngine::update() noexcept
{
    
}

void TEngine::quit() noexcept
{
    mRun = false;
    glfwSetWindowShouldClose(mWindow, true);
}

TEngine& TEngine::get() noexcept
{
    static TEngine instance;
    return instance;
}
