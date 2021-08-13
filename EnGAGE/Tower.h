#pragma once

#include "Texture.h"
#include "Enemy.h"
#include "Logger.h"

#include <glm/vec2.hpp>
#include <glm/gtx/norm.hpp>
#include <memory>

class Tower {
	glm::vec2 mPosition;
	std::weak_ptr<Texture> mBaseTexture;
	std::weak_ptr<Texture> mCannonTexture;
	float mCannonRotation;

	float mRange = 2.0f;
public:
	Tower(const glm::vec2& pos, const std::weak_ptr<Texture>& base_texture,
		const std::weak_ptr<Texture>& cannon_texture, float cannon_rotation) noexcept :
		mPosition(pos),
		mBaseTexture(base_texture),
		mCannonTexture(cannon_texture),
		mCannonRotation(cannon_rotation)
	{}

	inline void Update(const std::vector<Enemy>& enemies) noexcept {
		static constexpr float FLOAT_MAX = std::numeric_limits<float>::max();

		bool has_enemy = !enemies.empty();
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

		if (has_enemy && length_min_squared < mRange * mRange) {
			//Lock into target
			float real_length = glm::sqrt(length_min_squared);
			direction.x /= real_length;
			direction.y /= real_length;
			mCannonRotation = glm::degrees(glm::atan(direction.y, direction.x) - glm::pi<float>() / 2.0f);
		}
	}

	inline void SetRotation(float rotation) noexcept { mCannonRotation = rotation; }
	inline void SetRange(float range) noexcept { mRange = range; }

	inline const glm::vec2& GetPos() const noexcept { return mPosition; }
	inline const std::weak_ptr<Texture>& GetBaseTexture() const noexcept { return mBaseTexture; }
	inline const std::weak_ptr<Texture>& GetCannonTexture() const noexcept { return mCannonTexture; }
	inline const float& GetRotation() const noexcept { return mCannonRotation; }
	inline const float& GetRange() const noexcept { return mRange; }
};