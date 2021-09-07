#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <cassert>

namespace DynamicVertex {
	
	class VertexLayout {
	public :
		enum class Type {
			Position2D,
			Position3D,
			Texture2D,
			Normal,
			Color3,
			Color4,
			Tangent,
			BiTangent
		};

		template<Type> struct Map;

		template<> struct Map<Type::Position2D>
		{
			using SysType = glm::vec2;
			static constexpr const char* code = "P2";
		};

		template<> struct Map<Type::Position3D>
		{
			using SysType = glm::vec3;
			static constexpr const char* code = "P3";
		};
		template<> struct Map<Type::Texture2D>
		{
			using SysType = glm::vec2;
			static constexpr const char* code = "T2";
		};
		template<> struct Map<Type::Normal>
		{
			using SysType = glm::vec3;
			static constexpr const char* code = "N";
		};
		template<> struct Map<Type::Color3>
		{
			using SysType = glm::vec3;
			static constexpr const char* code = "C3";
		};
		template<> struct Map<Type::Color4>
		{
			using SysType = glm::vec4;
			static constexpr const char* code = "C4";
		};
		template<> struct Map<Type::Tangent>
		{
			using SysType = glm::vec3;
			static constexpr const char* code = "TA";
		};
		template<> struct Map<Type::BiTangent>
		{
			using SysType = glm::vec3;
			static constexpr const char* code = "BTA";
		};

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
				case Type::Position2D:
					return sizeof(Map<Type::Position2D>::SysType);
				case Type::Position3D:
					return sizeof(Map<Type::Position3D>::SysType);
				case Type::Texture2D:
					return sizeof(Map<Type::Texture2D>::SysType);
				case Type::Normal:
					return sizeof(Map<Type::Normal>::SysType);
				case Type::Color3:
					return sizeof(Map<Type::Color3>::SysType);
				case Type::Color4:
					return sizeof(Map<Type::Color4>::SysType);
				case Type::Tangent:
					return sizeof(Map<Type::Tangent>::SysType);
				case Type::BiTangent:
					return sizeof(Map<Type::BiTangent>::SysType);
				}
				return 0u;
			}

			constexpr const char* GetCode() const noexcept {
				switch (type)
				{
				case Type::Position2D:
					return Map<Type::Position2D>::code;
				case Type::Position3D:
					return Map<Type::Position3D>::code;
				case Type::Texture2D:
					return Map<Type::Texture2D>::code;
				case Type::Normal:
					return Map<Type::Normal>::code;
				case Type::Color3:
					return Map<Type::Color3>::code;
				case Type::Color4:
					return Map<Type::Color4>::code;
				case Type::Tangent:
					return Map<Type::Tangent>::code;
				case Type::BiTangent:
					return Map<Type::BiTangent>::code;
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


		inline size_t GetElementCount() const noexcept { return elements.size(); }
		inline const std::vector<Element>& GetElements() const noexcept { return elements; }
	private:
		std::vector<Element> elements;
	};

	class Vertex {
		friend class VertexBuffer;

		char* pData = nullptr;
		const VertexLayout& layout;
		
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
			switch (element.type) {
			case VertexLayout::Type::Position2D:
				SetAttribute<VertexLayout::Type::Position2D>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Type::Position3D:
				SetAttribute<VertexLayout::Type::Position3D>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Type::Texture2D:
				SetAttribute<VertexLayout::Type::Texture2D>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Type::Normal:
				SetAttribute<VertexLayout::Type::Normal>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Type::Color3:
				SetAttribute<VertexLayout::Type::Color3>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Type::Color4:
				SetAttribute<VertexLayout::Type::Color4>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Type::Tangent:
				SetAttribute<VertexLayout::Type::Tangent>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Type::BiTangent:
				SetAttribute<VertexLayout::Type::BiTangent>(pAttribute, std::forward<T>(val));
				break;
			default:
				assert(!"Bad element type");
				break;
			}
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