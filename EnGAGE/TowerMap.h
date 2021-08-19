#pragma once

#include "Tower.h"
#include "TileMap.h"
#include "TowerType.h"

#include <vector>
#include <memory>
#include <algorithm>

class TowerMap {
	unsigned int mWidth, mHeight;
	std::vector<std::unique_ptr<Tower>> mTowers;
public:
	void Load(const TileMap& map) noexcept {
		mWidth = map.GetWidth();
		mHeight = map.GetHeight();

		mTowers.resize(mWidth * mHeight);
		std::generate(mTowers.begin(), mTowers.end(), []() {
			return nullptr;
		});
	}

	void Update(float delta, const std::vector<Enemy>& enemies) noexcept {
		std::for_each(mTowers.begin(), mTowers.end(), [&](const std::unique_ptr<Tower>& tower) {
			if(tower)
				tower->Update(enemies, delta);
		});
	}

	void PlaceTower(unsigned int x, unsigned int y, TowerType type) noexcept {
		if (x < 0 || y < 0 || x >= mWidth || y >= mHeight) return;
		std::unique_ptr<Tower> tower_ptr = nullptr;
		TowerTypeInstance::Generate(type, tower_ptr, { x, y });
		mTowers[x + y * mWidth] = std::move(tower_ptr);
	}

	void RemoveTower(unsigned int x, unsigned int y) noexcept {
		mTowers[x + y * mWidth] = nullptr;
	}

	inline const std::vector<std::unique_ptr<Tower>>& GetTowers() const noexcept { return mTowers; }
};