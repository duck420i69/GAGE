#include "pch.h"
#include "opengl.h"

#include "logger.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

void Opengl::init()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwTerminate();
        assert(!"Failed to initialize OpenGL context !");
    }
#ifndef NDEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback((GLDEBUGPROC) Opengl::messageCallback, nullptr);
#endif // !NDEBUG

}

void Opengl::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}


void Opengl::messageCallback(uint32_t source, uint32_t type, uint32_t id, uint32_t severity, uint32_t length, const char* message, const void* userParam)
{
    std::string sourceName, typeName;

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:
        sourceName = "GL_DEBUG_SOURCE_API";
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        sourceName = "GL_DEBUG_SOURCE_WINDOW_SYSTEM";
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        sourceName = "GL_DEBUG_SOURCE_SHADER_COMPILER";
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        sourceName = "GL_DEBUG_SOURCE_THIRD_PARTY";
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        sourceName = "GL_DEBUG_SOURCE_APPLICATION";
        break;
    case GL_DEBUG_SOURCE_OTHER:
        sourceName = "GL_DEBUG_SOURCE_OTHER";
        break;
    }

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:
        typeName = "GL_DEBUG_TYPE_ERROR";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        typeName = "GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        typeName = "GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        typeName = "GL_DEBUG_TYPE_PORTABILITY";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        typeName = "GL_DEBUG_TYPE_PERFORMANCE";
        break;
    case GL_DEBUG_TYPE_MARKER:
        typeName = "GL_DEBUG_TYPE_MARKER";
        break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
        typeName = "GL_DEBUG_TYPE_PUSH_GROUP";
        break;
    case GL_DEBUG_TYPE_POP_GROUP:
        typeName = "GL_DEBUG_TYPE_POP_GROUP";
        break;
    case GL_DEBUG_TYPE_OTHER:
        typeName = "GL_DEBUG_TYPE_OTHER";
        break;
    }

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        Logger::error("[Opengl] From: {}\n type: {}\n infoID: {}\n message: {}", sourceName, typeName, id, message);
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        Logger::error("[Opengl] From: {}\n type: {}\n infoID: {}\n message: {}", sourceName, typeName, id, message);
        break;
    case GL_DEBUG_SEVERITY_LOW:
        Logger::warn("[Opengl] From: {}\n type: {}\n infoID: {}\n message: {}", sourceName, typeName, id, message);
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        Logger::info("[Opengl] From: {}\n type: {}\n infoID: {}\n message: {}", sourceName, typeName, id, message);
        break;
    }
}
