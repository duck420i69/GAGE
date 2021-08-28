#pragma once

#include "Bindable.h"
#include "Opengl.h"

template<typename C>
class UniformBufferObject final : public Bindable {
	UniformBuffer ub;
public:

	UniformBufferObject(const unsigned int slot, const C& consts) noexcept : 
		ub(Opengl::CreateUniformBuffer(slot, sizeof(C), &consts))
	{}

	UniformBufferObject(const unsigned int slot) noexcept : 
		ub(Opengl::CreateUniformBuffer(slot, sizeof(C), nullptr))
	{

	}


	void Bind(const C& consts) const noexcept override {
		Opengl::UpdateUniformBuffer(ub, sizeof(C), &C);
	}
};