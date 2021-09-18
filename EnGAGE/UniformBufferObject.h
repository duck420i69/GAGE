#pragma once

#include "Bindable.h"
#include "Opengl.h"
#include "DynamicUniform.h"
#include "TechniqueProbe.h"

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

class UniformBufferObjectDynamic final : public Bindable {
	UniformBuffer ub;
	unsigned int slot;
	DynamicUniform::Buffer buffer;
	mutable bool dirty = false;
public:

	UniformBufferObjectDynamic(const unsigned int slot, const DynamicUniform::Buffer& buffer) noexcept :
		ub(Opengl::CreateUniformBuffer(slot, buffer.GetRawBuffer().size(), buffer.GetRawBuffer().data())),
		slot(slot),
		buffer(buffer)
	{

	}

	void Accept(class TechniqueProbe& probe) noexcept override {
		if (probe.VisitBuffer(buffer)) {
			dirty = true;
		}
	};
	void Bind() const noexcept override {
		if (dirty) {
			Update(buffer);
			dirty = false;
		}
		Opengl::BindUniformBuffer(ub, slot);
	}

	void Update(const DynamicUniform::Buffer& buffer) const noexcept  {
		Opengl::UpdateUniformBuffer(ub, buffer.GetRawBuffer().size(), buffer.GetRawBuffer().data());
	}

	static std::string GenerateUID(const unsigned int slot, const DynamicUniform::Buffer& buffer) noexcept {
		using namespace std::string_literals;

		return typeid(UniformBufferObjectDynamic).name() + "#"s + buffer.GetLayout().GenerateUID();
	}

	std::string GetUID() const noexcept override {
		return GenerateUID(slot, buffer);
	}

};