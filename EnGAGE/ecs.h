#pragma once

#include "common.h"
#include "component_manager.h"
#include "entity_manager.h"
#include "system_manager.h"

class ECS
{
	static Shared<ECS> sInstance;

	Unique<ComponentManager> mComponentManager;
	Unique<EntityManager> mEntityManager;
	Unique<SystemManager> mSystemManager;
public:
	static Shared<ECS> getInstance();

	void init()
	{
		mComponentManager = makeUnique<ComponentManager>();
		mEntityManager = makeUnique<EntityManager>();
		mSystemManager = makeUnique<SystemManager>();
	}

	//Entity
	Entity createEntity()
	{
		return mEntityManager->createEntity();
	}

	void destroyEntity(Entity e)
	{
		mEntityManager->destroyEntity(e);
		mComponentManager->entityDestroyed(e);
		mSystemManager->entityDestroyed(e);
	}

	//Component
	template<typename T>
	void registerComponent()
	{
		mComponentManager->registerComponent<T>();
	}

	template<typename T>
	void addComponent(Entity e, T component)
	{
		mComponentManager->addComponent<T>(e, component);

		auto signature = mEntityManager->getSignature(e);
		signature.set(mComponentManager->getComponentType<T>(), true);
		mEntityManager->setSignature(signature, e);

		mSystemManager->entitySignatureChanged(e, signature);
	}

	template<typename T>
	void removeComponent(Entity e)
	{
		mComponentManager->removeComponent<T>(e);

		auto signature = mEntityManager->getSignature(e);
		signature.set(mComponentManager->getComponentType<T>(), false);
		mEntityManager->setSignature(signature, e);

		mSystemManager->entitySignatureChanged(e, signature);
	}
	template<typename T>
	T& getComponent(Entity entity)
	{
		return mComponentManager->getComponent<T>(entity);
	}

	template<typename T>
	ComponentType getComponentType()
	{
		return mComponentManager->getComponentType<T>();
	}

	// System
	template<typename T>
	Shared<T> registerSystem()
	{
		return mSystemManager->registerSystem<T>();
	}

	template<typename T>
	void setSystemSignature(Signature signature)
	{
		mSystemManager->setSignature<T>(signature);
	}
};