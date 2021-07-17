#pragma once

#include "types.h"

namespace ECS
{
	struct BaseSystem
	{
		friend class EntityManager;
		EntitySignature		mSignature;
		std::set<EntityID>	mEntities;
	public:
		BaseSystem() = default;
		virtual ~BaseSystem() = default;

		virtual void init() {}
		virtual void update() {}
		virtual void shutdown() {}

		void addEntity(const EntityID id)
		{
			mEntities.insert(id);
		}

		void removeEntity(const EntityID id)
		{
			mEntities.erase(id);
		}

		const EntitySignature getSignature() const
		{
			return mSignature;
		}

		template<typename T>
		void addComponentSignature()
		{
			mSignature.insert(getComponentType<T>());
		}
	};
}