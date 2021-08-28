#pragma once

#include "Bindable.h"
#include "Opengl.h"
#include "VertexBufferObject.h"

#include <vector>


class VertexLayout final : public Bindable {
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

	VertexLayout(const std::vector<Layout>& layouts) noexcept :
		layouts(layouts){}

	virtual void Bind() const noexcept override {
		for (const auto& layout : layouts) {
			Opengl::Layout(layout.slot, layout.size, layout.stride, layout.offset);
		}
	}
};