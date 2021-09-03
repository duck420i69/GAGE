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
public:

	VertexLayoutObject(const std::vector<Layout>& layouts) noexcept :
		layouts(layouts){}

	VertexLayoutObject(const DynamicVertex::VertexLayout& dyn_layout) noexcept
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

	virtual void Bind() const noexcept override {
		for (const auto& layout : layouts) {
			Opengl::Layout(layout.slot, layout.size, layout.stride, layout.offset);
		}
	}
};