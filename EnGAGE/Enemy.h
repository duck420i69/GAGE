#pragma once

#include "Texture.h"
#include "EnemyType.h"
#include "CheckPoint.h"

#include <memory>
#include <glm/vec2.hpp>

class Enemy 
{
	EnemyType mType;
	glm::vec2 mPos = {0, 0}, mVel = { 0, 0 };
	std::weak_ptr<Texture> mTexture;
	std::vector<CheckPoint> mCheckPoints;
	bool mAlive = true;
public:
	Enemy(EnemyType type, const CheckPoint& spawn_point, const std::vector<CheckPoint>& checkpoints) noexcept :
		mType(type), 
		mCheckPoints(checkpoints)
	{
		mTexture = EnemyTypeTexture::Get(type);
		mPos = spawn_point.pos;
		mVel = TileType::GetLogicDir(spawn_point.tile);
	}

	inline void Update(float delta,  unsigned int map_width, unsigned int map_height, const float& speed) noexcept {
		mPos.x += mVel.x * delta * speed;
		mPos.y += mVel.y * delta * speed;

		constexpr float EPSILON = 0.25f;

		auto it = mCheckPoints.begin();
		while (it != mCheckPoints.end()) {
			if (std::abs(it->pos.x - mPos.x) < EPSILON && std::abs(it->pos.y - mPos.y) < EPSILON) {
				mVel = TileType::GetLogicDir(it->tile);
				mPos = it->pos;
				it = mCheckPoints.erase(it);
			}
			else {
				it++;
			}
			
		}

		if (mPos.x < -1 || mPos.y < -1 || mPos.x >(map_width + 1) || mPos.y >(map_height + 1)) {
			mAlive = false;
		}
	}

	inline const float& GetX() const noexcept { return mPos.x; }
	inline const float& GetY() const noexcept { return mPos.y; }
	inline const bool& Alive() const noexcept { return mAlive; }
	inline const std::weak_ptr<Texture>& GetTexture() const noexcept { return mTexture; }
};