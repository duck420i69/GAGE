#include "pch.h"
#include "WaveManager.h"

#include "TileType.h"



void WaveManager::Load(const TileMap& map) noexcept
{
	for (unsigned int y = 0; y < map.GetHeight(); y++) {
		for (unsigned int x = 0; x < map.GetWidth(); x++) {
			const auto& logic_tile = map.GetLogicTiles()[x + y * map.GetWidth()];
			if (TileType::IsLogicSpawn(logic_tile)) {
				mSpawnPoints.push_back(CheckPoint{ glm::vec2{x, y }, logic_tile });
			}
			else if (TileType::IsCheckpoint(logic_tile)) {
				mCheckPoints.push_back(CheckPoint{ glm::vec2{x, y }, logic_tile });
			}
		}
	}
}

void WaveManager::Update(float delta, const TileMap& map) noexcept
{
	if (mRound >= mWaves.size()) {
		mGameOver = true;
		return;
	}
	const Wave* wave = &mWaves[mRound];
	if (mRoundStart && !mWaveFull) {
		mAccumulatedTime += delta;
		if (mAccumulatedTime > wave->GetEnemySpawnDelay()) {
			//Spawn enemy
			for (const auto& spawn_point : mSpawnPoints) {
				mEnemies.emplace_back(wave->GetEnemies()[mEnemyIndex], spawn_point, mCheckPoints);
			}
			mAccumulatedTime = 0.0f;
			mEnemyIndex++;
		}

		if (mEnemyIndex >= wave->GetEnemies().size()) {
			mWaveFull = true;
		}
	}

	auto it = mEnemies.begin();
	while (it != mEnemies.end()) {
		//Remove if dead and reset all flags
		if (!it->Alive()) {
			it = mEnemies.erase(it);

			if (mEnemies.empty()) {
				mWaveFull = false;
				mRoundStart = false;
				mEnemyIndex = 0;
				mAccumulatedTime = 0.0f;
				mRound++;
			}
		}
		else {
			//Update all enemy's positions
			it->Update(delta, map.GetWidth(), map.GetHeight(), wave->GetEnemySpeed());
			it++;
		}
	}
}
