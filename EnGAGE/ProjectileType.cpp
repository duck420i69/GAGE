#include "pch.h"
#include "ProjectileType.h"

#include "Asset.h"

std::weak_ptr<Texture> ProjecTileTypeTexture::Get(ProjectileType type) noexcept
{
	switch (type)
	{
	case ProjectileType::CANNON_BALL:
		return Asset::GetTexture("Assets/Textures/cannon_ball.png");
	}

	return {};
}
