#include "pch.h"
#include "Opengl.h"

#include "Window.h"
#include "Logger.h"

#include <glad/glad.h>

void MessageCallbackFn(
	GLenum source,
	GLenum type, 
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* user_param)
{
	Logger::info("{}", message);
}

void Opengl::Init() noexcept
{
#	ifndef NDEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(MessageCallbackFn, nullptr);
#	endif // !NDEBUG

	int texture_units;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &texture_units);

	Logger::info("Max texture image units: {}", texture_units);

	//Enable n stuffs
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Opengl::Clear() noexcept
{
	glViewport(0, 0, Window::GetWidth(), Window::GetHeight());
	glClear(GL_COLOR_BUFFER_BIT);
}
