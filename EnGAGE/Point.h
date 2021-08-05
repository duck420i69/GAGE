#pragma once

#include "LogicTile.h"
#include <memory>
#include <glm/vec2.hpp>

struct Point {
	std::weak_ptr<LogicTile> tile;
	glm::vec2 position;
};