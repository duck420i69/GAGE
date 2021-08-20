#pragma once

#include "Texture.h"
#include "Projectile.h"
#include "TowerTypeEnum.h"

#include <glm/vec2.hpp>
#include <memory>

struct Tower {
	TowerType type;

	glm::vec2 position;
	std::weak_ptr<Texture> base_texture;
	std::weak_ptr<Texture> cannon_texture;
	float range;
	float firing_delay;

	ProjectileType projectile_type;
	float projectile_speed;
	float damage;

	float cannon_rotation = 0.0f;
	float accumulated_time = 0.0f;
	bool target_locked = false;
};

/*inline void Update(const std::vector<Enemy>& enemies, float delta, ProjectileManager& projectile_manager) noexcept {

	static constexpr float FLOAT_MAX = std::numeric_limits<float>::max();

	bool has_enemy = !enemies.empty();
	if (!has_enemy) return;

	glm::vec2 direction;
	float length_min_squared = FLOAT_MAX;
	std::for_each(enemies.begin(), enemies.end(),
		[&](const Enemy& e) {
			float x = e.GetX() - this->mPosition.x;
			float y = e.GetY() - this->mPosition.y;
			float x2 = x * x;
			float y2 = y * y;
			float length_squared = x2 + y2;
			if (length_squared < length_min_squared) {
				length_min_squared = length_squared;
				direction = { x, y };
			}
		});
	if (mTargetLocked = has_enemy && length_min_squared < mRange * mRange) {
		//Lock into target
		float real_length = glm::sqrt(length_min_squared);
		direction.x /= real_length;
		direction.y /= real_length;
		mCannonRotation = glm::degrees(glm::atan(direction.y, direction.x) - glm::pi<float>() / 2.0f);
		mAccumulatedTime += delta;

		if (mAccumulatedTime >= mFiringDelay) {
			projectile_manager.Spawn(mProjectileType, mPosition, direction, mProjectileSpeed);
			mAccumulatedTime = 0.0f;
		}
	}

}*/