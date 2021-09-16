#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#define TYPE_MACRO \
    X(Int) \
    X(Float) \
    X(Vec2) \
    X(Vec3) \
    X(Vec4) \
    X(Mat4) \

namespace DynamicUniform {
	enum class Type {
		Empty,
#define X(name) name,
		TYPE_MACRO
#undef X
	};

	template<Type T>
	struct TypeMap;
	template<>
	struct TypeMap<Type::Int> {
		using SysType = int;
		static constexpr unsigned int Aligment = sizeof(int) * 4;
		static constexpr const char* UID = "I";
	};

	template<>
	struct TypeMap<Type::Float> {
		using SysType = float;
		static constexpr unsigned int Aligment = sizeof(float) * 4;
		static constexpr const char* UID = "F";
	};

	template<>
	struct TypeMap<Type::Vec2> {
		using SysType = glm::vec2;
		static constexpr unsigned int Aligment = sizeof(float) * 4;
		static constexpr const char* UID = "F2";
	};

	template<>
	struct TypeMap<Type::Vec3> {
		using SysType = glm::vec3;
		static constexpr unsigned int Aligment = sizeof(float) * 4;
		static constexpr const char* UID = "F3";
	};

	template<>
	struct TypeMap<Type::Vec4> {
		using SysType = glm::vec4;
		static constexpr unsigned int Aligment = sizeof(float) * 4;
		static constexpr const char* UID = "F4";
	};

	template<>
	struct TypeMap<Type::Mat4> {
		using SysType = glm::mat4x4;
		static constexpr unsigned int Aligment = sizeof(float) * 16;
		static constexpr const char* UID = "M4x4";
	};

	struct Element {
		Type type;
		size_t offset;

		//Ugly macros, but it works bruh
		static constexpr size_t GetAligment(Type type) noexcept {
			switch (type)
			{
			case DynamicUniform::Type::Empty:
				assert(!"Element type is empty");
				break;
#define X(name) case DynamicUniform::Type::name: return TypeMap<Type::name>::Aligment;
				TYPE_MACRO
#undef X
			}
			assert(!"Unknown element type");
			return 0;
		}
		static constexpr size_t GetSize(Type type) noexcept {
			switch (type)
			{
			case DynamicUniform::Type::Empty:
				assert(!"Element type is empty");
				break;
#define X(name) case DynamicUniform::Type::name: return sizeof(TypeMap<Type::name>::SysType);
				TYPE_MACRO
#undef X
			}
			assert(!"Unknown element type");
			return 0;
		}
		static constexpr const char* GetTypeUID(Type type) noexcept
		{
			switch (type)
			{
			case DynamicUniform::Type::Empty:
				assert(!"Element type is empty");
				break;
#define X(name) case DynamicUniform::Type::name: return TypeMap<Type::name>::UID;
				TYPE_MACRO
#undef X
			}
			assert(!"Unknown element type");
			return 0;
		}
	};

	class Layout {
		friend class Buffer;

		std::unordered_map<std::string, Element> mElements;
	public:
		Layout() = default;
		Layout& PushBack(const std::string& name, const Type& type) noexcept;

		Type& operator[](const std::string& name) noexcept;
		std::string GenerateUID() const noexcept;
	};

	class Buffer {
		std::unordered_map<std::string, Element> mBakedElements;
		std::vector<char> mBuffer;
		const Layout& mLayout;
	public:
		struct ElementRef {
			char* mLocation;
			size_t mSize;

			template<typename T>
			void operator=(const T& t) noexcept {
				memcpy(mLocation, &t, mSize);
			}
		};

		Buffer(const Layout& layout) noexcept;

		ElementRef operator[](const std::string& name) noexcept;

		inline const std::vector<char>& GetRawBuffer() const noexcept { return mBuffer; }
		inline const Layout& GetLayout() const noexcept { return mLayout; }
	};
}

#undef TYPE_MACRO