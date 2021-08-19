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


void TileMap::LoadNew(unsigned int width, unsigned int height, TileType tile, LogicTileType logic_tile) noexcept
{
	this->mTiles.clear();
	this->mWidth = width;
	this->mHeight = height;

	size_t capacity = (size_t)mWidth * (size_t)mHeight;
	for (unsigned int i = 0; i < mWidth * mHeight; i++) {
		mTiles.push_back(TileManager::Get(tile));
		mLogicTiles.push_back(TileManager::Get(logic_tile));
	}
}

void TileMap::Load(unsigned int width, unsigned int height, TileArray& tiles, LogicTileArray& logic_tiles) noexcept
{
	this->mTiles.clear();
	this->mLogicTiles.clear();

	this->mWidth = width;
	this->mHeight = height;
	this->mTiles = std::move(tiles);
	this->mLogicTiles = std::move(logic_tiles);
}

void TileMap::PlaceTile(unsigned int x, unsigned  int y, TileType tile)
{
	if (x < 0 || y < 0 || x >= mWidth || y >= mHeight) return;
	mTiles[x + y * mWidth].reset(nullptr);
	mTiles[x + y * mWidth] = std::move(TileManager::Get(tile));
}

void TileMap::PlaceLogicTile(unsigned int x, unsigned int y, LogicTileType tile)
{
	if (x < 0 || y < 0 || x >= mWidth || y >= mHeight) return;
	mLogicTiles[x + y * mWidth].reset(nullptr);
	mLogicTiles[x + y * mWidth] = std::move(TileManager::Get(tile));
}


