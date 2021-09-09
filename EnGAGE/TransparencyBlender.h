#pragma once

#include "Bindable.h"
#include "Opengl.h"

class TransparencyBlender : public Bindable{
	bool blending;
public:
	TransparencyBlender(bool blending) noexcept :
		blending(blending) 
	{}
	void Bind() const noexcept override {
		if (blending) {
			Opengl::Enable(Opengl::State::BLEND);
			Opengl::BlendFunc(Opengl::Blend::SRC_ALPHA, Opengl::Blend::ONE_MINUS_SRC_ALPHA);
			Opengl::BlendEquation(Opengl::BlendOp::FUNC_ADD);
		}
		else {
			Opengl::Disable(Opengl::State::BLEND);
		}
	}; 

	std::string GetUID() const noexcept override {
		
	}
	static std::string GenerateUID(bool blending) noexcept {
		using namespace std::string_literals;
		return typeid(TransparencyBlender).name() + "#"s + std::to_string(blending);
	}
};