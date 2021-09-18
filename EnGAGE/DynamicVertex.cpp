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
	bool VertexLayout::Has(Type type) noexcept
	{
		for (const auto& e : elements) {
			if (e.type == type) {
				return true;
			}
		}
		return false;
	}
	Vertex::Vertex(char* pData, const VertexLayout& layout) noexcept :
		pData(pData), layout(layout)
	{
		assert(pData != nullptr);
	}
	VertexBuffer::VertexBuffer(VertexLayout layout) noexcept :
		layout(std::move(layout)) {}

	template<VertexLayout::Type type>
	struct AttributeAiMeshFill {
		static constexpr auto Exec(VertexBuffer* pBuffer, const aiMesh& mesh) noexcept {
			for (unsigned int i = 0; i < mesh.mNumVertices; i++) {
				(*pBuffer)[i].Attr<type>() = VertexLayout::Map<type>::Extract(mesh, i);
			}
		}
	};
	VertexBuffer::VertexBuffer(VertexLayout layout, const aiMesh& mesh) noexcept :
		layout(std::move(layout)) 
	{
		Resize(mesh.mNumVertices);

		for (size_t i = 0, end = layout.GetElementCount(); i < end; i++) {
			VertexLayout::Bridge<AttributeAiMeshFill>(layout.ResolveByIndex(i).type, this, mesh);
		}
	}
	void VertexBuffer::Resize(size_t new_size) noexcept
	{
		const auto size = Size();
		if (size < new_size)
		{
			buffer.resize(buffer.size() + layout.Size() * (new_size - size));
		}
	}
}
