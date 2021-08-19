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

void TowerTypeInstance::Generate(TowerType e, std::unique_ptr<Tower>& instance, const glm::vec2& pos)
{
	TowerTexture tex = TowerTypeTexture::Generate(e);

	switch (e)
	{
	case TowerType::BASIC:
		instance = std::make_unique<Tower>(pos, tex.base_texture, tex.cannon_texture, 6.0f, 1.0f);
		break;
	}
}
