#pragma once

#include "Tile.h"

#include <memory>
#include <glm/vec2.hpp>

#include "TileTypeEnum.h"
#include "LogicTileTypeEnum.h"

namespace TileManager {
	std::weak_ptr<Texture> GetTexture(TileType type) noexcept;
	std::weak_ptr<Texture> GetTexture(LogicTileType type) noexcept;
	Tile Get(TileType type) noexcept;
	Tile Get(LogicTileType type) noexcept;
	bool NotLogicNone(LogicTileType type) noexcept;
	bool IsLogicSpawn(LogicTileType type) noexcept;
	bool IsCheckPoint(LogicTileType type) noexcept;
	glm::vec2 GetLogicDir(LogicTileType tile) noexcept;
}
