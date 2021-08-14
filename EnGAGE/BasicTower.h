#pragma once

#include "Tower.h"
#include "Asset.h"

class BasicTower : public Tower{
public:
	BasicTower(const glm::vec2& pos) noexcept :
		Tower(pos, Asset::GetTexture("Assets/Textures/tower_generic_base.png"),
			Asset::GetTexture("Assets/Textures/tower_generic_cannon.png"), 2.0f, 1.0f) {}

	void 
};