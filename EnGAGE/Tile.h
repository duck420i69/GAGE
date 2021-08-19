#pragma once

#include "Texture.h"
#include "TileTypeEnum.h"

struct Tile
{
	std::string name;
	std::weak_ptr<Texture> texture;
	unsigned int type;
};