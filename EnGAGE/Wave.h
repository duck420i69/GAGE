#pragma once

#include <vector>
#include "EnemyType.h"

class Wave {
	std::vector<EnemyType> mEnemies;
public:

	void AddEnemy(EnemyType e) noexcept {
		mEnemies.push_back(e);
	}

	void RemoveEnemy(size_t index) noexcept {
		mEnemies.erase(mEnemies.begin() + index);
	}

	inline const std::vector<EnemyType>& GetEnemies() const noexcept { return mEnemies; };
};