#pragma once

#include "Texture.h"
#include "Point.h"
#include "TileType.h"

#include <memory>

class Enemy
{
	float mX, mY;
	float mVX = 0, mVY = 0;
	float mSpeed;
	std::weak_ptr<Texture> mTexture;
	bool mAlive = true;
public:
	Enemy(float x, float y, float vx, float vy, float speed, const std::weak_ptr<Texture>& texture) noexcept :
		mX(x), mY(y),mVX(vx), mVY(vy), mSpeed(speed), mTexture(texture) {}
	~Enemy() noexcept = default;

	inline void Update(float delta, unsigned int map_width, unsigned int map_height, const std::vector<Point>& check_points) noexcept {		
		mX += mVX * mSpeed * delta;
		mY += mVY * mSpeed * delta;

		for (const auto& point : check_points) {
			float x = point.position.x;
			float y = point.position.y;

			//Check if near, not absolute
			const float epsilon = 0.1f;
			if (std::abs(mX - x) < epsilon && std::abs(mY - y) < epsilon) {
				auto dir = TileType::GetCheckpointDir(point.tile);
				mVX = dir.x;
				mVY = dir.y;
			}
		}

		if (mX < -1 || mY < -1 || mX >= (map_width + 1) || mY >= (map_height + 1)) {
			mAlive = false;
		}
	}

	inline void SetVX(float vx) noexcept { mVX = vx; }
	inline void SetVY(float vy) noexcept { mVY = vy; }

	inline const float& GetX() const noexcept { return mX; }
	inline const float& GetY() const noexcept { return mY; }
	inline const std::weak_ptr<Texture>& GetTexture() const noexcept { return mTexture; }
	inline const bool& IsAlive() const noexcept { return mAlive; }
};