#include "pch.h"
#include "TowerType.h"

#include "Asset.h"
#include "BasicTower.h"

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
	switch (e)
	{
	case TowerType::BASIC:
		instance = std::make_unique<BasicTower>(pos);
		break;
	}
}
