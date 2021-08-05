#pragma once

#include "Tile.h"
#include "LogicTile.h"

#include <memory>
#include <vector>

class Player;
class TileMap {

	using TilePtr = std::weak_ptr<Tile>;
	using LogicTilePtr = std::weak_ptr<LogicTile>;
	using TileArray = std::vector<TilePtr>;
	using LogicTileArray = std::vector<LogicTilePtr>;
	
	unsigned int mWidth, mHeight;
	TileArray mTiles = {};
	LogicTileArray mLogicTiles = {};
public:

	void LoadNew(unsigned int width, unsigned int height, const TilePtr& tile, const LogicTilePtr& logic_tile) noexcept;
	void Load(const std::string& file_name) noexcept;
	void Write(const std::string& file_name) noexcept;

	void PlaceTile(unsigned int x, unsigned int y, const TilePtr& tile);
	void PlaceLogicTile(unsigned int x, unsigned int y, const LogicTilePtr& tile);

	inline const TileArray& GetTiles() const noexcept { return mTiles; }
	inline const TilePtr& GetTile(unsigned int x, unsigned int y) const noexcept { return mTiles[x + y * mWidth]; }
	inline const LogicTileArray& GetLogicTiles() const noexcept { return mLogicTiles; }
	inline const LogicTilePtr& GetLogicTile(unsigned int x, unsigned int y) const noexcept { return mLogicTiles[x + y * mWidth]; }
	inline const unsigned int& GetWidth() const noexcept { return mWidth; }
	inline const unsigned int& GetHeight() const noexcept { return mHeight; }
};