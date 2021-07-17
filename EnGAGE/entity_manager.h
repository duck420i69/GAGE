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
		std::map<EntityID, std::shared_ptr<EntitySignature>>	mEntitySignatures;
		std::map<SystemID, std::unique_ptr<BaseSystem>>			mRegisteredSystems;
		std::map<ComponentID, std::shared_ptr<IComponentArray>> mComponentArray;
	public:
		EntityManager() : mEntityCount(0u)
		{
			for (EntityID e = 0u; e < MAX_ENTITY_COUNT; e++)
			{
				mAvalidableEntities.push(e);
			}
		}
		~EntityManager() = default;

		const EntityID createEntity()
		{
			const EntityID id = mAvalidableEntities.front();
			mAvalidableEntities.pop();
			addEntitySignature(id);
			mEntityCount++;
			return id;
		}

		void destroyEntity(const EntityID e)
		{
			assert(e < MAX_ENTITY_COUNT && "EntityID dat qua muc toi da !");
			mEntitySignatures.erase(e);

			for (auto& [componentID, pComponentArray] : mComponentArray)
			{
				pComponentArray->erase(e);
			}

			for (auto& [systemID, pSystem] : mRegisteredSystems)
			{
				pSystem->removeEntity(e);
			}
			mEntityCount--;
			mAvalidableEntities.push(e);
		}

		void update()
		{
			for (const auto& [systemID, pSystem] : mRegisteredSystems)
			{
				pSystem->update();
			}
		}

		void render()
		{
			for (const auto& [systemID, pSystem] : mRegisteredSystems)
			{
				pSystem->render();
			}
		}

		template<typename T, typename... Args>
		void addComponent(const EntityID e, Args&&... args)
		{
			assert(e < MAX_ENTITY_COUNT && "EntityID dat qua muc toi da !");
			assert(mEntitySignatures[e]->size() < MAX_COMPONENT_COUNT && "So luong component dat muc toi da");

			T component(std::forward<Args>(args)...);
			component.mEntityID = e;
			getEntitySignature(e)->insert(getComponentType<T>());
			getComponentArray<T>()->insert(component);
			updateEntityTargetSystem(e);
		}

		template<typename T>
		T& getComponent(const EntityID e)
		{
			assert(e < MAX_ENTITY_COUNT && "EntityID dat qua muc toi da !");
			return getComponentArray<T>()->get(e);
		}

		template<typename T>
		const bool hasComponent(const EntityID e)
		{
			assert(e < MAX_ENTITY_COUNT && "EntityID dat qua muc toi da !");
			const EntitySignature signature = mEntitySignatures.at(e);
			const ComponentID compID = getComponentType<T>();
			return (signature.count(compID) > 0);
		}


		template<typename T>
		void removeComponent(const EntityID e)
		{
			assert(e < MAX_ENTITY_COUNT && "EntityID dat qua muc toi da !");
			getEntitySignature(e)->erase(getComponentType<T>());
			getComponentArray<T>()->erase(e);
			updateEntityTargetSystem(e);
		}

		template<typename T>
		void registerSystem()
		{
			const SystemID sysID = getSystemType<T>();
			assert(mRegisteredSystems.count(sysID) == 0 && "System already registered !");
			auto system = std::make_unique<T>();

			for (EntityID e = 0; e < mEntityCount; e++)
			{
				processEntityToSystem(e, system.get());
			}

			system->init();
			mRegisteredSystems[sysID] = std::move(system);
		}

		template<typename T>
		void unregisterSystem()
		{
			const SystemID sysID = getSystemType<T>();
			assert(mRegisteredSystems.count(sysID) != 0 && "System not registered !");
			mRegisteredSystems.erase(sysID);
		}
	private:

		template<typename T>
		void addComponentArray()
		{
			const ComponentID compID = getComponentType<T>();
			assert(mComponentArray.find(compID) == mComponentArray.end() && "Component da dang ky");
			mComponentArray.insert({ compID, std::make_shared<ComponentArray<T>>() });
		}

		template<typename T>
		std::shared_ptr<ComponentArray<T>> getComponentArray()
		{
			const ComponentID compID = getComponentType<T>();
			if (mComponentArray.count(compID) == 0)
			{
				addComponentArray<T>();
			}
			return std::static_pointer_cast<ComponentArray<T>>(mComponentArray.at(compID));
		}

		void addEntitySignature(const EntityID e)
		{
			assert(mEntitySignatures.find(e) == mEntitySignatures.end() && "Signature not found.");
			mEntitySignatures.insert({ e, std::move(std::make_shared<EntitySignature>()) });
		}

		std::shared_ptr<EntitySignature> getEntitySignature(const EntityID e)
		{
			assert(mEntitySignatures.find(e) != mEntitySignatures.end() && "Signature not found.");
			return mEntitySignatures.at(e);
		}

		void updateEntityTargetSystem(const EntityID e)
		{
			for (auto& [systemID, pSystem] : mRegisteredSystems)
			{
				processEntityToSystem(e, pSystem.get());
			}
		}

		void processEntityToSystem(const EntityID e, BaseSystem* system)
		{
			if (belongToSystem(e, system->mSignature))
			{
				system->mEntities.insert(e);
			}
			else
			{
				system->mEntities.erase(e);
			}
		}

		bool belongToSystem(const EntityID e, const EntitySignature& systemSignature)
		{
			for (const ComponentID& comp : systemSignature)
			{
				if (getEntitySignature(e)->count(comp) == 0)
				{
					return false;
				}
			}
			return true;
		}
	};
}