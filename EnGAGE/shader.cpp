#include "pch.h"
#include "shader.h"

#include <glad/glad.h>


std::vector<uint32_t> Shader::sPrograms;

void Shader::shutdown() noexcept
{
	for (const auto& program : sPrograms)
	{
		glDeleteProgram(program);
	}
}

Shader::Shader(const std::string& name) noexcept  :
	mName(name)
{
	mProgram = glCreateProgram();
}

void Shader::loadVertexShader(const std::string& path) noexcept
{
	mShaders.push_back(processShader(path, GL_VERTEX_SHADER));
}

void Shader::loadFragmentShader(const std::string& path) noexcept
{
	mShaders.push_back(processShader(path, GL_FRAGMENT_SHADER));
}

void Shader::compileAllShaders() const noexcept
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
}

int32_t Shader::loadUniformLocation(const std::string& name) const noexcept
{
	int32_t location = glGetUniformLocation(mProgram, name.c_str());

	if (location == -1)
	{
		Logger::warn("Uniform location at( {} ) is unused !", name);
	}
	return location;
}




uint32_t Shader::processShader(const std::string& path, uint32_t shaderType) noexcept
{
	uint32_t shader;
	std::ifstream f;
	std::string source;
	f.exceptions(std::ios::failbit | std::ios::badbit);
	try
	{
		std::stringstream ss;

		f.open(path);
		ss << f.rdbuf();

		source = ss.str();
	}
	catch (std::ios::failure& e)
	{
		Logger::error("Error reading/opening file: {}, ifstream: {}", path, e.what());
		return 0;
	}

	shader = glCreateShader(shaderType);
	const char* source_c_str = source.c_str();
	glShaderSource(shader, 1, &source_c_str, nullptr);
	glCompileShader(shader);
	//TODO: check for errors
	return shader;
}
