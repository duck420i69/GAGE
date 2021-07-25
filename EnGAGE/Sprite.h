#pragma once

#include "Texture.h"

#include <memory>
#include <array>
#include <glm/vec2.hpp>

class Sprite
{
	std::weak_ptr<Texture> mTexture;
	std::array<glm::vec2, 4> mTexCoords;
public:
	explicit Sprite(const std::weak_ptr<Texture>& texture)  noexcept :
		Sprite(texture, {
				mTexCoords[0] = { 1, 1 },
				mTexCoords[1] = { 1, 0 },
				mTexCoords[2] = { 0, 0 },
				mTexCoords[3] = { 0, 1 }
		})
	{}

	explicit Sprite(const std::weak_ptr<Texture>& texture, const std::array<glm::vec2, 4>& tex_coords) noexcept
		: mTexture(texture), mTexCoords(tex_coords)
	{}

	inline const std::weak_ptr<Texture>& GetTexture() const noexcept { return mTexture; }
	inline const std::array<glm::vec2, 4>& GetTexCoords() const noexcept { return mTexCoords; }

	inline const bool operator!=(const Sprite& other) const noexcept
	{
		return
			mTexture.lock() != other.mTexture.lock() ||
			mTexCoords != other.mTexCoords;
	}
};