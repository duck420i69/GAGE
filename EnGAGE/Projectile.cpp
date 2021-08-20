#include "pch.h"
#include "Projectile.h"

#include "ProjectileType.h"


void ProjectileManager::Spawn(ProjectileType type, const glm::vec2& pos, const glm::vec2& dir, float speed, float damage) noexcept
{
	mProjectiles.push_back(Projectile{ type, ProjecTileTypeTexture::Get(type), pos, dir, speed , damage });
}

void ProjectileManager::Update(float delta, unsigned int map_width, unsigned int map_height, std::vector<Enemy>& enemies) noexcept
{

	auto aabb_test = [](const glm::vec2& a, const glm::vec2& b) -> bool {
		static constexpr float WIDTH = 1.0f, HEIGHT = 1.0f;

		if ((a.x + WIDTH) > b.x && a.x < (b.x + WIDTH) && (a.y + HEIGHT) > b.y && a.y < (b.y + HEIGHT)) {
			return true;
		}

		return false;
	};


	std::vector<Projectile>::iterator it = mProjectiles.begin();
	while (it != mProjectiles.end()) {

		bool bound_check = it->pos.x < 0 || it->pos.y < 0 || it->pos.x > map_width || it->pos.y > map_height;
		bool aabb_check = false;

		for (auto& e : enemies) {
			if (aabb_check = aabb_test(e.GetPos(), it->pos)) {
				e.DoDamange(it->damage);
				break;
			}
		}

		if(bound_check || aabb_check) {
			it = mProjectiles.erase(it);
		} else {
			it->pos += it->vel * it->speed * delta;
			it++;
		}
	}
}
