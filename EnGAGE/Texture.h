#pragma once

class Texture
{
	unsigned int mID;
	unsigned int mWidth, mHeight;
public:
	Texture(const std::string& filePath) noexcept;
	~Texture() noexcept;

	void Bind(int slot = 0) const noexcept;

	inline const unsigned int& GetWidth() const noexcept { return mWidth; }
	inline const unsigned int& GetHeight() const noexcept { return mHeight; }
};