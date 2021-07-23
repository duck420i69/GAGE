#pragma once


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

	void UploadMat4x4(const std::string& name, const float* ptr) const;
	void UploadTexture(const std::string& name, const int& slot) const;
	void UploadIntArr(const std::string& name, const int* arr, const uint32_t size) const;
private:
	void Load(const std::string& file_path, uint32_t type) noexcept;

	int GetUniformLocation(const std::string& name) const;
};