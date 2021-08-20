#pragma once

#include "TowerTexture.h"
#include "TowerTypeEnum.h"
#include "Tower.h"
#include <memory>



namespace TowerTypeTexture {
	TowerTexture Generate(TowerType e);
};

namespace TowerTypeInstance {
	Tower Generate(TowerType e, const glm::vec2& pos);
};