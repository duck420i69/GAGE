#pragma once

#include "Bindable.h"

class StencilObject final : public Bindable {
public:
	enum Mode{
		Off,
		Write,
		Mask
	};
private:
	Mode mode;
public:
	StencilObject(Mode mode) noexcept;
	void Bind() const noexcept override;


	std::string GetUID() const noexcept override {
		return GenerateUID(mode);
	}


	static std::string GenerateUID(Mode mode) noexcept {
		using namespace std::string_literals;
		return typeid(StencilObject).name() + "#"s + std::to_string(int(mode));
	}
};