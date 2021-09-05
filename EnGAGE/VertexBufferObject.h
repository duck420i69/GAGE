#pragma once

#include "pch.h"
#include "Bindable.h"
#include "Opengl.h"

#include "DynamicVertex.h"

class VertexBufferObject final : public Bindable {
	friend class VertexLayoutObject;

	VertexArray va;
	VertexBuffer vb;
	std::string tag;
public:

	VertexBufferObject(const std::string& tag, const DynamicVertex::VertexBuffer& vbuf) noexcept :
		tag(tag),
		va(Opengl::CreateVertexArray()),
		vb(Opengl::CreateVertexBuffer(vbuf.SizeBytes(), vbuf.GetData()))
	{}

	void Bind() const noexcept override {
		Opengl::BindVertexArray(va);
		Opengl::BindVertexBuffer(vb);
	}

	template<typename... Ignore>
	static std::string GenerateUID(const std::string& tag, Ignore&&... ignore) noexcept {
		return GenerateUIDInternal(tag);
	}
	std::string GetUID() const noexcept override {
		return GenerateUID(tag);
	}

private:
	static std::string GenerateUIDInternal(const std::string& tag) {
		using namespace std::string_literals;
		return typeid(VertexBufferObject).name() + "#"s + tag;
	}

};
