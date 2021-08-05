#include "pch.h"
#include "EnemyManager.h"

#include "TileType.h"
#include "Asset.h"

void EnemyManager::Load(const std::vector<std::weak_ptr<LogicTile>>& logic_tiles, unsigned int width, unsigned int height) noexcept
{
	mMapWidth = width;
	mMapHeight = height;
	for (unsigned int y = 0; y < height; y++) {
		for (unsigned int x = 0; x < width; x++) {
			const auto& tile = logic_tiles[x + y * width];
			if (TileType::IsLogicSpawn(tile)) {
				mSpawnPoints.push_back({ tile, glm::vec2{x, y} });
			}
			else if (TileType::IsCheckpoint(tile)) {
				mCheckpoints.push_back({ tile, glm::vec2{x, y} });
			}
		}
	}
}

void EnemyManager::Spawn(const Enemy& e) noexcept
{
	mEnemies.push_back(e);
}

void EnemyManager::Update(float delta) noexcept
{
	static float accumulated_time = 0.0f;
	accumulated_time += delta;

	if (accumulated_time > 0.1f) {
		for (const auto& point : mSpawnPoints) {
			auto spawn_dir = TileType::GetLogicSpawnDir(point.tile);
			Spawn(Enemy(point.position.x, point.position.y, spawn_dir.x, spawn_dir.y, mEnemySpeed, Asset::GetTexture("Assets/Textures/ignore_this.png")));
		}
		accumulated_time = 0.0f;
	}

	for (auto it = mEnemies.begin(); it != mEnemies.end(); ++it) {
		it->Update(delta, mMapWidth, mMapHeight, mCheckpoints);
	}
	mEnemies.erase(std::remove_if(mEnemies.begin(), mEnemies.end(),
		[](const auto& e) {
			return !e.IsAlive();
		}), mEnemies.end());

}
