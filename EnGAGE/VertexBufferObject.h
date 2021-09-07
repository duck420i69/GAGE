#pragma once

#include "pch.h"
#include "Bindable.h"
#include "Opengl.h"

#include "DynamicVertex.h"

class VertexBufferObject final : public Bindable {
	friend class VertexLayoutObject;

	VertexArray va;
	VertexBuffer vb;
	VertexBuffer ib;
	std::string tag;
	const DynamicVertex::VertexLayout& dyn_layout;
public:

	VertexBufferObject(const std::string& tag, const DynamicVertex::VertexLayout& dyn_layout, const VertexBuffer ib, const DynamicVertex::VertexBuffer& vbuf) noexcept :
		tag(tag),
		va(Opengl::CreateVertexArray()),
		vb(Opengl::CreateVertexBuffer(vbuf.SizeBytes(), vbuf.GetData())),
		dyn_layout(dyn_layout),
		ib(ib)
	{
		Opengl::BindVertexArray(va);
		Opengl::BindVertexBuffer(vb);
		Opengl::BindIndexBuffer(ib);
		const auto& elements = dyn_layout.GetElements();

		for (size_t i = 0; i < elements.size(); i++) {
			size_t slot = i;
			size_t size = (elements[i].Size() / sizeof(float));
			size_t stride = (unsigned int)dyn_layout.Size();
			size_t offset = (unsigned int)elements[i].GetOffset();
			Opengl::Layout(slot, size, stride, offset);
		}

		Opengl::BindVertexBuffer(0);
		Opengl::BindVertexArray(0);
	}

	void Bind() const noexcept override {
		Opengl::BindVertexArray(va);
	}

	template<typename... Ignore>
	static std::string GenerateUID(const std::string& tag, const DynamicVertex::VertexLayout& dyn_layout, const VertexBuffer ib, Ignore&&... ignore) noexcept {
		return GenerateUIDInternal(tag, dyn_layout, ib);
	}
	std::string GetUID() const noexcept override {
		return GenerateUID(tag, dyn_layout, ib);
	}

private:
	static std::string GenerateUIDInternal(const std::string& tag, const DynamicVertex::VertexLayout& dyn_layout, const VertexBuffer ib) {
		using namespace std::string_literals;
		return typeid(VertexBufferObject).name() + "#"s + tag + "#"s + dyn_layout.GetCode() + "#"s + std::to_string(ib);
	}

};
