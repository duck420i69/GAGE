#pragma once

#include <glm/mat4x4.hpp>

class Shader
{
	static std::vector<uint32_t> sPrograms;

	std::string				mName;
	uint32_t				mProgram;
	std::vector<uint32_t>	mShaders;
public:
	static void shutdown() noexcept;

	Shader(const std::string& name) noexcept;

	void loadVertexShader(const std::string& path) noexcept;
	void loadFragmentShader(const std::string& path) noexcept;
	void compileAllShaders() const noexcept;

	int32_t loadUniformLocation(const std::string& name) const noexcept;

	inline const std::string&	getName()    const noexcept { return mName; }
	inline const uint32_t&		getProgram() const noexcept { return mProgram; }
private:
	uint32_t processShader(const std::string& path, uint32_t shaderType) noexcept;
};