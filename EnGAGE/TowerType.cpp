#include "pch.h"
#include "TowerType.h"

#include "Asset.h"

TowerTexture TowerTypeTexture::Generate(TowerType e)
{
	switch (e)
	{
	case TowerType::BASIC:
		return { 
			Asset::GetTexture("Assets/Textures/tower_generic_base.png") ,
			Asset::GetTexture("Assets/Textures/tower_generic_cannon.png") };
	}

	return {};
}

Tower TowerTypeInstance::Generate(TowerType e, const glm::vec2& pos)
{
	TowerTexture tex = TowerTypeTexture::Generate(e);

	switch (e)
	{
	case TowerType::BASIC:
		return Tower{ TowerType::BASIC, pos, tex.base_texture, tex.cannon_texture, 12.0f, 0.04f, ProjectileType::CANNON_BALL, 24.0f, 1.0f };
	}

	return Tower{ TowerType::NONE, glm::vec2(), std::weak_ptr<Texture>(), std::weak_ptr<Texture>(), 0, 0, ProjectileType::NONE, 0, 0 };
}
