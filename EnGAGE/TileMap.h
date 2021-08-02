#pragma once

#include "Tile.h"
#include "Asset.h"


#include <glm/mat4x4.hpp>
#include <memory>
#include <vector>

class Player;
class TileMap {

	unsigned int mVAO = 0, mVBO = 0;
	std::weak_ptr<Shader> mShader;

	glm::mat4	mProjection = glm::mat4(1.0f);
	glm::mat4	mView = glm::mat4(1.0f);

	unsigned int mWidth, mHeight;
	std::vector<std::weak_ptr<Tile>> mTiles = {};
public:
	TileMap() noexcept;
	~TileMap() noexcept;

	void LoadNew(unsigned int width, unsigned int height) noexcept;
	void Load(unsigned int width, unsigned int height, const std::vector<std::weak_ptr<Tile>>& tiles) noexcept;

	void PlaceTile(int x, int y, const std::weak_ptr<Tile>& tile);
	void Update(float delta, const Player& player) noexcept;
	void Render() const noexcept;

	inline const std::vector<std::weak_ptr<Tile>>& GetTiles() const noexcept { return mTiles; }
	inline const unsigned int& GetWidth() const noexcept { return mWidth; }
	inline const unsigned int& GetHeight() const noexcept { return mHeight; }
private:
	void LoadGPUData() noexcept;
};