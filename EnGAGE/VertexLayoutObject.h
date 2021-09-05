#pragma once

#include "Bindable.h"
#include "Opengl.h"
#include "VertexBufferObject.h"
#include "DynamicVertex.h"

#include <vector>


class VertexLayoutObject final : public Bindable {
public:
	struct Layout {
		unsigned int slot;
		unsigned int size;
		unsigned int stride;
		unsigned int offset;
	};

private:
	std::vector<Layout> layouts;
	const DynamicVertex::VertexLayout* dyn_layout;
public:
	VertexLayoutObject(const DynamicVertex::VertexLayout& dyn_layout) noexcept :
		dyn_layout(&dyn_layout)
	{
		const auto& elements = dyn_layout.GetElements();

		for (size_t i = 0; i < elements.size(); i++) {
			Layout layout;
			layout.slot = i;
			layout.size = elements[i].Size() / sizeof(float);
			layout.stride = dyn_layout.Size();
			layout.offset = elements[i].GetOffset();
			layouts.push_back(layout);
		}
	}

	void Bind() const noexcept override {
		for (const auto& layout : layouts) {
			Opengl::Layout(layout.slot, layout.size, layout.stride, layout.offset);
		}
	}

	static std::string GenerateUID(const DynamicVertex::VertexLayout& dyn_layout) noexcept {
		using namespace std::string_literals;
		return typeid(VertexLayoutObject).name() + "#"s + dyn_layout.GetCode();
	}


	std::string GetUID() const noexcept override {
		return GenerateUID(*dyn_layout);
	}


};