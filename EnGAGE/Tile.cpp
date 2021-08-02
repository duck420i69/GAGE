#include "pch.h"
#include "Tile.h"

Tile::Tile(const std::weak_ptr<Texture>& texture, unsigned int id) noexcept :
	mTexture(texture), mID(id)
{
}
