#pragma once

#include "Wave.h"
#include "Enemy.h"
#include "TileMap.h"
#include "CheckPoint.h"

#include <vector>

class WaveManager {
	std::vector<CheckPoint> mSpawnPoints = {};
	std::vector<CheckPoint> mCheckPoints = {};

	std::vector<Wave> mWaves = {};
	std::vector<Enemy> mEnemies = {};
	unsigned int mRound = 0;
	bool mRoundStart = false;

	float mAccumulatedTime = 0.0f;
	unsigned int mEnemyIndex = 0;
	bool mWaveFull = false;
	bool mGameOver = false;
	unsigned int mCurrentWaveEnemieCount = 0;
public:
	void Load(const TileMap& map) noexcept;
	void Update(float delta, const TileMap& map) noexcept;


	inline const bool& IsGameOver() const noexcept { return mGameOver; }
	inline void StartRound() noexcept { mRoundStart = true; }
	inline const unsigned int& GetRound() const noexcept { return mRound; }
	inline const std::vector<Enemy>& GetEnemies() const noexcept { return mEnemies; }
	inline std::vector<Enemy>& GetEnemies() noexcept { return mEnemies; }
	inline const std::vector<Wave>& GetWaves() const noexcept { return mWaves; }
	inline std::vector<Wave>& GetWaves() noexcept { return mWaves; }

	inline void SetWaves(const std::vector<Wave>& waves) { mWaves = std::move(waves); }
};