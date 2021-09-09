#pragma once

#include "Bindable.h"
#include "Opengl.h"

class Rasterizer final : public Bindable {
	bool mTwoSide;
public:

	Rasterizer(bool two_side) noexcept :
		mTwoSide(two_side)
	{

	}

	void Bind() const noexcept override {
		if (mTwoSide) {
			Opengl::Disable(Opengl::State::CULL);
		}
		else {
			Opengl::Enable(Opengl::State::CULL);
		}
	}

	std::string GetUID() const noexcept override {
		return GenerateUID(mTwoSide);
	}

	static std::string GenerateUID(bool two_side) noexcept {
		using namespace std::string_literals;
		return typeid(Rasterizer).name() + "#"s + std::to_string(two_side);
	}
};