#pragma once

#include "Bindable.h"
#include "Opengl.h"

template<typename C>
class UniformBufferObject final : public Bindable {
	UniformBuffer ub;
	unsigned int slot;
public:

	UniformBufferObject(const unsigned int slot, const C& consts) noexcept : 
		ub(Opengl::CreateUniformBuffer(slot, sizeof(C), &consts)),
		slot(slot)
	{}

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
};