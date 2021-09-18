#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <cassert>

#include <assimp/mesh.h>

#define ASSIMP_ELEMENT_EXTRACTOR(member) static SysType Extract(const aiMesh& mesh, size_t i) noexcept { return *reinterpret_cast<const SysType*>(&mesh.member[i]); }

#define VERTEX_LAYOUT_TYPE \
	X(Position2D) \
	X(Position3D) \
	X(Texture2D) \
	X(Normal) \
	X(Tangent) \
	X(BiTangent) \

namespace DynamicVertex {
	
	class VertexLayout {
	public :
		enum class Type {
#define X(el) el,
		VERTEX_LAYOUT_TYPE
#undef X
		};

		template<Type> struct Map;

		template<> struct Map<Type::Position2D>
		{
			using SysType = glm::vec2;
			static constexpr const char* code = "P2";
			ASSIMP_ELEMENT_EXTRACTOR(mVertices)
		};

		template<> struct Map<Type::Position3D>
		{
			using SysType = glm::vec3;
			static constexpr const char* code = "P3";
			ASSIMP_ELEMENT_EXTRACTOR(mVertices)
		};
		template<> struct Map<Type::Texture2D>
		{
			using SysType = glm::vec2;
			static constexpr const char* code = "T2";
			ASSIMP_ELEMENT_EXTRACTOR(mTextureCoords[0])
		};
		template<> struct Map<Type::Normal>
		{
			using SysType = glm::vec3;
			static constexpr const char* code = "N";
			ASSIMP_ELEMENT_EXTRACTOR(mNormals)
		};
		template<> struct Map<Type::Tangent>
		{
			using SysType = glm::vec3;
			static constexpr const char* code = "TA";
			ASSIMP_ELEMENT_EXTRACTOR(mTangents)
		};
		template<> struct Map<Type::BiTangent>
		{
			using SysType = glm::vec3;
			static constexpr const char* code = "BTA";
			ASSIMP_ELEMENT_EXTRACTOR(mBitangents)
		};

		template<template<VertexLayout::Type> class F, typename... Args>
		static constexpr auto Bridge(VertexLayout::Type type, Args&&... args) noexcept {
			switch (type) {
				#define X(el) case VertexLayout::Type::el: return F<VertexLayout::Type::el>::Exec(std::forward<Args>(args)...);
							VERTEX_LAYOUT_TYPE
				#undef X

			}
			assert(!"Invalid element type");
		} 

		class Element {
		public:
			Type type;
			size_t offset;
		public:
			Element(Type type, size_t offset) noexcept :
				type(type), offset(offset) {}
			inline size_t GetOffset() const noexcept { return offset; }
			inline size_t GetOffsetAfter() const noexcept { return offset + Size(); }
			inline size_t Size() const noexcept { return SizeOf(type); }

			static constexpr size_t SizeOf(Type type) noexcept {
				switch (type)
				{
#define X(el) case Type::el: return sizeof(Map<Type::el>::SysType); 
				VERTEX_LAYOUT_TYPE
#undef X
				}
				return 0u;
			}
			constexpr const char* GetCode() const noexcept {
				switch (type)
				{
#define X(el) case Type::el: return Map<Type::BiTangent>::code; 
				VERTEX_LAYOUT_TYPE
#undef X
				}
				return "";
			}
		};
	public:
		template<Type T>
		const Element& Resolve() const noexcept {
			for (const auto& e : elements) {
				if (e.type == T) {
					return e;
				}
			}
			assert(!"could not resolve element type");
			return elements.front();
		}

		const Element& ResolveByIndex(size_t i) const noexcept;
		VertexLayout& Append(Type T) noexcept;
		size_t Size() const noexcept;
		std::string GetCode() const noexcept;
		bool Has(Type type) noexcept;

		inline size_t GetElementCount() const noexcept { return elements.size(); }
		inline const std::vector<Element>& GetElements() const noexcept { return elements; }
	private:
		std::vector<Element> elements;
	};

