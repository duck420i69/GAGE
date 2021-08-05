#include "pch.h"
#include "TileMap.h"

#include "Window.h"
#include "Events.h"
#include "Logger.h"
#include "TileType.h"
#include "Player.h"

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


void TileMap::Write(const std::string& file_name) noexcept
{
	try {
		std::ofstream file;
		file.exceptions(std::ofstream::badbit | std::ofstream::failbit);
		file.open(file_name);
		unsigned int width = GetWidth();
		unsigned int height = GetHeight();
		const auto& tiles = GetTiles();
		const auto& logic_tiles = GetLogicTiles();
		file << width << "\n";
		file << height << "\n";
		for (unsigned int y = 0; y < GetHeight(); y++) {
			for (unsigned int x = 0; x < GetWidth(); x++) {
				auto current_tile = tiles[x + y * width].lock();
				file << current_tile->GetID() << " ";
			}
			file << "\n";
		}
		file << "\n";
		for (unsigned int y = 0; y < GetHeight(); y++) {
			for (unsigned int x = 0; x < GetWidth(); x++) {
				auto current_tile = logic_tiles[x + y * width].lock();
				file << current_tile->GetID() << " ";
			}
			file << "\n";
		}
	
		file.close();
	}
	catch (std::ios_base::failure& e) {
		Logger::error("Editor failed to write save file: {}, ios error: ", file_name, e.what());
	}
}

void TileMap::Load(const std::string& file_name) noexcept
{
	try {
		std::ifstream file;
		file.exceptions(std::ifstream::badbit | std::ifstream::failbit);
		file.open(file_name);
		unsigned int width, height;
		TileArray tiles;
		LogicTileArray logic_tiles;
		auto tile_array = TileType::GetArray();
		auto logic_tile_array = TileType::GetLogicArray();
		file >> width;
		file >> height;
		for (unsigned int y = 0; y < height; y++) {
			for (unsigned int x = 0; x < width; x++) {
				unsigned int value;
				file >> value;
				tiles.push_back(tile_array[value]);
			}
		}

		for (unsigned int y = 0; y < height; y++) {
			for (unsigned int x = 0; x < width; x++) {
				unsigned int value;
				file >> value;
				logic_tiles.push_back(logic_tile_array[value]);
			}
		}

		this->mTiles.clear();
		this->mWidth = width;
		this->mHeight = height;
		this->mTiles = tiles;
		this->mLogicTiles = logic_tiles;
	}
	catch (std::ios_base::failure& e) {
		Logger::error("Editor failed to load save file: {}, ios error: {}", file_name, e.what());
	}
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


