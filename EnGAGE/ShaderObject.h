#pragma once

#include "Bindable.h"
#include "Opengl.h"
#include "BindableCodex.h"

#include <cstdint>
#include <vector>
#include <string>

class ShaderObject final : public Bindable
{
	ShaderProgram program;
	struct {
		std::string vertex_path;
		std::string fragment_path;
	} path;
public:
	ShaderObject(const std::string& vertex_path, const std::string& fragment_path) noexcept :
		path{ vertex_path, fragment_path }
	{
		program = Opengl::LoadShader(path.vertex_path, path.fragment_path);
	}

	void Bind() const noexcept override {
		Opengl::BindProgram(program);
	}

	void LoadTextureSlot(const std::string& name, const unsigned int slot) const noexcept {
		Opengl::BindProgram(program);
		Opengl::LoadUniformInteger(program, name, slot);
		Opengl::BindProgram(0);
	}

	std::string GetUID() const noexcept override {
		return GenerateUID(path.vertex_path, path.fragment_path);
	}


	static std::string GenerateUID(const std::string& vertex_path, const std::string& fragment_path) noexcept {
		using namespace std::string_literals;
		return typeid(ShaderObject).name() + "#"s + vertex_path + "#"s + fragment_path;
	}
};