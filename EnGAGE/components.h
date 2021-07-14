#pragma once

#include "common.h"

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

using ComponentType = U8;

constexpr U32 MAX_COMPONENTS = 16;

struct Transform
{
	glm::vec3 position;
	glm::quat rotation;
	Real	  scale;
};