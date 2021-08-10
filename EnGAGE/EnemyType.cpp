#include "pch.h"
#include "EnemyType.h"

#include "Asset.h"

std::weak_ptr<Texture> EnemyTypeTexture::Get(EnemyType e) noexcept
{
	switch (e)
	{
	case EnemyType::GENERIC:
		return Asset::GetTexture("Assets/Textures/enemy_generic.png");
	}
	assert(!"Undefined enemytype !");
	return std::weak_ptr<Texture>();
}
