#pragma once

#include <glm/vec2.hpp>
#include "LogicTileTypeEnum.h"

struct CheckPoint {
	glm::ivec2 pos;
	LogicTileType tile;
};