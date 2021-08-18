#pragma once

#include "Texture.h"
#include "Enemy.h"
#include "Logger.h"

#include <glm/vec2.hpp>
#include <glm/gtx/norm.hpp>
#include <memory>
#include <chrono>

class Tower {
	using TexturePtr = std::weak_ptr<Texture>;

	glm::vec2 mPosition;
	TexturePtr mBaseTexture;
	TexturePtr mCannonTexture;
	float mRange;
	float mCannonRotation = 0;

	float mFiringDelay;
	float mAccumulatedTime = 0.0f;
protected:
	bool mTargetLocked;
public:
	Tower(const glm::vec2& pos, float range, float firing_delay) noexcept :
		mPosition(pos),
		mRange(range),
		mFiringDelay(firing_delay)
	{}

	inline void Update(const std::vector<Enemy>& enemies, float delta) noexcept {

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
				Logger::info("yo i shot myself");
				mAccumulatedTime = 0.0f;
			}
		}

	}
	inline void SetRotation(float rotation) noexcept { mCannonRotation = rotation; }
	inline void SetRange(float range) noexcept { mRange = range; }

	inline const glm::vec2& GetPos() const noexcept { return mPosition; }
	inline const TexturePtr& GetBaseTexture() const noexcept { return mBaseTexture; }
	inline const TexturePtr& GetCannonTexture() const noexcept { return mCannonTexture; }
	inline const float& GetRotation() const noexcept { return mCannonRotation; }
	inline const float& GetRange() const noexcept { return mRange; }
protected:
	inline void SetBaseTex(const TexturePtr& tex) noexcept { mBaseTexture = tex; }
	inline void SetCannonTex(const TexturePtr& tex) noexcept { mCannonTexture = tex; }
};