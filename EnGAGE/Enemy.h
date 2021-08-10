#pragma once

#include "Texture.h"
#include "EnemyType.h"

#include <memory>
#include <glm/vec2.hpp>

class Enemy 
{
	EnemyType mType;
	glm::vec2 mPos = {0, 0}, mVel = { 0, 0 };
	std::weak_ptr<Texture> mTexture;
public:
	Enemy(EnemyType type, const std::weak_ptr<Texture>& texture) noexcept :
		mType(type), mTexture(texture) {}

	inline const float& GetX() const noexcept { return mPos.x; }
	inline const float& GetY() const noexcept { return mPos.y; }
	inline const std::weak_ptr<Texture>& GetTexture() const noexcept { return mTexture; }
};