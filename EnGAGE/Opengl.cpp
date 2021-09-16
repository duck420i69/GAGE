#include "pch.h"
#include "Opengl.h"

#include "Window.h"
#include "Logger.h"
#include "Utils.h"

#include <glad/glad.h>
#include <stb_image.h>

void MessageCallbackFn(
	GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* user_param)
{
	if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) return;
	Logger::info("{}", message);
}

std::vector<VertexArray> Opengl::s_vaos;
std::vector<VertexBuffer> Opengl::s_vbos;
std::vector<Texture> Opengl::s_textures;
std::vector<ShaderProgram> Opengl::s_shaders;
glm::mat4x4 Opengl::s_projection = glm::mat4x4(1.0f), Opengl::s_camera = glm::mat4x4(1.0f);

void Opengl::Init() noexcept
{
#	ifndef NDEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallbackFn, nullptr);
#	endif // !NDEBUG

	int texture_units, uniform_binding, uniform_block_size;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &texture_units);
	glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &uniform_binding);
	glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &uniform_block_size);

	Logger::info("Max texture image units: {}", texture_units);
	Logger::info("Max uniform buffer bindings: {}", uniform_binding);
	Logger::info("Max uniform buffer block size: {}", uniform_block_size);

	//Enable n stuffs
	glCullFace(GL_BACK);
}

void Opengl::Clear() noexcept
{
	glViewport(0, 0, Window::GetWidth(), Window::GetHeight());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

void Opengl::Destroy() noexcept
{
	glDeleteVertexArrays(s_vaos.size(), s_vaos.data());
	glDeleteBuffers(s_vbos.size(), s_vbos.data());
	glDeleteTextures(s_textures.size(), s_textures.data());
	for (const auto& program : s_shaders)
		glDeleteProgram(program);
	Logger::info("Clearing gpu memory !");
}

VertexArray Opengl::CreateVertexArray() noexcept
{
	VertexArray vao;
	glGenVertexArrays(1, &vao);
	s_vaos.push_back(vao);
	return vao;
}

VertexBuffer Opengl::CreateVertexBuffer(const uint64_t size, const void* data, const BufferUsage usage) noexcept
{
	VertexBuffer vbo;

	GLenum opengl_usage = GetBufferUsage(usage);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size, data, opengl_usage);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	s_vbos.push_back(vbo);
	return vbo;
}

VertexBuffer Opengl::CreateIndexBuffer(const uint64_t size, const unsigned int* data, const BufferUsage usage)
{
	VertexBuffer vbo;
	GLenum opengl_usage = GetBufferUsage(usage);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned int), data, opengl_usage);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	s_vbos.push_back(vbo);
	return vbo;
}

UniformBuffer Opengl::CreateUniformBuffer(const unsigned int slot, const uint64_t size, const void* data, const BufferUsage usage) noexcept
{
	UniformBuffer vbo;

	GLenum opengl_usage = GetBufferUsage(usage);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_UNIFORM_BUFFER, vbo);
	glBufferData(GL_UNIFORM_BUFFER, size, data, opengl_usage);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, slot, vbo);
	s_vbos.push_back(vbo);
	return vbo;
}

Texture Opengl::LoadTexture(const std::string& path, const Opengl::TextureFilter min_filter, const Opengl::TextureFilter mag_filter, const Opengl::TextureWrap wrap, int* out_width, int* out_height, bool* has_alpha) noexcept
{
	Logger::info("Loading texture: {}", path);
	return LoadTextureInternal(path, min_filter, mag_filter, wrap, out_width, out_height, has_alpha);
}

ShaderProgram Opengl::LoadShader(const std::string& path) noexcept
{
	Logger::info("Loading shader: {}", path);
	return LoadShaderProgramInternal(path + "_VS.glsl", path + "_FS.glsl");
}

ShaderProgram Opengl::LoadShader(const std::string& vertex_path, const std::string& fragment_path) noexcept
{
	Logger::info("Loading shader: {}, {}", vertex_path, fragment_path);
	return LoadShaderProgramInternal(vertex_path, fragment_path);
}

void Opengl::LoadUniformInteger(const ShaderProgram program, const std::string& name, const int i)
{
	glUniform1i(glGetUniformLocation(program, name.c_str()), i);
}

