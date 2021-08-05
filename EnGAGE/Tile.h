#pragma once

#include "Texture.h"

class Tile
{
	std::string mName;
	std::weak_ptr<Texture> mTexture;
	unsigned int mID;
public:
	Tile(const std::string& name, const std::weak_ptr<Texture>& texture, unsigned int id) noexcept :
		mName(name), mTexture(texture), mID(id)
	{}

	inline const std::weak_ptr<Texture> GetTexture() const noexcept { return mTexture; }
	inline const unsigned int& GetID() const noexcept { return mID; }
	inline const std::string& GetName() const noexcept { return mName; }
};