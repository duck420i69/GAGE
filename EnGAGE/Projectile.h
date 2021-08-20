#pragma once

#include "ProjectileType.h"
#include "Texture.h"
#include "Enemy.h"

#include <glm/vec2.hpp>
#include <memory>

struct Projectile 
{
	ProjectileType type;
	std::weak_ptr<Texture> texture;
	glm::vec2 pos;
	glm::vec2 vel;
	float speed;
	float damage;
};

class ProjectileManager
{
	std::vector<Projectile> mProjectiles;
public:
	void Spawn(ProjectileType type, const glm::vec2& pos, const glm::vec2& dir, float speed, float damage) noexcept;
	void Update(float delta, unsigned int map_width, unsigned int map_height, std::vector<Enemy>& enemies) noexcept;


	inline const std::vector<Projectile>& GetProjectiles() const noexcept { return mProjectiles; };
};