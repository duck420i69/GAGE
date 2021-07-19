#include "pch.h"
#include "Opengl.h"

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
	std::stringstream ss;
	ss << std::string(message) << "\n";
	std::string result = ss.str();
	//TODO: Logger here
}

void Opengl::Init() noexcept
{
#	ifndef NDEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(MessageCallbackFn, nullptr);
#	endif // !NDEBUG
}

void Opengl::Clear() noexcept
{
	glClear(GL_COLOR_BUFFER_BIT);
}
