#pragma once

#include "pch.h"
#include "Bindable.h"
#include "Opengl.h"

class IndexBufferObject final : public Bindable {
	std::string tag;
	VertexBuffer ib;
	uint64_t count;
public:
	IndexBufferObject(const std::string& tag, const std::vector<unsigned int>& indices) noexcept :
		tag(tag),
		ib(Opengl::CreateIndexBuffer(indices.size() * sizeof(unsigned int), indices.data())),
		count(indices.size())
	{

	}

	void Bind() const noexcept override {
		Opengl::BindIndexBuffer(ib);
	}

	template<typename... Ignore>
	static std::string GenerateUID(const std::string& tag, Ignore&&... ignore) noexcept {
		return GenerateUIDInternal(tag);
	}
	std::string GetUID() const noexcept override {
		return GenerateUID(tag);
	}

	inline const uint64_t& GetCount() const noexcept { return count; }
private:
	static std::string GenerateUIDInternal(const std::string& tag) {
		using namespace std::string_literals;
		return typeid(IndexBufferObject).name() + "#"s + tag;
	}
};