void Opengl::Layout(const unsigned int slot, const unsigned int size, const unsigned int stride, const unsigned int offset) noexcept
{
	glEnableVertexAttribArray(slot);
	glVertexAttribPointer(slot, size, GL_FLOAT, GL_FALSE, stride, (const void*)(offset));
}


void Opengl::BindVertexArray(const VertexArray va) noexcept
{
	glBindVertexArray(va);
}

void Opengl::BindVertexBuffer(const VertexBuffer vb) noexcept
{
	glBindBuffer(GL_ARRAY_BUFFER, vb);
}

void Opengl::BindUniformBuffer(const UniformBuffer ub, const uint32_t slot) noexcept
{
	//glBindBuffer(GL_UNIFORM_BUFFER, ub);
	glBindBufferBase(GL_UNIFORM_BUFFER, slot, ub);
}

void Opengl::BindIndexBuffer(const VertexBuffer ib) noexcept
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
}

void Opengl::BindProgram(const ShaderProgram program) noexcept
{
	glUseProgram(program);
}

void Opengl::BindTexture(const Texture tex, const unsigned int slot) noexcept
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, tex);
}

void Opengl::TextureParameters(const TextureFilter min_filter, const TextureFilter mag_filter, const TextureWrap wrap)
{
	assert(mag_filter != TextureFilter::NEAREST_MIPMAP_LINEAR && mag_filter != TextureFilter::NEAREST_MIPMAP_NEAREST &&
		mag_filter != TextureFilter::LINEAR_MIPMAP_LINEAR && mag_filter != TextureFilter::LINEAR_MIPMAP_NEAREST && "Texture mag filter should only be NEAREST or LINEAR");
	auto gl_min_filter = GetTextureFilter(min_filter);
	auto gl_mag_filter = GetTextureFilter(mag_filter);
	auto gl_wrap = GetTextureWrap(wrap);
	if (min_filter == TextureFilter::LINEAR_MIPMAP_LINEAR || min_filter == TextureFilter::LINEAR_MIPMAP_NEAREST ||
		min_filter == TextureFilter::NEAREST_MIPMAP_LINEAR || min_filter == TextureFilter::NEAREST_MIPMAP_NEAREST)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gl_min_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gl_mag_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, gl_wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, gl_wrap);
}

