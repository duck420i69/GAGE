#pragma once

#include "entity_manager.h"
#include "types.h"

namespace ECS
{
	class Entity
	{
		EntityID mID;
		EntityManager* mMng;
	public:
		Entity(EntityID id, EntityManager* mng) : mID(id), mMng(mng) {}
		~Entity() = default;

		const EntityID getID() const { return mID; }


		template<typename T, typename... Args>
		void addComponent(Args&&... args)
		{
			mMng->addComponent<T>(mID, std::forward<Args>(args)...);
		}

		template<typename T>
		void addComponent(T& component)
		{
			mMng->addComponent<T>(mID, component);
		}

		template<typename T>
		T& getComponent()
		{
			return mMng->getComponent<T>(mID);
		}

		template<typename T>
		void removeComponent()
		{
			mMng->removeComponent<T>(mID);
		}
		
		template<typename T>
		bool hasComponent()
		{
			return mMng->hasComponent<T>(mID);
		}

		void destroy()
		{
			mMng->destroyEntity(mID);
		}
	};
}