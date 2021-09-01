#pragma once

#include "Bindable.h"
#include "Opengl.h"

class TextureObject final : public Bindable {
	Texture tex;
public:
	TextureObject(const std::string& path) noexcept :
		tex(Opengl::LoadTexture(path)) {}

	void Bind() const noexcept override {
		Opengl::BindTexture(tex);
	}
};