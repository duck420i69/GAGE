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

TileMap::TileMap() noexcept :
	mShader(Asset::GetShader("Assets/Shaders/tile"))
{
	LoadGPUData();
}

TileMap::~TileMap() noexcept
{
	Logger::info("Deleting tilemap...");
	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBO);
	mVAO = 0;
	mVBO = 0;
}

void TileMap::LoadNew(unsigned int width, unsigned int height, const TilePtr& tile, const LogicTilePtr& logic_tile) noexcept
{
	this->mTiles.clear();
	this->mLogicTiles.clear();
	this->mWidth = width;
	this->mHeight = height;
	
	mTiles.reserve((size_t)mWidth * (size_t)mHeight);
	mLogicTiles.reserve(mTiles.capacity());
	for (unsigned int i = 0; i < mWidth * mHeight; i++) {
		mTiles.push_back(tile);
		mLogicTiles.push_back(logic_tile);
	}
}

void TileMap::Load(unsigned int width, unsigned int height, const TileArray& tiles, const LogicTileArray& logic_tiles) noexcept
{
	this->mTiles.clear();
	this->mWidth = width;
	this->mHeight = height;
	this->mTiles = tiles;
	this->mLogicTiles = logic_tiles;
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
		const auto& logicTiles = GetLogicTiles();
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
				auto current_tile = logicTiles[x + y * width].lock();
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
		std::vector<std::weak_ptr<Tile<false>>> tiles;
		std::vector<std::weak_ptr<Tile<true>>> logicTiles;
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
				logicTiles.push_back(logic_tile_array[value]);
			}
		}
		Load(width, height, tiles, logicTiles);
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

void TileMap::Update(float delta, const Player& player) noexcept
{
	//Update matrices
	float aspectRatio = (float)Window::GetWidth() / (float)Window::GetHeight();
	float zoom = player.GetZoom();
	auto pos = player.GetPos();
	mProjection = glm::ortho(-zoom * aspectRatio, zoom * aspectRatio, -zoom, zoom);
	mView = glm::translate(glm::mat4(1.0f), glm::vec3(-pos, 0));
}

void TileMap::Render() const noexcept
{
	//Render all tiles and logic tiles
	auto shader = mShader.lock();
	shader->Bind();
	glBindVertexArray(mVAO);

	shader->UploadMat4x4("uProjection", glm::value_ptr(mProjection));
	shader->UploadMat4x4("uView", glm::value_ptr(mView));

	glm::mat4 model;
	for (unsigned int y = 0; y < mHeight; y++) {
		for (unsigned int x = 0; x < mWidth; x++) {
			model = glm::translate(glm::mat4(1.0f), { x, y, 0 });
			shader->UploadMat4x4("uModel", glm::value_ptr(model));
			mTiles[x + y * mWidth].lock()->GetTexture().lock()->Bind();
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
	}
	glBindVertexArray(0);
	glUseProgram(0);
}

void TileMap::RenderEdit() const noexcept
{
	//Render all tiles and logic tiles
	auto shader = mShader.lock();
	shader->Bind();
	glBindVertexArray(mVAO);

	shader->UploadMat4x4("uProjection", glm::value_ptr(mProjection));
	shader->UploadMat4x4("uView", glm::value_ptr(mView));

	glm::mat4 model;
	for (unsigned int y = 0; y < mHeight; y++) {
		for (unsigned int x = 0; x < mWidth; x++) {
			model = glm::translate(glm::mat4(1.0f), { x, y, 0 });
			shader->UploadMat4x4("uModel", glm::value_ptr(model));
			mTiles[x + y * mWidth].lock()->GetTexture().lock()->Bind();
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mLogicTiles[x + y * mWidth].lock()->GetTexture().lock()->Bind();
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
	}
	glBindVertexArray(0);
	glUseProgram(0);
}

void TileMap::LoadGPUData() noexcept
{
	const float vertex_data[] = {
		0, 1, 0, 1,
		0, 0, 0, 0,
		1, 1, 1, 1,
		1, 1, 1, 1,
		0, 0, 0, 0,
		1, 0, 1, 0
	};


	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);

	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, vertex_data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (const void*)(sizeof(float) * 2));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	Logger::info("Loading tilemap's gpu data...");
}


