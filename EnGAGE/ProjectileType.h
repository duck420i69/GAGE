#pragma once

#include "Texture.h"

enum class ProjectileType {
	NONE,
	CANNON_BALL,
	COUNT
};

namespace ProjecTileTypeTexture {
	std::weak_ptr<Texture> Get(ProjectileType type) noexcept;
}