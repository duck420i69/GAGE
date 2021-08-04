#pragma once

#include "Texture.h"

#include <memory>

class LogicTile {
	std::weak_ptr<Texture> mTexture;
	unsigned int mID;
public:
	LogicTile(const std::weak_ptr<Texture>& texture, unsigned int id) noexcept;

	inline const std::weak_ptr<Texture> GetTexture() const noexcept { return mTexture; }
	inline const unsigned int& GetID() const noexcept { return mID; }
};