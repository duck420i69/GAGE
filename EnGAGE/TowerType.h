#pragma once

#include "TowerTexture.h"
#include "Tower.h"
#include <memory>

enum class TowerType {
	BASIC,
	COUNT
};


namespace TowerTypeTexture {
	TowerTexture Generate(TowerType e);
}

namespace TowerTypeInstance {
	void Generate(TowerType e, std::unique_ptr<Tower>& instance, const glm::vec2& pos);
}