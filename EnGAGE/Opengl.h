#pragma once

#include <cstdint>

#include <vector>
#include <unordered_map>
#include <string>
#include <glm/ext/matrix_float4x4.hpp>

using VertexArray = unsigned int;
using VertexBuffer = unsigned int;
using UniformBuffer = unsigned int;
using ShaderProgram = unsigned int;
using Texture = unsigned int;


class Opengl
{
	static std::vector<VertexArray> s_vaos;
	static std::vector<VertexBuffer> s_vbos;
	static std::vector<Texture> s_textures;
	static std::vector<ShaderProgram> s_shaders;
	static glm::mat4x4 s_projection, s_camera;
public:
	enum class BufferUsage {
		DYNAMIC, STATIC, STREAM
	};

	enum class TextureFilter {
		LINEAR, NEAREST, 
		NEAREST_MIPMAP_NEAREST, LINEAR_MIPMAP_NEAREST, NEAREST_MIPMAP_LINEAR, LINEAR_MIPMAP_LINEAR
	};
	enum class TextureWrap {
		REPEAT,
		MIRRORED_REPEAT,
		CLAMP_TO_EDGE,
		CLAMP_TO_BORDER
	};

	enum class State {
		CULL,
		BLEND
	};
	enum class BlendOp {
		FUNC_ADD,
		FUNC_SUBTRACT,
		FUNC_REVERSE_SUBTRACT,
		MIN,
		MAX
	};
	enum class Blend {
		ZERO,
		ONE, 
		SRC_COLOR,
		ONE_MINUS_SRC_COLOR,
		DST_COLOR,
		ONE_MINUS_DST_COLOR,
		SRC_ALPHA,
		ONE_MINUS_SRC_ALPHA,
		DST_ALPHA,
		ONE_MINUS_DST_ALPHA,
		CONSTANT_COLOR,
		ONE_MINUS_CONSTANT_COLOR,
		CONSTANT_ALPHA,
		ONE_MINUS_CONSTANT_ALPHA,
		SRC_ALPHA_SATURATE,
		SRC1_COLOR,
		ONE_MINUS_SRC1_COLOR,
		SRC1_ALPHA,
		ONE_MINUS_SRC1_ALPHA,
	};
public:
	static void Init() noexcept;
	static void Clear() noexcept;
	static void Destroy() noexcept;

	static VertexArray CreateVertexArray() noexcept;
	static VertexBuffer CreateVertexBuffer(const uint64_t size, const void* data, const BufferUsage usage = BufferUsage::STATIC) noexcept;
	static VertexBuffer CreateIndexBuffer(const uint64_t size, const unsigned int* data, const BufferUsage usage = BufferUsage::STATIC);
	static UniformBuffer CreateUniformBuffer(const unsigned int slot, const uint64_t size, const void* data, const BufferUsage usage = BufferUsage::DYNAMIC) noexcept;
	static Texture LoadTexture(const std::string& path, const Opengl::TextureFilter min_filter, const Opengl::TextureFilter mag_filter, const Opengl::TextureWrap wrap, int* out_width = nullptr, int* out_height = nullptr, bool* has_alpha = nullptr) noexcept;
	static ShaderProgram LoadShader(const std::string& path) noexcept;
	static ShaderProgram LoadShader(const std::string& vertex_path, const std::string& fragment_path) noexcept;
	static void LoadUniformInteger(const ShaderProgram program, const std::string& name, const int i);

	static void Layout(const unsigned int slot, const unsigned int size, const unsigned int stride, const unsigned int offset) noexcept;
	static void BindVertexArray(const VertexArray va) noexcept;
	static void BindVertexBuffer(const VertexBuffer vb) noexcept;
	static void BindUniformBuffer(const UniformBuffer ub, const uint32_t slot) noexcept;
	static void BindIndexBuffer(const VertexBuffer ib) noexcept;
	static void BindProgram(const ShaderProgram program) noexcept;
	static void BindTexture(const Texture tex, const unsigned int slot = 0) noexcept;
	static void TextureParameters(const TextureFilter min_filter, const TextureFilter mag_filter, const TextureWrap wrap);
	static void UpdateUniformBuffer(const UniformBuffer ub, const uint64_t size, const void* data) noexcept;


	static void DrawIndexed(const int count) noexcept;

	static void SetProjection(const glm::mat4x4& proj) noexcept;
	static const glm::mat4x4& GetProjection() noexcept;

	static void SetCamera(const glm::mat4x4& cam) noexcept;
	static const glm::mat4x4& GetCamera() noexcept;

	static void Enable(State state);
	static void Disable(State state);

	static void BlendFunc(Blend src, Blend dest);
	static void BlendEquation(BlendOp op);
private:
	static constexpr unsigned int GetBufferUsage(const BufferUsage usage) noexcept;
	static constexpr unsigned int GetTextureFilter(const TextureFilter filter) noexcept;
	static constexpr unsigned int GetTextureWrap(const TextureWrap filter) noexcept;
	static constexpr unsigned int GetBlendState(const Blend blend) noexcept;
	static constexpr unsigned int GetBlendOp(const BlendOp state) noexcept;
	static constexpr unsigned int GetState(const State state) noexcept;

	static Texture LoadTextureInternal(const std::string& path, const Opengl::TextureFilter min_filter, const Opengl::TextureFilter mag_filter, const Opengl::TextureWrap wrap, int* out_width, int* out_height, bool* has_alpha) noexcept;

	static uint32_t LoadShaderInternal(const std::string& path, uint32_t type) noexcept;
	static ShaderProgram LoadShaderProgramInternal(const std::string& vertex_path, const std::string& fragment_path) noexcept;
};