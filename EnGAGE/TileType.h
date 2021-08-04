#pragma once

#include "Tile.h"

#include <memory>

class TileType
{
public:
	static constexpr unsigned int NUM_TYPES = 3;
	static constexpr unsigned int NUM_LOGIC_TYPES = 2;
	static std::shared_ptr<Tile<false>> NONE, DIRT, GRASS;
	static std::shared_ptr<Tile<true>> LOGIC_NONE, LOGIC_SPAWN;

	static void Load() noexcept;
	static std::array<std::weak_ptr<Tile<false>>, NUM_TYPES> GetArray() noexcept;
	static std::array<std::weak_ptr<Tile<true>>, NUM_LOGIC_TYPES> GetLogicArray() noexcept;
};