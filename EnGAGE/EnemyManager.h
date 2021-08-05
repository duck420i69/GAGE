#pragma once

#include <vector>
#include <memory>
#include <glm/vec2.hpp>

#include "Enemy.h"
#include "LogicTile.h"

#include "Point.h"

class EnemyManager {

	

	std::vector<Point> mSpawnPoints;
	std::vector<Point> mCheckpoints;
	
	float	mEnemySpeed = 10.0f;
	std::vector<Enemy> mEnemies;
	unsigned int mMapWidth, mMapHeight;
public:
	void Load(const std::vector<std::weak_ptr<LogicTile>>& logic_tiles, unsigned int width, unsigned int height) noexcept;
	void Spawn(const Enemy& e) noexcept;
	void Update(float delta) noexcept;

	inline const std::vector<Enemy>& GetEnemies() const noexcept { return mEnemies; }
};