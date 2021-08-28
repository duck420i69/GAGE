#include "pch.h"
#include "Opengl.h"

#include "Window.h"
#include "Logger.h"
#include "Utils.h"

#include <glad/glad.h>
#include <stb_image.h>

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

std::vector<VertexArray> Opengl::s_vaos;
std::vector<VertexBuffer> Opengl::s_vbos;
std::unordered_map<std::string, Texture> Opengl::s_texture_map;
std::unordered_map<std::string, ShaderProgram> Opengl::s_shader_map;

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
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

void Opengl::Destroy() noexcept
{
	glDeleteVertexArrays(s_vaos.size(), s_vaos.data());
	glDeleteBuffers(s_vbos.size(), s_vbos.data());
}

VertexArray Opengl::CreateVertexArray() noexcept
{
	VertexArray vao;
	glGenVertexArrays(1, &vao);
	s_vaos.push_back(vao);
	return vao;
}

VertexBuffer Opengl::CreateVertexBuffer(const uint64_t size, const void* data, const BufferUsage usage) noexcept
{
	VertexBuffer vbo;

	GLenum opengl_usage = GetBufferUsage(usage);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size, data, opengl_usage);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	s_vbos.push_back(vbo);
	return vbo;
}

VertexBuffer Opengl::CreateIndexBuffer(const uint64_t size, const unsigned int* data, const BufferUsage usage)
{
	VertexBuffer vbo;
	GLenum opengl_usage = GetBufferUsage(usage);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, opengl_usage);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	s_vbos.push_back(vbo);
	return vbo;
}

UniformBuffer Opengl::CreateUniformBuffer(const unsigned int slot, const uint64_t size, const void* data, const BufferUsage usage) noexcept
{
	UniformBuffer vbo;

	GLenum opengl_usage = GetBufferUsage(usage);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_UNIFORM_BUFFER, vbo);
	glBufferData(GL_UNIFORM_BUFFER, size, data, opengl_usage);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, slot, vbo, 0, size);
	s_vbos.push_back(vbo);
	return vbo;
}

Texture Opengl::LoadTexture(const std::string& path) noexcept
{
	if (s_texture_map.count(path) == 0) {
		//Load new texture
		Texture texture;
		LoadTextureInternal(path, &texture);
		s_texture_map[path] = texture;
		return texture;
	}
	else {
		//Get already loaded texture
		return s_texture_map[path];
	}
}

ShaderProgram Opengl::LoadShader(const std::string& path) noexcept
{
	if (s_shader_map.count(path) == 0) {
		//Load new shader
		ShaderProgram shader = LoadShaderProgramInternal(path + "_VS.glsl", path + "_FS.glsl");
		s_shader_map[path] = shader;
		return shader;
	}
	else {
		//Get already loaded shader
		return s_shader_map[path];
	}

}

void Opengl::Layout(const unsigned int slot, const unsigned int size, const unsigned int stride, const unsigned int offset) noexcept
{
	glEnableVertexAttribArray(slot);
	glVertexAttribPointer(slot, size, GL_FLOAT, GL_FALSE, stride, (const void*)(offset));
}


void Opengl::BindVertexArray(const VertexArray va) noexcept
{
	glBindVertexArray(va);
}

void Opengl::BindVertexBuffer(const VertexBuffer vb) noexcept
{
	glBindBuffer(GL_ARRAY_BUFFER, vb);
}

void Opengl::BindUniformBuffer(const UniformBuffer ub) noexcept
{
	glBindBuffer(GL_UNIFORM_BUFFER, ub);
}

void Opengl::BindIndexBuffer(const VertexBuffer ib) noexcept
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
}

void Opengl::BindProgram(const ShaderProgram program) noexcept
{
	glUseProgram(program);
}

void Opengl::UpdateUniformBuffer(const UniformBuffer ub, const uint64_t size, const void* data) noexcept
{
	glBindBuffer(GL_UNIFORM_BUFFER, ub);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Opengl::DrawIndexed(const uint64_t count) noexcept
{
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}

unsigned int Opengl::GetBufferUsage(const BufferUsage usage) noexcept
{
	switch (usage) {
	case BufferUsage::DYNAMIC:
		return GL_DYNAMIC_DRAW;
	case BufferUsage::STREAM:
		return GL_STREAM_DRAW;
	case BufferUsage::STATIC:
		return GL_STATIC_DRAW;
	}
	return GL_STATIC_DRAW;
}

void Opengl::LoadTextureInternal(const std::string& path, Texture* texture) noexcept
{
	try
	{
		stbi_uc* image_data = nullptr;
		int bpp = 0;
		image_data = stbi_load(path.c_str(), &texture->width, &texture->height, &bpp, STBI_rgb_alpha);
		if (!image_data)
		{
			std::stringstream ss;
			ss << "Failed to load image : " << path << ", stb_image's report: " <<
				std::string(stbi_failure_reason());

			throw std::runtime_error(ss.str());
		}
		glGenTextures(1, &texture->id);
		glBindTexture(GL_TEXTURE_2D, texture->id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glHint(GL_TEXTURE_COMPRESSION_HINT, GL_NICEST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);

		glBindTexture(GL_TEXTURE_2D, 0);

		stbi_image_free(image_data);

		Logger::info("Creating texture: {}", path);
	}
	catch (std::exception& e)
	{
		Logger::error("Texture exception thrown: {}", e.what());
	}
}

uint32_t Opengl::LoadShaderInternal(const std::string& path, uint32_t type) noexcept
{
	try
	{
		std::string source = Utils::LoadFile(path); //throws IO exceptions

		uint32_t shader = glCreateShader(type);
		const char* source_c_str = source.c_str();
		glShaderSource(shader, 1, &source_c_str, nullptr);
		glCompileShader(shader);

		int compile_status;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
		if (compile_status == GL_FALSE)
		{
			//Throw glsl error
			int log_length;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);

			std::vector<char> message(log_length);
			glGetShaderInfoLog(shader, log_length, &log_length, message.data());
			throw std::string(message.data());
		}
		return shader;
	}
	catch (std::ios::failure& e)
	{
		Logger::error("Error loading shader file: {}, io: {}", path, e.what());
	}
	catch (std::string& glsl_error)
	{
		Logger::error("Shader compiling shader: {}, error: {}", path, glsl_error);
	}
	return 0;
}

ShaderProgram Opengl::LoadShaderProgramInternal(const std::string& vertex_path, const std::string& fragment_path) noexcept
{
	ShaderProgram program = glCreateProgram();
	std::vector<uint32_t> shaders;
	shaders.push_back(LoadShaderInternal(vertex_path, GL_VERTEX_SHADER));
	shaders.push_back(LoadShaderInternal(fragment_path, GL_FRAGMENT_SHADER));


	for (const auto& shader : shaders) {
		glAttachShader(program, shader);
	}
	glLinkProgram(program);
	glValidateProgram(program);

	for (const auto& shader : shaders) {
		glDeleteShader(shader);
	}


	return program;
}

