#pragma once

#include "Bindable.h"
#include "Opengl.h"

template<typename C>
class UniformBufferObject final : public Bindable {
	UniformBuffer ub;
	unsigned int slot;
public:

	UniformBufferObject(const unsigned int slot) noexcept : 
		ub(Opengl::CreateUniformBuffer(slot, sizeof(C), nullptr)),
		slot(slot)
	{

	}

	void Update(const C& consts) const noexcept {
		Opengl::UpdateUniformBuffer(ub, sizeof(C), &consts);
	}

	void Bind() const noexcept override {
		Opengl::BindUniformBuffer(ub, slot);
	}

	static std::string GenerateUID(const unsigned int slot) noexcept {
		using namespace std::string_literals;

		return typeid(UniformBufferObject).name();
	}

	std::string GetUID() const noexcept override {
		return GenerateUID(slot);
	}

};