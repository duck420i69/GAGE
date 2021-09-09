#pragma once

#include "Bindable.h"
#include "Opengl.h"

class TextureObject final : public Bindable {
	std::string path;
	Texture tex;
	bool has_alpha;
	unsigned int slot;
	Opengl::TextureFilter min_filter, mag_filter;
	Opengl::TextureWrap wrap;
public:
	TextureObject(const std::string& path,
		const Opengl::TextureFilter min_filter = Opengl::TextureFilter::NEAREST,
		const Opengl::TextureFilter mag_filter = Opengl::TextureFilter::NEAREST,
		const Opengl::TextureWrap wrap = Opengl::TextureWrap::CLAMP_TO_EDGE, const unsigned int slot = 0u) noexcept :
		path(path), tex(Opengl::LoadTexture(path, min_filter, mag_filter, wrap, nullptr, nullptr, &has_alpha)), slot(slot) {}

	void Bind() const noexcept override {
		Opengl::BindTexture(tex, slot);
	}

	inline bool HasAlpha() const noexcept {	return has_alpha;}

	std::string GetUID() const noexcept override {
		return GenerateUID(path, min_filter, mag_filter, wrap, slot);
	}

	static std::string GenerateUID(const std::string& path, const Opengl::TextureFilter min_filter, const Opengl::TextureFilter mag_filter, const Opengl::TextureWrap wrap, const unsigned int slot) noexcept {
		using namespace std::string_literals;
		return typeid(TextureObject).name() + "#"s +
			path + "#"s +
			std::to_string((int)min_filter) + "#"s +
			std::to_string((int)mag_filter) + "#"s +
			std::to_string((int)wrap) + "#"s +
			std::to_string(slot);
	}
};