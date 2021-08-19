#pragma once

#include "Tile.h"
#include "LogicTileTypeEnum.h"

#include <memory>
#include <vector>

class Camera;
class TileMap {

	using TilePtr = std::unique_ptr<Tile>;
	using TileArray = std::vector<TilePtr>;
	using LogicTileArray = std::vector<TilePtr>;
	
	unsigned int mWidth, mHeight;
	TileArray mTiles;
	LogicTileArray mLogicTiles;
public:

	void LoadNew(unsigned int width, unsigned int height, TileType tile, LogicTileType logic_tile) noexcept;
	void Load(unsigned int width, unsigned int height, TileArray& tiles, LogicTileArray& logic_tiles) noexcept;

	void PlaceTile(unsigned int x, unsigned int y, TileType tile);
	void PlaceLogicTile(unsigned int x, unsigned int y, LogicTileType tile);

	inline const TileArray& GetTiles() const noexcept { return mTiles; }
	inline const TilePtr& GetTile(unsigned int x, unsigned int y) const noexcept { return mTiles[x + y * mWidth]; }
	inline const LogicTileArray& GetLogicTiles() const noexcept { return mLogicTiles; }
	inline const TilePtr& GetLogicTile(unsigned int x, unsigned int y) const noexcept { return mLogicTiles[x + y * mWidth]; }
	inline const unsigned int& GetWidth() const noexcept { return mWidth; }
	inline const unsigned int& GetHeight() const noexcept { return mHeight; }
};