#pragma once

#include <cstdint>

#include <vector>
#include <unordered_map>
#include <string>

using VertexArray = unsigned int;
using VertexBuffer = unsigned int;
using UniformBuffer = unsigned int;
using ShaderProgram = unsigned int;

struct Texture {
	unsigned int id;
	int width, height;
};


class Opengl
{
	static std::vector<VertexArray> s_vaos;
	static std::vector<VertexBuffer> s_vbos;
	static std::unordered_map<std::string, Texture> s_texture_map;
	static std::unordered_map<std::string, ShaderProgram> s_shader_map;
public:
	enum class BufferUsage {
		DYNAMIC, STATIC, STREAM
	};
public:
	static void Init() noexcept;
	static void Clear() noexcept;
	static void Destroy() noexcept;

	static VertexArray CreateVertexArray() noexcept;
	static VertexBuffer CreateVertexBuffer(const uint64_t size, const void* data, const BufferUsage usage = BufferUsage::STATIC) noexcept;
	static VertexBuffer CreateIndexBuffer(const uint64_t size, const unsigned int* data, const BufferUsage usage = BufferUsage::STATIC);
	static UniformBuffer CreateUniformBuffer(const unsigned int slot, const uint64_t size, const void* data, const BufferUsage usage = BufferUsage::DYNAMIC) noexcept;
	static Texture LoadTexture(const std::string& path) noexcept;
	static ShaderProgram LoadShader(const std::string& path) noexcept;

	static void Layout(const unsigned int slot, const unsigned int size, const unsigned int stride, const unsigned int offset) noexcept;
	static void BindVertexArray(const VertexArray va) noexcept;
	static void BindVertexBuffer(const VertexBuffer vb) noexcept;
	static void BindUniformBuffer(const UniformBuffer ub) noexcept;
	static void BindIndexBuffer(const VertexBuffer ib) noexcept;
	static void BindProgram(const ShaderProgram program) noexcept;
	static void UpdateUniformBuffer(const UniformBuffer ub, const uint64_t size, const void* data) noexcept;


	static void DrawIndexed(const uint64_t count) noexcept;
private:
	static unsigned int GetBufferUsage(const BufferUsage usage) noexcept;

	static void LoadTextureInternal(const std::string& path, Texture* texture) noexcept;

	static uint32_t LoadShaderInternal(const std::string& path, uint32_t type) noexcept;
	static ShaderProgram LoadShaderProgramInternal(const std::string& vertex_path, const std::string& fragment_path) noexcept;
};