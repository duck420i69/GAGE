#include "pch.h"
#include "Shader.h"

#include "Logger.h"
#include "Utils.h"

#include <glad/glad.h>

Shader::Shader() noexcept
{
	mProgram = glCreateProgram();
}

Shader::~Shader() noexcept
{
	glDeleteProgram(mProgram);
	Logger::info("Destroying shader: {}", mProgram);
}

void Shader::Bind() const
{
	glUseProgram(mProgram);
}

void Shader::LoadVertex(const std::string& file_path) noexcept
{
	Load(file_path, GL_VERTEX_SHADER);
	Logger::info("Loading vertex shader: {}", file_path);
}

void Shader::LoadFragment(const std::string& file_path) noexcept
{
	Load(file_path, GL_FRAGMENT_SHADER);
	Logger::info("Loading fragment shader: {}", file_path);
}

void Shader::Create() const noexcept
{
	for (const auto& shader : mShaders)
	{
		glAttachShader(mProgram, shader);
	}

	glLinkProgram(mProgram);
	glValidateProgram(mProgram);

	for (const auto& shader : mShaders)
	{
		glDeleteShader(shader);
	}
	Logger::info("Creating program: {}", mProgram);
}

void Shader::UploadMat4x4(const std::string& name, const float* ptr) const
{
	int location = GetUniformLocation(name);
	glUniformMatrix4fv(location, 1, GL_FALSE, ptr);
}

void Shader::UploadFloat(const std::string& name, const float value) const
{
	int location = GetUniformLocation(name);
	glUniform1f(location, value);
}

void Shader::UploadTexture(const std::string& name, const int& slot) const
{
	int location = GetUniformLocation(name);
	glUniform1i(location, slot);
}

void Shader::UploadIntArr(const std::string& name, const int* arr, const uint32_t size) const
{
	int location = GetUniformLocation(name);
	glUniform1iv(location, size, arr);
}

void Shader::Load(const std::string& file_path, uint32_t type) noexcept
{
	try
	{
		std::string source = Utils::LoadFile(file_path); //throws IO exceptions

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
		mShaders.push_back(shader);
	}
	catch (std::ios::failure& e)
	{
		Logger::error("Error loading shader file: {}, io: {}", file_path, e.what());
	}
	catch (std::string& glsl_error)
	{
		Logger::error("Shader compiling shader: {}, error: {}", file_path, glsl_error);
	}
}

int Shader::GetUniformLocation(const std::string& name) const
{
	int location = glGetUniformLocation(mProgram, name.c_str());
#	ifndef NDEBUG
		if (location == -1)
		{
			Logger::warn("Uniform location ( {} ) is not used !", name);
		}
#	endif // !NDEBUG


	return location;
}
