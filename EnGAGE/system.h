#pragma once

#include <set>
#include "entity.h"

class System
{
public:
	std::set<Entity> mEntities;
};