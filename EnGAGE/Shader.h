#pragma once

#include <cstdint>
#include <string>

class Shader
{
	uint32_t				mProgram;
	std::vector<uint32_t>	mShaders;
public:
	Shader() noexcept;
	~Shader() noexcept;

	void Bind() const;

	void LoadVertex(const std::string& file_path) noexcept;
	void LoadFragment(const std::string& file_path) noexcept;
	void Create() const noexcept;

	void UploadMat4x4(const std::string& name, const float* ptr);
private:
	void Load(const std::string& file_path, uint32_t type) noexcept;

	int GetUniformLocation(const std::string& name);
};