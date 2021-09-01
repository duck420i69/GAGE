#pragma once

#include "Bindable.h"
#include "Opengl.h"

class Sampler final : public Bindable {
	Opengl::TextureFilter min_filter, mag_filter;
	Opengl::TextureWrap wrap;
public:
	Sampler(const Opengl::TextureFilter min_filter, const Opengl::TextureFilter mag_filter, const Opengl::TextureWrap wrap) noexcept :
		min_filter(min_filter), mag_filter(mag_filter),
		wrap(wrap)
	{};

	void Bind() const noexcept {
		Opengl::TextureParameters(min_filter, mag_filter, wrap);
	}
};