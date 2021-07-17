#pragma once

#include "types.h"
#include "base_system.h"
#include "base_component.h"
#include "component_array.h"

#include <queue>
#include <map>
#include <memory>

namespace ECS
{
	class EntityManager
	{
		EntityID mEntityCount;
		std::queue<EntityID>									mAvalidableEntities;
		std::map<EntityID, EntitySignature>						mEntitySignatures;
		std::map<SystemID, std::unique_ptr<BaseSystem>>			mRegisteredSystems;
		std::map<ComponentID, std::shared_ptr<IComponentArray>> mComponentArray;
	public:

	private:

	};
}