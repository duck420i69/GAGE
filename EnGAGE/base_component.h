#pragma once

#include "types.h"

namespace ECS
{
	struct BaseComponent
	{
		friend class EntityManager;
		EntityID	mEntityID;
	public:
		BaseComponent() = default;
		virtual ~BaseComponent() = default;

		inline const EntityID getID() const { return mEntityID; }
	};
}