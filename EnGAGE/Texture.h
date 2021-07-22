#pragma once

class Texture
{
	uint32_t mID;
public:
	Texture(const std::string& filePath) noexcept;
	~Texture() noexcept;

	void Bind(int slot = 0) const noexcept;
};