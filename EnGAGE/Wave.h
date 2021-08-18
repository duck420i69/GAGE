#pragma once

#include <vector>
#include "EnemyType.h"

class Wave {
	std::vector<EnemyType> mEnemies = {};
	float mEnemySpeed = 1.0f;
	float mEnemySpawnDelay = 0.5f; //Default to 500 ms 
public:

	void AddEnemy(EnemyType e) noexcept {
		mEnemies.push_back(e);
	}

	void RemoveEnemy(size_t index) noexcept {
		mEnemies.erase(mEnemies.begin() + index);
	}

	inline void SetEnemySpawnDelay(float delay) noexcept { mEnemySpawnDelay = delay; }
	inline void SetEnemySpeed(float speed) noexcept { mEnemySpeed = speed; }

	inline const float& GetEnemySpeed() const noexcept { return mEnemySpeed; }
	inline float& GetEnemySpeed() noexcept { return mEnemySpeed; }
	inline const float& GetEnemySpawnDelay() const noexcept { return mEnemySpawnDelay; }
	inline float& GetEnemySpawnDelay() noexcept { return mEnemySpawnDelay; }
	inline const std::vector<EnemyType>& GetEnemies() const noexcept { return mEnemies; }
	inline std::vector<EnemyType>& GetEnemies() noexcept { return mEnemies; }
};