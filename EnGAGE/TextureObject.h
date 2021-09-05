#pragma once

#include "Bindable.h"
#include "Opengl.h"

class TextureObject final : public Bindable {
	std::string path;
	Texture tex;
	unsigned int slot;
public:
	TextureObject(const std::string& path, const unsigned int slot = 0u) noexcept :
		path(path), tex(Opengl::LoadTexture(path)), slot(slot) {}

	void Bind() const noexcept override {
		Opengl::BindTexture(tex, slot);
	}

	std::string GetUID() const noexcept override {
		return GenerateUID(path, slot);
	}

	static std::string GenerateUID(const std::string& path, const unsigned int slot) noexcept {
		using namespace std::string_literals;
		return typeid(TextureObject).name() + "#"s + path + "#"s + std::to_string(slot);
	}
};