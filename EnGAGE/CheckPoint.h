#pragma once

#include <glm/vec2.hpp>
#include "TileType.h"

struct CheckPoint {
	glm::ivec2 pos;
	std::weak_ptr<LogicTile> tile;
};