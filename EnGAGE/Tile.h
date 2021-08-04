#pragma once

#include "Texture.h"

template<bool isLogic>
class Tile
{
	std::weak_ptr<Texture> mTexture;
	unsigned int mID;
public:
	Tile(const std::weak_ptr<Texture>& texture, unsigned int id) noexcept :
		mTexture(texture), mID(id)
	{
	}

	inline const std::weak_ptr<Texture> GetTexture() const noexcept { return mTexture; }
	inline const unsigned int& GetID() const noexcept { return mID; }
};