	class Vertex {
		friend class VertexBuffer;

		char* pData = nullptr;
		const VertexLayout& layout;
	private:
		template<VertexLayout::Type Type>
		struct AttributeSettings {
			template<typename T>
			static const auto Exec(Vertex* pVertex, char* pAttribute, T&& val) noexcept {
				return pVertex->SetAttribute<Type>(pAttribute, std::forward<T>(val));
			}
		};
		
	public:
		explicit Vertex(char* pData, const VertexLayout& layout) noexcept;
		template<VertexLayout::Type DestType, typename SrcType>
		void SetAttribute(char* pAttribute, SrcType&& val) noexcept {

			using Dest = typename VertexLayout::Map<DestType>::SysType;

			if constexpr (std::is_assignable<Dest, SrcType>::value)
			{
				*reinterpret_cast<Dest*>(pAttribute) = val;
			}
			else {
				assert(!"Parameter attributte type mismatch");
			}
		}

		template<VertexLayout::Type T>
		auto& Attr() noexcept {
			char* pAttribute = pData + layout.Resolve<T>().GetOffset();
			return *reinterpret_cast<typename VertexLayout::Map<T>::SysType*>(pAttribute);
		}

		template<typename First, typename... Rest>
		void SetAttributeByIndex(size_t i, First&& first, Rest&&... rest) noexcept {
			SetAttributeByIndex(i, std::forward<First>(first));
			SetAttributeByIndex(i + 1, std::forward<Rest>(rest)...);
		}

		template<typename T>
		void SetAttributeByIndex(size_t i, T&& val) noexcept{
			const VertexLayout::Element& element = layout.ResolveByIndex(i);
			char* pAttribute = pData + element.GetOffset();
			VertexLayout::Bridge<AttributeSettings>(element.type, this, pAttribute, std::forward<T>(val));
		}
	};

	class ConstVertex {
		Vertex vertex;
	public:
		ConstVertex(const Vertex& v) noexcept :
			vertex(v) {}

		template<VertexLayout::Type T>
		const auto& Attr() const noexcept {
			return const_cast<Vertex&>(vertex).Attr<T>();
		}
	};

	class VertexBuffer {
		std::vector<char> buffer;
		VertexLayout layout;
	public:
		VertexBuffer(VertexLayout layout) noexcept;
		VertexBuffer(VertexLayout layout, const aiMesh& mesh) noexcept;
		void Resize(size_t new_size) noexcept;
		inline const VertexLayout& GetLayout() const noexcept { return layout; }
		inline size_t Size() const noexcept { return buffer.size() / layout.Size(); }
		inline size_t SizeBytes() const noexcept { return buffer.size(); }
		inline const char* GetData() const noexcept { return buffer.data(); }

		template<typename... Params>
		void EmplaceBack(Params&&... params) noexcept {
			assert(sizeof...(params) == layout.GetElementCount() && "param count mismatch");
			buffer.resize(buffer.size() + layout.Size());
			Back().SetAttributeByIndex(0u, std::forward<Params>(params)...);
		}


		inline Vertex Back() noexcept {
			assert(buffer.size() != 0);
			return Vertex{ buffer.data() + buffer.size() - layout.Size(), layout };
		}
		inline Vertex Front() noexcept {
			assert(buffer.size() != 0);
			return Vertex{ buffer.data(), layout };
		}
		inline Vertex operator[](size_t i) noexcept {
			assert(i < Size());
			return Vertex{ buffer.data() + layout.Size() * i, layout };
		}

		inline ConstVertex Back() const noexcept {
			return const_cast<VertexBuffer*>(this)->Back();
		}

		inline ConstVertex Front() const noexcept {
			return const_cast<VertexBuffer*>(this)->Front();
		}

		inline ConstVertex operator[](size_t i) const noexcept {
			return const_cast<VertexBuffer&>(*this)[i];
		}
	};
}

#undef VERTEX_LAYOUT_TYPE
#undef ASSIMP_ELEMENT_EXTRACTOR