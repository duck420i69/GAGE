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
	if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) return;
	Logger::info("{}", message);
}

std::vector<VertexArray> Opengl::s_vaos;
std::vector<VertexBuffer> Opengl::s_vbos;
std::vector<Texture> Opengl::s_textures;
std::vector<ShaderProgram> Opengl::s_shaders;
glm::mat4x4 Opengl::s_projection = glm::mat4x4(1.0f), Opengl::s_camera = glm::mat4x4(1.0f);

void Opengl::Init() noexcept
{
#	ifndef NDEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(MessageCallbackFn, nullptr);
#	endif // !NDEBUG

	int texture_units, uniform_binding, uniform_block_size;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &texture_units);
	glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &uniform_binding);
	glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &uniform_block_size);

	Logger::info("Max texture image units: {}", texture_units);
	Logger::info("Max uniform buffer bindings: {}", uniform_binding);
	Logger::info("Max uniform buffer block size: {}", uniform_block_size);

	//Enable n stuffs
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	
}

void Opengl::Clear() noexcept
{
	glViewport(0, 0, Window::GetWidth(), Window::GetHeight());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

void Opengl::Destroy() noexcept
{
	glDeleteVertexArrays(s_vaos.size(), s_vaos.data());
	glDeleteBuffers(s_vbos.size(), s_vbos.data());
	glDeleteTextures(s_textures.size(), s_textures.data());
	for(const auto& program : s_shaders)
		glDeleteProgram(program);
	Logger::info("Clearing gpu memory !");
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
	glBindBufferBase(GL_UNIFORM_BUFFER, slot, vbo);
	s_vbos.push_back(vbo);
	return vbo;
}

Texture Opengl::LoadTexture(const std::string& path, int* out_width, int* out_height) noexcept
{
	Logger::info("Loading texture: {}", path);
	return LoadTextureInternal(path, out_width, out_height);
}

ShaderProgram Opengl::LoadShader(const std::string& path) noexcept
{
	Logger::info("Loading shader: {}", path);
	return LoadShaderProgramInternal(path + "_VS.glsl", path + "_FS.glsl");
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

void Opengl::BindUniformBuffer(const UniformBuffer ub, const uint32_t slot) noexcept
{
	//glBindBuffer(GL_UNIFORM_BUFFER, ub);
	glBindBufferBase(GL_UNIFORM_BUFFER, slot, ub);
}

void Opengl::BindIndexBuffer(const VertexBuffer ib) noexcept
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
}

void Opengl::BindProgram(const ShaderProgram program) noexcept
{
	glUseProgram(program);
}

void Opengl::BindTexture(const Texture tex, const unsigned int slot) noexcept
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, tex);
}

void Opengl::TextureParameters(const TextureFilter min_filter, const TextureFilter mag_filter, const TextureWrap wrap)
{
	auto gl_min_filter = GetTextureFilter(min_filter);
	auto gl_mag_filter = GetTextureFilter(mag_filter);
	auto gl_wrap = GetTextureWrap(wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gl_min_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gl_min_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, gl_wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, gl_wrap);
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

void Opengl::SetProjection(const glm::mat4x4& proj) noexcept
{
	s_projection = proj;
}

const glm::mat4x4& Opengl::GetProjection() noexcept
{
	return s_projection;
}

void Opengl::SetCamera(const glm::mat4x4& cam) noexcept
{
	s_camera = cam;
}

const glm::mat4x4& Opengl::GetCamera() noexcept
{
	return s_camera;
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

unsigned int Opengl::GetTextureFilter(const TextureFilter filter) noexcept
{
	switch (filter)
	{
	case TextureFilter::LINEAR:
		return GL_LINEAR;
	case TextureFilter::NEAREST:
		return GL_NEAREST;
	case TextureFilter::NEAREST_MIPMAP_NEAREST:
		return GL_NEAREST_MIPMAP_NEAREST;
	case TextureFilter::LINEAR_MIPMAP_NEAREST:
		return GL_LINEAR_MIPMAP_NEAREST;
	case TextureFilter::NEAREST_MIPMAP_LINEAR:
		return GL_NEAREST_MIPMAP_LINEAR;
	case TextureFilter::LINEAR_MIPMAP_LINEAR:
		return GL_LINEAR_MIPMAP_LINEAR;
	}

	return GL_NEAREST;
}

unsigned int Opengl::GetTextureWrap(const TextureWrap filter) noexcept
{
	switch (filter)
	{
	case TextureWrap::REPEAT:
		return GL_REPEAT;
	case TextureWrap::MIRRORED_REPEAT:
		return GL_MIRRORED_REPEAT;
	case TextureWrap::CLAMP_TO_EDGE:
		return GL_CLAMP_TO_EDGE;
	case TextureWrap::CLAMP_TO_BORDER:
		return GL_CLAMP_TO_BORDER;
	}
	return GL_REPEAT;;
}

Texture Opengl::LoadTextureInternal(const std::string& path, int* out_width, int* out_height) noexcept
{
	Texture texture = 0;
	int width = 0, height = 0;
	try
	{
		stbi_uc* image_data = nullptr;
		int bpp = 0;
		image_data = stbi_load(path.c_str(), &width, &height, &bpp, STBI_rgb_alpha);
		if (!image_data)
		{
			std::stringstream ss;
			ss << "Failed to load image : " << path << ", stb_image's report: " <<
				std::string(stbi_failure_reason());

			throw std::runtime_error(ss.str());
		}
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glHint(GL_TEXTURE_COMPRESSION_HINT, GL_NICEST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);

		glBindTexture(GL_TEXTURE_2D, 0);

		stbi_image_free(image_data);
	}
	catch (std::exception& e)
	{
		Logger::error("Texture exception thrown: {}", e.what());
	}
	if (out_width)
		*out_width = width;
	if (out_height)
		*out_height = height;
	return texture;
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

