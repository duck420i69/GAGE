#include "pch.h"
#include "TileMap.h"

#include "Window.h"
#include "Events.h"
#include "Logger.h"
#include "TileType.h"
#include "Camera.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>


void TileMap::LoadNew(unsigned int width, unsigned int height, const TilePtr& tile, const LogicTilePtr& logic_tile) noexcept
{
	this->mTiles.clear();
	this->mWidth = width;
	this->mHeight = height;

	size_t capacity = (size_t)mWidth * (size_t)mHeight;
	mTiles.reserve(capacity);
	mLogicTiles.reserve(capacity);
	for (unsigned int i = 0; i < mWidth * mHeight; i++) {
		mTiles.push_back(tile);
		mLogicTiles.push_back(logic_tile);
	}
}

void TileMap::Load(unsigned int width, unsigned int height, const TileArray& tiles, const LogicTileArray& logic_tiles) noexcept
{
	this->mTiles.clear();
	this->mLogicTiles.clear();

	this->mWidth = width;
	this->mHeight = height;
	this->mTiles = tiles;
	this->mLogicTiles = logic_tiles;
}

void TileMap::PlaceTile(unsigned int x, unsigned  int y, const TilePtr& tile)
{
	if (x < 0 || y < 0 || x >= mWidth || y >= mHeight) return;

	mTiles[x + y * mWidth] = tile;
}

void TileMap::PlaceLogicTile(unsigned int x, unsigned int y, const LogicTilePtr& tile)
{
	if (x < 0 || y < 0 || x >= mWidth || y >= mHeight) return;

	mLogicTiles[x + y * mWidth] = tile;
}


