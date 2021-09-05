#include "pch.h"
#include "DynamicVertex.h"


namespace DynamicVertex {
	const VertexLayout::Element& VertexLayout::ResolveByIndex(size_t i) const noexcept
	{
		return elements[i];
	}
	VertexLayout& VertexLayout::Append(Type T) noexcept
	{
		elements.emplace_back(T, Size());
		return *this;
	}
	size_t VertexLayout::Size() const noexcept
	{
		return elements.empty() ? 0u : elements.back().GetOffsetAfter();
	}
	std::string VertexLayout::GetCode() const noexcept
	{
		std::string code;

		for (const auto& e : elements) {
			code += e.GetCode();
		}

		return code;
	}
	Vertex::Vertex(char* pData, const VertexLayout& layout) noexcept :
		pData(pData), layout(layout)
	{
		assert(pData != nullptr);
	}
	VertexBuffer::VertexBuffer(VertexLayout layout) noexcept :
		layout(std::move(layout)) {}
}
