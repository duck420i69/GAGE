#pragma once

#include "Tile.h"

#include <memory>

class TileType
{
public:
	static constexpr unsigned int NUM_TYPES = 3;
	static std::shared_ptr<Tile> NONE, DIRT, GRASS;

	static void Load() noexcept;
	static std::array<std::weak_ptr<Tile>, NUM_TYPES> GetArray() noexcept;
};