#pragma once

class Texture
{
	std::string  mName;
	unsigned int mID;
	unsigned int mWidth, mHeight;
public:
	Texture(const std::string& file_path) noexcept;
	~Texture() noexcept;

	void Bind(int slot = 0) const noexcept;

	inline const unsigned int& GetWidth() const noexcept { return mWidth; }
	inline const unsigned int& GetHeight() const noexcept { return mHeight; }
	inline const unsigned int& GetID() const noexcept { return mID; }
	inline const std::string& GetName() const noexcept { return mName;  }
};