void Opengl::UpdateUniformBuffer(const UniformBuffer ub, const uint64_t size, const void* data) noexcept
{
	glBindBuffer(GL_UNIFORM_BUFFER, ub);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Opengl::DrawIndexed(const int count) noexcept
{
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}

void Opengl::SetProjection(const glm::mat4x4& proj) noexcept
{
	s_projection = proj;
}

const glm::mat4x4& Opengl::GetProjection() noexcept
{
	return s_projection;
}

void Opengl::SetCamera(const glm::mat4x4& cam) noexcept
{
	s_camera = cam;
}

const glm::mat4x4& Opengl::GetCamera() noexcept
{
	return s_camera;
}

void Opengl::Enable(State state)
{
	glEnable(GetState(state));
}

void Opengl::Disable(State state)
{
	glDisable(GetState(state));
}

void Opengl::BlendFunc(Blend src, Blend dest)
{
	glBlendFunc(GetBlendState(src), GetBlendState(dest));
}

void Opengl::BlendEquation(BlendOp op)
{
	glBlendEquation(GetBlendOp(op));
}

constexpr unsigned int Opengl::GetBufferUsage(const BufferUsage usage) noexcept
{
	switch (usage) {
	case BufferUsage::DYNAMIC:
		return GL_DYNAMIC_DRAW;
	case BufferUsage::STREAM:
		return GL_STREAM_DRAW;
	case BufferUsage::STATIC:
		return GL_STATIC_DRAW;
	}
	return GL_STATIC_DRAW;
}

constexpr unsigned int Opengl::GetTextureFilter(const TextureFilter filter) noexcept
{
	switch (filter)
	{
	case TextureFilter::LINEAR:
		return GL_LINEAR;
	case TextureFilter::NEAREST:
		return GL_NEAREST;
	case TextureFilter::NEAREST_MIPMAP_NEAREST:
		return GL_NEAREST_MIPMAP_NEAREST;
	case TextureFilter::LINEAR_MIPMAP_NEAREST:
		return GL_LINEAR_MIPMAP_NEAREST;
	case TextureFilter::NEAREST_MIPMAP_LINEAR:
		return GL_NEAREST_MIPMAP_LINEAR;
	case TextureFilter::LINEAR_MIPMAP_LINEAR:
		return GL_LINEAR_MIPMAP_LINEAR;
	}

	return GL_NEAREST;
}

constexpr unsigned int Opengl::GetTextureWrap(const TextureWrap filter) noexcept
{
	switch (filter)
	{
	case TextureWrap::REPEAT:
		return GL_REPEAT;
	case TextureWrap::MIRRORED_REPEAT:
		return GL_MIRRORED_REPEAT;
	case TextureWrap::CLAMP_TO_EDGE:
		return GL_CLAMP_TO_EDGE;
	case TextureWrap::CLAMP_TO_BORDER:
		return GL_CLAMP_TO_BORDER;
	}
	return GL_REPEAT;;
}

constexpr unsigned int Opengl::GetBlendState(const Blend blend) noexcept
{
	switch (blend)
	{
	case Blend::ZERO: return GL_ZERO;
	case Blend::ONE: return GL_ONE;
	case Blend::SRC_COLOR: return GL_SRC_COLOR;
	case Blend::ONE_MINUS_SRC_COLOR: return GL_ONE_MINUS_SRC_COLOR;
	case Blend::DST_COLOR: return GL_DST_COLOR;
	case Blend::ONE_MINUS_DST_COLOR: return GL_ONE_MINUS_DST_COLOR;
	case Blend::SRC_ALPHA: return GL_SRC_ALPHA;
	case Blend::ONE_MINUS_SRC_ALPHA: return GL_ONE_MINUS_SRC_ALPHA;
	case Blend::DST_ALPHA:  return GL_DST_ALPHA;
	case Blend::ONE_MINUS_DST_ALPHA: return GL_ONE_MINUS_DST_ALPHA;
	case Blend::CONSTANT_COLOR: return GL_CONSTANT_COLOR;
	case Blend::ONE_MINUS_CONSTANT_COLOR: return GL_ONE_MINUS_CONSTANT_COLOR;
	case Blend::CONSTANT_ALPHA: return GL_CONSTANT_ALPHA;
	case Blend::ONE_MINUS_CONSTANT_ALPHA:  return GL_ONE_MINUS_CONSTANT_ALPHA;
	case Blend::SRC_ALPHA_SATURATE: return GL_SRC_ALPHA_SATURATE;
	case Blend::SRC1_COLOR: return GL_SRC1_COLOR;
	case Blend::ONE_MINUS_SRC1_COLOR: return GL_ONE_MINUS_SRC1_COLOR;
	case Blend::SRC1_ALPHA: return GL_SRC1_ALPHA;
	case Blend::ONE_MINUS_SRC1_ALPHA: return GL_ONE_MINUS_SRC1_ALPHA;
	}
	return 0;
}

constexpr unsigned int Opengl::GetBlendOp(const BlendOp state) noexcept
{
	switch (state)
	{
		case BlendOp::FUNC_ADD: return GL_FUNC_ADD;
		case BlendOp::FUNC_SUBTRACT: return GL_FUNC_SUBTRACT;
		case BlendOp::FUNC_REVERSE_SUBTRACT: return GL_FUNC_REVERSE_SUBTRACT;
		case BlendOp::MIN: return GL_MIN;
		case BlendOp::MAX: return GL_MAX;
	}
	return 0;
}

constexpr unsigned int Opengl::GetState(const State state) noexcept
{
	switch (state)
	{
	case State::BLEND:
		return GL_BLEND;
	case State::CULL:
		return GL_CULL_FACE;
	}
	return 0;
}

Texture Opengl::LoadTextureInternal(const std::string& path, const Opengl::TextureFilter min_filter, const Opengl::TextureFilter mag_filter, const Opengl::TextureWrap wrap, int* out_width, int* out_height, bool* has_alpha) noexcept
{
	Texture texture = 0;
	int width = 0, height = 0, bpp = 0;
	try
	{
		stbi_uc* image_data = nullptr;
		stbi_set_flip_vertically_on_load(1);
		image_data = stbi_load(path.c_str(), &width, &height, &bpp, 0);
		if (!image_data)
		{
			std::stringstream ss;
			ss << "Failed to load image : " << path << ", stb_image's report: " <<
				std::string(stbi_failure_reason());

			throw std::runtime_error(ss.str());
		}


		unsigned int internal_format = 0, file_format = 0;
		
		if (bpp == STBI_rgb_alpha) {
			internal_format = GL_COMPRESSED_RGBA;
			file_format = GL_RGBA;
		}
		else if (bpp == STBI_rgb) {
			internal_format = GL_COMPRESSED_RGB;
			file_format = GL_RGB;
		}
		else if (bpp == STBI_grey) {

			internal_format = GL_COMPRESSED_RED;
			file_format = GL_RED;
		}
		
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glHint(GL_TEXTURE_COMPRESSION_HINT, GL_FASTEST);
		glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, file_format, GL_UNSIGNED_BYTE, image_data);
		
		GLfloat value;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &value);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, value);

		TextureParameters(min_filter, mag_filter, wrap);

		glBindTexture(GL_TEXTURE_2D, 0);

		stbi_image_free(image_data);
	}
	catch (std::exception& e)
	{
		Logger::error("Texture exception thrown: {}", e.what());
	}
	if (out_width)
		*out_width = width;
	if (out_height)
		*out_height = height;
	if (has_alpha) {
		*has_alpha = (bpp == STBI_rgb_alpha);
	}
	s_textures.push_back(texture);
	return texture;
}

