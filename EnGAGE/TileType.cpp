#include "pch.h"
#include "TileType.h"

#include "Asset.h"

std::shared_ptr<Tile> TileType::NONE = nullptr;
std::shared_ptr<Tile> TileType::DIRT = nullptr;
std::shared_ptr<Tile> TileType::GRASS = nullptr;

void TileType::Load() noexcept
{
	TileType::NONE = std::make_shared<Tile>(Asset::GetTexture("Assets/Textures/none.png"), 0);
	TileType::DIRT = std::make_shared<Tile>(Asset::GetTexture("Assets/Textures/dirt.png"), 1);
	TileType::GRASS = std::make_shared<Tile>(Asset::GetTexture("Assets/Textures/grass.png"), 2);
}

std::array<std::weak_ptr<Tile>, TileType::NUM_TYPES> TileType::GetArray() noexcept
{
	return { NONE, DIRT, GRASS };
}
