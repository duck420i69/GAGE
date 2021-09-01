#pragma once

#include "Bindable.h"
#include "Opengl.h"

#include <cstdint>
#include <vector>
#include <string>

class ShaderObject final : public Bindable
{
	ShaderProgram program;
public:
	ShaderObject(const std::string& path) noexcept :
		program(Opengl::LoadShader(path)) {}

	void Bind() const noexcept override {
		Opengl::BindProgram(program);
	}
};