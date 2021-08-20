#pragma once

#include "Tower.h"
#include "TileMap.h"
#include "TowerType.h"

#include <vector>
#include <memory>
#include <algorithm>
#include <glm/trigonometric.hpp>
#include <glm/gtc/constants.hpp>

class TowerMap {
	inline static const Tower NULL_TOWER = Tower{ TowerType::NONE, glm::vec2(), std::weak_ptr<Texture>(), std::weak_ptr<Texture>(), 0, 0, ProjectileType::NONE, 0 , 0};


	unsigned int mWidth, mHeight;
	std::vector<Tower> mTowers;
public:
	void Load(const TileMap& map) noexcept {
		mWidth = map.GetWidth();
		mHeight = map.GetHeight();

		mTowers.resize(mWidth * mHeight);
		std::generate(mTowers.begin(), mTowers.end(), []() {
			return NULL_TOWER;
			});
	}

	void Update(float delta, const std::vector<Enemy>& enemies, ProjectileManager& projectile_manager) noexcept {
		static constexpr float TARGET_PADDING = 0.5f;
		static constexpr float FLOAT_MAX = std::numeric_limits<float>::max();
		for (auto& tower : mTowers) {


			bool has_enemy = !enemies.empty();
			if (!has_enemy) return;

			glm::vec2 direction;
			float length_min_squared = FLOAT_MAX;
			std::for_each(enemies.begin(), enemies.end(),
				[&](const Enemy& e) {
					float x = e.GetX()  - tower.position.x;
					float y = e.GetY()  - tower.position.y;
					float x2 = x * x;
					float y2 = y * y;
					float length_squared = x2 + y2;
					if (length_squared < length_min_squared) {
						length_min_squared = length_squared;
						direction = { x, y };
					}
				});
			if (tower.target_locked = has_enemy && length_min_squared < tower.range * tower.range) {
				//Lock into target
				float real_length = glm::sqrt(length_min_squared);
				direction.x /= real_length;
				direction.y /= real_length;
				tower.cannon_rotation = glm::degrees(glm::atan(direction.y, direction.x) - glm::pi<float>() / 2.0f);
				tower.accumulated_time += delta;

				if (tower.accumulated_time >= tower.firing_delay) {
					projectile_manager.Spawn(tower.projectile_type, tower.position, direction, tower.projectile_speed, tower.damage);
					tower.accumulated_time = 0.0f;
				}
			}
		}
	}

	void PlaceTower(unsigned int x, unsigned int y, TowerType type) noexcept {
		if (x < 0 || y < 0 || x >= mWidth || y >= mHeight) return;

		Tower& tower = mTowers[x + y * mWidth];
		if (tower.type == TowerType::NONE)
			tower = TowerTypeInstance::Generate(type, { x, y });
	}

	void RemoveTower(unsigned int x, unsigned int y) noexcept {
		mTowers[x + y * mWidth] = NULL_TOWER;
	}

	inline const std::vector<Tower>& GetTowers() const noexcept { return mTowers; }
};