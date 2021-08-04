#include "pch.h"
#include "TileType.h"

#include "Asset.h"

std::shared_ptr<Tile<false>> TileType::NONE = nullptr;
std::shared_ptr<Tile<false>> TileType::DIRT = nullptr;
std::shared_ptr<Tile<false>> TileType::GRASS = nullptr;

std::shared_ptr<Tile<true>> TileType::LOGIC_NONE = nullptr;
std::shared_ptr<Tile<true>> TileType::LOGIC_SPAWN = nullptr;

void TileType::Load() noexcept
{
	TileType::NONE = std::make_shared<Tile<false>>(Asset::GetTexture("Assets/Textures/none.png"), 0);
	TileType::DIRT = std::make_shared<Tile<false>>(Asset::GetTexture("Assets/Textures/dirt.png"), 1);
	TileType::GRASS = std::make_shared<Tile<false>>(Asset::GetTexture("Assets/Textures/grass.png"), 2);


	TileType::LOGIC_NONE = std::make_shared<Tile<true>>(Asset::GetTexture("Assets/Textures/logic_none.png"), 0);
	TileType::LOGIC_SPAWN = std::make_shared<Tile<true>>(Asset::GetTexture("Assets/Textures/logic_spawn.png"), 1);
}

std::array<std::weak_ptr<Tile<false>>, TileType::NUM_TYPES> TileType::GetArray() noexcept
{
	return { NONE, DIRT, GRASS };
}

std::array<std::weak_ptr<Tile<true>>, TileType::NUM_LOGIC_TYPES> TileType::GetLogicArray() noexcept
{
	return { LOGIC_NONE, LOGIC_SPAWN};
}
