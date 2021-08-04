#pragma once

#include "Tile.h"
#include "Asset.h"


#include <glm/mat4x4.hpp>
#include <memory>
#include <vector>

class Player;
class TileMap {

	using TilePtr = std::weak_ptr<Tile<false>>;
	using LogicTilePtr = std::weak_ptr<Tile<true>>;
	using TileArray = std::vector<TilePtr>;
	using LogicTileArray = std::vector<LogicTilePtr>;

	unsigned int mVAO = 0, mVBO = 0;
	std::weak_ptr<Shader> mShader;

	glm::mat4	mProjection = glm::mat4(1.0f);
	glm::mat4	mView = glm::mat4(1.0f);

	unsigned int mWidth, mHeight;
	TileArray mTiles = {};
	LogicTileArray mLogicTiles = {};
public:
	TileMap() noexcept;
	~TileMap() noexcept;

	void LoadNew(unsigned int width, unsigned int height, const TilePtr& tile, const LogicTilePtr& logic_tile) noexcept;
	void Load(unsigned int width, unsigned int height, const TileArray& tiles, const LogicTileArray& logic_tiles) noexcept;
	void Load(const std::string& file_name) noexcept;
	void Write(const std::string& file_name) noexcept;

	void PlaceTile(unsigned int x, unsigned int y, const TilePtr& tile);
	void PlaceLogicTile(unsigned int x, unsigned int y, const LogicTilePtr& tile);
	void Update(float delta, const Player& player) noexcept;
	void Render() const noexcept;
	void RenderEdit() const noexcept;

	inline const TileArray& GetTiles() const noexcept { return mTiles; }
	inline const LogicTileArray& GetLogicTiles() const noexcept { return mLogicTiles; }
	inline const unsigned int& GetWidth() const noexcept { return mWidth; }
	inline const unsigned int& GetHeight() const noexcept { return mHeight; }

private:
	void LoadGPUData() noexcept;
};