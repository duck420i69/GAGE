#pragma once

#include "pch.h"
#include "Bindable.h"
#include "Opengl.h"

#include "DynamicVertex.h"

class VertexBufferObject final : public Bindable {
	friend class VertexLayoutObject;

	VertexArray va;
	VertexBuffer vb;
public:

	template<typename T>
	VertexBufferObject(const std::vector<T>& vertices) noexcept :
		va(Opengl::CreateVertexArray()),
		vb(Opengl::CreateVertexBuffer(sizeof(T) * vertices.size(), vertices.data()))
	{}

	VertexBufferObject(const DynamicVertex::VertexBuffer& vbuf) noexcept :
		va(Opengl::CreateVertexArray()),
		vb(Opengl::CreateVertexBuffer(vbuf.SizeBytes(), vbuf.GetData()))
	{}

	void Bind() const noexcept override {
		Opengl::BindVertexArray(va);
		Opengl::BindVertexBuffer(vb);
	}

};
