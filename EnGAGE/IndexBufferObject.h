#pragma once

#include "pch.h"
#include "Bindable.h"
#include "Opengl.h"

class IndexBufferObject final : public Bindable {
	VertexBuffer ib;
	uint64_t count;
public:
	IndexBufferObject(const std::vector<unsigned int>& indices) noexcept :
		ib(Opengl::CreateIndexBuffer(indices.size() * sizeof(unsigned int), indices.data())),
		count(indices.size())
	{

	}

	void Bind() const noexcept override {
		Opengl::BindIndexBuffer(ib);
	}


	inline const uint64_t& GetCount() const noexcept { return count; }
};