uint32_t Opengl::LoadShaderInternal(const std::string& path, uint32_t type) noexcept
{
	std::function<void(std::string&, const std::filesystem::path&)> process_file;

	process_file = [&](std::string& source,const std::filesystem::path& source_path) {
		size_t pos;
		if ((pos = source.find("#include")) != std::string::npos) {
			std::string include_line = source.substr(pos, source.find("\n", pos) - pos);
			size_t line_pos = include_line.find_first_of("\"") + 1;
			std::string include_name = include_line.substr(line_pos, include_line.find_last_of("\"") - line_pos);

			source.erase(pos, source.find("\n", pos) - pos);

			std::ifstream file;
			std::stringstream ss;
			file.exceptions(std::ios::badbit);
			file.open(source_path.string() + "\\" + include_name);
			ss << file.rdbuf();
			source.insert(pos, ss.str());
			file.close();
			process_file(source, source_path);
		}
	};


	
	try
	{
		std::ifstream file;
		std::stringstream ss;
		std::string source;
		file.exceptions(std::ios::badbit);
		file.open(path);

		ss << file.rdbuf();
		source = ss.str();

		process_file(source, std::filesystem::path(path).parent_path());


		file.close();

		uint32_t shader = glCreateShader(type);
		const char* source_c_str = source.c_str();
		glShaderSource(shader, 1, &source_c_str, nullptr);
		glCompileShader(shader);

		int compile_status;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
		if (compile_status == GL_FALSE)
		{
			//Throw glsl error
			int log_length;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);

			std::vector<char> message(log_length);
			glGetShaderInfoLog(shader, log_length, &log_length, message.data());
			throw std::string(message.data());
		}
		return shader;
	}
	catch (const std::ios_base::failure& e)
	{
		Logger::error("Error loading shader file: {}, io: {}", path, e.what());
	}
	catch (std::string& glsl_error)
	{
		Logger::error("Shader compiling shader: {}, error: {}", path, glsl_error);
	}
	return 0;
}

ShaderProgram Opengl::LoadShaderProgramInternal(const std::string& vertex_path, const std::string& fragment_path) noexcept
{
	ShaderProgram program = glCreateProgram();
	std::vector<uint32_t> shaders;
	shaders.push_back(LoadShaderInternal(vertex_path, GL_VERTEX_SHADER));
	shaders.push_back(LoadShaderInternal(fragment_path, GL_FRAGMENT_SHADER));


	for (const auto& shader : shaders) {
		glAttachShader(program, shader);
	}
	glLinkProgram(program);
	glValidateProgram(program);

	for (const auto& shader : shaders) {
		glDeleteShader(shader);
	}


	return program;
}

