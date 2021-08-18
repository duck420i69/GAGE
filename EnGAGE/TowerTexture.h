#pragma once

#include "Texture.h"

#include <memory>

struct TowerTexture {
	std::weak_ptr<Texture> base_texture;
	std::weak_ptr<Texture> cannon_texture;
};