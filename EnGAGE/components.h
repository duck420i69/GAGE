#pragma once


#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

using ComponentType = uint8_t;

constexpr uint32_t MAX_COMPONENTS = 16;

struct TransformComponent
{
	glm::vec3 position;
	glm::quat rotation;
	float	  scale;
};

struct MeshComponent
{
	uint32_t vao, vertex_count;
};