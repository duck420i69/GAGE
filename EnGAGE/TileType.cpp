#include "pch.h"
#include "TileType.h"

#include "Asset.h"


std::weak_ptr<Texture> TileManager::GetTexture(TileType type) noexcept
{
	switch (type)
	{
	case TileType::NONE:
		return Asset::GetTexture("Assets/Textures/none.png");
	case TileType::DIRT:
		return  Asset::GetTexture("Assets/Textures/dirt.png");
	case TileType::GRASS:
		return Asset::GetTexture("Assets/Textures/grass.png");
	}
	return {};
}

std::weak_ptr<Texture> TileManager::GetTexture(LogicTileType type) noexcept
{
	switch (type)
	{
	case LogicTileType::NONE:
		return Asset::GetTexture("Assets/Textures/logic_none.png");
	case LogicTileType::SPAWN_UP:
		return  Asset::GetTexture("Assets/Textures/spawn_up.png");
	case LogicTileType::SPAWN_DOWN:
		return  Asset::GetTexture("Assets/Textures/spawn_down.png");
	case LogicTileType::SPAWN_LEFT:
		return Asset::GetTexture("Assets/Textures/spawn_left.png");
	case LogicTileType::SPAWN_RIGHT:
		return Asset::GetTexture("Assets/Textures/spawn_right.png");
	case LogicTileType::CHECKPOINT_UP:
		return  Asset::GetTexture("Assets/Textures/checkpoint_up.png");
	case LogicTileType::CHECKPOINT_DOWN:
		return Asset::GetTexture("Assets/Textures/checkpoint_down.png");
	case LogicTileType::CHECKPOINT_LEFT:
		return Asset::GetTexture("Assets/Textures/checkpoint_left.png");
	case LogicTileType::CHECKPOINT_RIGHT:
		return Asset::GetTexture("Assets/Textures/checkpoint_right.png");
	}
	return {};
}

std::unique_ptr<Tile> TileManager::Get(TileType type) noexcept
{
	auto texture = GetTexture(type);
	switch (type)
	{
	case TileType::NONE:
		return std::move(std::make_unique<Tile>(Tile{ "Empty tile", texture, (unsigned int)type }));
	case TileType::DIRT:
		return std::move(std::make_unique<Tile>(Tile{ "Dirt tile", texture, (unsigned int)type }));
	case TileType::GRASS:
		return std::move(std::make_unique<Tile>(Tile{ "Grass tile", texture, (unsigned int)type }));
	}
	return nullptr;
}

std::unique_ptr<Tile> TileManager::Get(LogicTileType type) noexcept
{
	auto texture = GetTexture(type);
	switch (type)
	{
	case LogicTileType::NONE:
		return std::move(std::make_unique<Tile>(Tile{ "Logic empty tile", texture, (unsigned int)type }));
	case LogicTileType::SPAWN_UP:
		return std::move(std::make_unique<Tile>(Tile{ "Spawn up", texture, (unsigned int)type }));
	case LogicTileType::SPAWN_DOWN:
		return std::move(std::make_unique<Tile>(Tile{ "Spawn down", texture, (unsigned int)type }));
	case LogicTileType::SPAWN_LEFT:
		return std::move(std::make_unique<Tile>(Tile{ "Spawn left", texture, (unsigned int)type }));
	case LogicTileType::SPAWN_RIGHT:
		return std::move(std::make_unique<Tile>(Tile{ "Spawn right", texture, (unsigned int)type }));
	case LogicTileType::CHECKPOINT_UP:
		return std::move(std::make_unique<Tile>(Tile{ "Checkpoint up", texture, (unsigned int)type }));
	case LogicTileType::CHECKPOINT_DOWN:
		return std::move(std::make_unique<Tile>(Tile{ "Checkpoint down", texture, (unsigned int)type }));
	case LogicTileType::CHECKPOINT_LEFT:
		return std::move(std::make_unique<Tile>(Tile{ "Checkpoint left", texture, (unsigned int)type }));
	case LogicTileType::CHECKPOINT_RIGHT:
		return std::move(std::make_unique<Tile>(Tile{ "Checkpoint right", texture, (unsigned int)type }));
	}

	return nullptr;
}

bool TileManager::NotLogicNone(LogicTileType type) noexcept
{
	return type != LogicTileType::NONE;
}

bool TileManager::IsLogicSpawn(LogicTileType type) noexcept
{
	return 
		type == LogicTileType::SPAWN_UP ||
		type == LogicTileType::SPAWN_DOWN ||
		type == LogicTileType::SPAWN_LEFT ||
		type == LogicTileType::SPAWN_RIGHT;
}

bool TileManager::IsCheckPoint(LogicTileType type) noexcept
{
	return
		type == LogicTileType::CHECKPOINT_UP ||
		type == LogicTileType::CHECKPOINT_DOWN ||
		type == LogicTileType::CHECKPOINT_LEFT ||
		type == LogicTileType::CHECKPOINT_RIGHT;
}

glm::vec2 TileManager::GetLogicDir(LogicTileType tile) noexcept
{
	if (tile == LogicTileType::SPAWN_UP || tile == LogicTileType::CHECKPOINT_UP) {
		return { 0, 1 };
	}
	else if (tile == LogicTileType::SPAWN_DOWN || tile == LogicTileType::CHECKPOINT_DOWN) {
		return { 0, -1 };
	}
	else if (tile == LogicTileType::SPAWN_LEFT || tile == LogicTileType::CHECKPOINT_LEFT) {
		return { -1, 0 };
	}
	else if (tile == LogicTileType::SPAWN_RIGHT || tile == LogicTileType::CHECKPOINT_RIGHT) {
		return { 1, 0 };
	}
	else {
		return { 0, 0 };
	}
}


