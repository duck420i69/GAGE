#include "pch.h"
#include "Opengl.h"

#include <Windows.h>
#include <glad/glad.h>

void messageCallbackFn(
	GLenum source,
	GLenum type, 
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	std::stringstream ss;
	ss << std::string(message) << "\n";
	std::string result = ss.str();
	OutputDebugStringA(result.c_str());
}

void Opengl::Init() noexcept
{
#	ifndef NDEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(messageCallbackFn, nullptr);
#	endif // !NDEBUG
}

void Opengl::Clear() noexcept
{
	glClear(GL_COLOR_BUFFER_BIT);
}
