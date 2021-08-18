#pragma once

#include "Tower.h"
#include "TowerType.h"
#include "Asset.h"

class BasicTower : public Tower {
public:
	BasicTower(const glm::vec2& pos) noexcept :
		Tower(pos, 2.0f, 1.0f) 
	{
		TowerTexture tex = TowerTypeTexture::Generate(TowerType::BASIC);
		this->SetBaseTex(tex.base_texture);
		this->SetCannonTex(tex.cannon_texture);
	}

};