#include "pch.h"
#include "TileType.h"

#include "Asset.h"

std::shared_ptr<Tile> TileType::NONE = nullptr;
std::shared_ptr<Tile> TileType::DIRT = nullptr;
std::shared_ptr<Tile> TileType::GRASS = nullptr;

std::shared_ptr<LogicTile> TileType::LOGIC_NONE = nullptr;
std::shared_ptr<LogicTile> TileType::LOGIC_SPAWN_UP = nullptr;
std::shared_ptr<LogicTile> TileType::LOGIC_SPAWN_DOWN = nullptr;
std::shared_ptr<LogicTile> TileType::LOGIC_SPAWN_LEFT = nullptr;
std::shared_ptr<LogicTile> TileType::LOGIC_SPAWN_RIGHT = nullptr;
std::shared_ptr<LogicTile> TileType::LOGIC_CHECKPOINT_UP = nullptr;
std::shared_ptr<LogicTile> TileType::LOGIC_CHECKPOINT_DOWN = nullptr;
std::shared_ptr<LogicTile> TileType::LOGIC_CHECKPOINT_LEFT = nullptr;
std::shared_ptr<LogicTile> TileType::LOGIC_CHECKPOINT_RIGHT = nullptr;

std::array<std::weak_ptr<Tile>, TileType::NUM_TYPES> TileType::sArray;
std::array<std::weak_ptr<LogicTile>, TileType::NUM_LOGIC_TYPES> TileType::sLogicArray;


void TileType::Load() noexcept
{
	TileType::NONE = std::make_shared<Tile>("Empty tile", Asset::GetTexture("Assets/Textures/none.png"), 0);
	TileType::DIRT = std::make_shared<Tile>("Dirt tile", Asset::GetTexture("Assets/Textures/dirt.png"), 1);
	TileType::GRASS = std::make_shared<Tile>("Grass tile", Asset::GetTexture("Assets/Textures/grass.png"), 2);

	TileType::LOGIC_NONE = std::make_shared<LogicTile>("Logic empty tile", Asset::GetTexture("Assets/Textures/logic_none.png"), 0);
	TileType::LOGIC_SPAWN_UP = std::make_shared<LogicTile>("Spawn up", Asset::GetTexture("Assets/Textures/spawn_up.png"), 1);
	TileType::LOGIC_SPAWN_DOWN = std::make_shared<LogicTile>("Spawn down", Asset::GetTexture("Assets/Textures/spawn_down.png"), 2);
	TileType::LOGIC_SPAWN_LEFT = std::make_shared<LogicTile>("Spawn left", Asset::GetTexture("Assets/Textures/spawn_left.png"), 3);
	TileType::LOGIC_SPAWN_RIGHT = std::make_shared<LogicTile>("Spawn right", Asset::GetTexture("Assets/Textures/spawn_right.png"), 4);

	TileType::LOGIC_CHECKPOINT_UP = std::make_shared<LogicTile>("Checkpoint up", Asset::GetTexture("Assets/Textures/checkpoint_up.png"), 5);
	TileType::LOGIC_CHECKPOINT_DOWN = std::make_shared<LogicTile>("Checkpoint down", Asset::GetTexture("Assets/Textures/checkpoint_down.png"), 6);
	TileType::LOGIC_CHECKPOINT_LEFT = std::make_shared<LogicTile>("Checkpoint left", Asset::GetTexture("Assets/Textures/checkpoint_left.png"), 7);
	TileType::LOGIC_CHECKPOINT_RIGHT = std::make_shared<LogicTile>("Checkpoint right", Asset::GetTexture("Assets/Textures/checkpoint_right.png"), 8);

	sArray = { NONE , DIRT, GRASS };

	sLogicArray =
	{ LOGIC_NONE,
	LOGIC_SPAWN_UP, LOGIC_SPAWN_DOWN, LOGIC_SPAWN_LEFT, LOGIC_SPAWN_RIGHT,
	LOGIC_CHECKPOINT_UP, LOGIC_CHECKPOINT_DOWN, LOGIC_CHECKPOINT_LEFT, LOGIC_CHECKPOINT_RIGHT };
}

const std::array<std::weak_ptr<Tile>, TileType::NUM_TYPES>& TileType::GetArray() noexcept
{
	return sArray;
}

const std::array<std::weak_ptr<LogicTile>, TileType::NUM_LOGIC_TYPES>& TileType::GetLogicArray() noexcept
{
	return sLogicArray;
}

bool TileType::NotLogicNone(const std::weak_ptr<LogicTile>& tile) noexcept {
	auto tile_ = tile.lock();
	return tile_ != LOGIC_NONE;
}

bool TileType::IsLogicSpawn(const std::weak_ptr<LogicTile>& tile) noexcept
{
	auto tile_ = tile.lock();
	return tile_ == LOGIC_SPAWN_UP  ||
		tile_ == LOGIC_SPAWN_DOWN || 
		tile_ == LOGIC_SPAWN_LEFT || 
		tile_ == LOGIC_SPAWN_RIGHT;
}

bool TileType::IsCheckpoint(const std::weak_ptr<LogicTile>& tile) noexcept
{
	auto tile_ = tile.lock();
	return 
		tile_ == LOGIC_CHECKPOINT_UP ||
		tile_ == LOGIC_CHECKPOINT_DOWN ||
		tile_ == LOGIC_CHECKPOINT_LEFT ||
		tile_ == LOGIC_CHECKPOINT_RIGHT;
}

glm::vec2 TileType::GetLogicDir(const std::weak_ptr<LogicTile>& tile) noexcept
{
	auto tile_ = tile.lock();
	if (tile_ == LOGIC_SPAWN_UP || tile_ == LOGIC_CHECKPOINT_UP) {
		return { 0, 1 };
	}
	else if (tile_ == LOGIC_SPAWN_DOWN || tile_ == LOGIC_CHECKPOINT_DOWN) {
		return { 0, -1 };
	}
	else if (tile_ == LOGIC_SPAWN_LEFT || tile_ == LOGIC_CHECKPOINT_LEFT) {
		return { -1, 0 };
	}
	else if (tile_ == LOGIC_SPAWN_RIGHT || tile_ == LOGIC_CHECKPOINT_RIGHT) {
		return { 1, 0 };
	}
	else {
		return { 0, 0 };
	}
}

