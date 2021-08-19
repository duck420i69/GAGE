#pragma once

#include "Tile.h"

#include <memory>
#include <glm/vec2.hpp>

#include "TileTypeEnum.h"
#include "LogicTileTypeEnum.h"

namespace TileManager {
	std::weak_ptr<Texture> GetTexture(TileType type) noexcept;
	std::weak_ptr<Texture> GetTexture(LogicTileType type) noexcept;
	std::unique_ptr<Tile> Get(TileType type) noexcept;
	std::unique_ptr<Tile> Get(LogicTileType type) noexcept;
	bool NotLogicNone(LogicTileType type) noexcept;
	bool IsLogicSpawn(LogicTileType type) noexcept;
	bool IsCheckPoint(LogicTileType type) noexcept;
	glm::vec2 GetLogicDir(LogicTileType tile) noexcept;
}

/*class TileType
{
public:
	static constexpr unsigned int NUM_TYPES = 3;
	static constexpr unsigned int NUM_LOGIC_TYPES = 9;
	static std::shared_ptr<Tile> NONE, DIRT, GRASS;
	static std::shared_ptr<LogicTile> LOGIC_NONE, 
		LOGIC_SPAWN_UP, LOGIC_SPAWN_DOWN, LOGIC_SPAWN_LEFT, LOGIC_SPAWN_RIGHT,
		LOGIC_CHECKPOINT_UP, LOGIC_CHECKPOINT_DOWN, LOGIC_CHECKPOINT_LEFT, LOGIC_CHECKPOINT_RIGHT;

	static std::array<std::weak_ptr<Tile>, NUM_TYPES> sArray;
	static std::array<std::weak_ptr<LogicTile>, NUM_LOGIC_TYPES> sLogicArray;

	static void Load() noexcept;
	static const std::array<std::weak_ptr<Tile>, NUM_TYPES>& GetArray() noexcept;
	static const std::array<std::weak_ptr<LogicTile>, NUM_LOGIC_TYPES>& GetLogicArray() noexcept;
	static bool NotLogicNone(const std::weak_ptr<LogicTile>& tile) noexcept;
	static bool IsLogicSpawn(const std::weak_ptr<LogicTile>& tile) noexcept;
	static bool IsCheckpoint(const std::weak_ptr<LogicTile>& tile) noexcept;
	static glm::vec2 GetLogicDir(const std::weak_ptr<LogicTile>& tile) noexcept;
};*/