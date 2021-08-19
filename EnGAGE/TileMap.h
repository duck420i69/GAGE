#pragma once

#include "Tile.h"
#include "LogicTileTypeEnum.h"

#include <memory>
#include <vector>

class Camera;
class TileMap {
	
	unsigned int mWidth, mHeight;
	std::vector<Tile> mTiles;
	std::vector<Tile> mLogicTiles;
public:

	void LoadNew(unsigned int width, unsigned int height, TileType tile, LogicTileType logic_tile) noexcept;
	void Load(unsigned int width, unsigned int height, std::vector<Tile>& tiles, std::vector<Tile>& logic_tiles) noexcept;

	void PlaceTile(unsigned int x, unsigned int y, TileType tile);
	void PlaceLogicTile(unsigned int x, unsigned int y, LogicTileType tile);

	inline const std::vector<Tile>& GetTiles() const noexcept { return mTiles; }
	inline const Tile& GetTile(unsigned int x, unsigned int y) const noexcept { return mTiles[x + y * mWidth]; }
	inline const std::vector<Tile>& GetLogicTiles() const noexcept { return mLogicTiles; }
	inline const Tile& GetLogicTile(unsigned int x, unsigned int y) const noexcept { return mLogicTiles[x + y * mWidth]; }
	inline const unsigned int& GetWidth() const noexcept { return mWidth; }
	inline const unsigned int& GetHeight() const noexcept { return mHeight; }
};