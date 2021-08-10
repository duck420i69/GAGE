#pragma once

#include "Texture.h"

enum class EnemyType {
	GENERIC,
	COUNT
};


namespace EnemyTypeTexture {
	std::weak_ptr<Texture> Get(EnemyType e) noexcept;
};