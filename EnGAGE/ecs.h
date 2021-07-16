#pragma once

#include "component_manager.h"
#include "entity_manager.h"
#include "system_manager.h"

class ECS
{
	static std::shared_ptr<ECS> sInstance;

	std::unique_ptr<ComponentManager> mComponentManager;
	std::unique_ptr<EntityManager> mEntityManager;
	std::unique_ptr<SystemManager> mSystemManager;
public:
	static ECS& getInstance() noexcept;

	void init() noexcept
	{
		try
		{
			mComponentManager = std::make_unique<ComponentManager>();
			mEntityManager = std::make_unique<EntityManager>();
			mSystemManager = std::make_unique<SystemManager>();
		}
		catch (std::bad_alloc& e)
		{
			Logger::error("Exception thrown: {}", e.what());
		}
	}

	//Creation

	Entity constructEntity(NameComponent name, TransformComponent transform) noexcept
	{
		Entity e = createEntity();
		addComponent<NameComponent>(e, name);
		addComponent<TransformComponent>(e, transform);
		return e;
	}

	//Entity
	Entity createEntity() noexcept
	{
		return mEntityManager->createEntity();
	}

	void destroyEntity(Entity e) noexcept
	{
		mEntityManager->destroyEntity(e);
		mComponentManager->entityDestroyed(e);
		mSystemManager->entityDestroyed(e);
	}

	//Component
	template<typename T>
	void registerComponent() noexcept
	{
		mComponentManager->registerComponent<T>();
	}

	template<typename T>
	void addComponent(Entity e, T component) noexcept
	{
		mComponentManager->addComponent<T>(e, component);

		auto signature = mEntityManager->getSignature(e);
		signature.set(mComponentManager->getComponentType<T>(), true);
		mEntityManager->setSignature(signature, e);

		mSystemManager->entitySignatureChanged(e, signature);
	}

	template<typename T>
	void removeComponent(Entity e) noexcept
	{
		mComponentManager->removeComponent<T>(e);

		auto signature = mEntityManager->getSignature(e);
		signature.set(mComponentManager->getComponentType<T>(), false);
		mEntityManager->setSignature(signature, e);

		mSystemManager->entitySignatureChanged(e, signature);
	}
	template<typename T>
	T& getComponent(Entity entity) noexcept
	{
		return mComponentManager->getComponent<T>(entity);
	}

	template<typename T>
	ComponentType getComponentType() noexcept
	{
		return mComponentManager->getComponentType<T>();
	}

	// System
	template<typename T>
	std::shared_ptr<T> registerSystem() noexcept
	{
		return mSystemManager->registerSystem<T>();
	}

	template<typename T>
	void setSystemSignature(Signature signature) noexcept
	{
		mSystemManager->setSignature<T>(signature);
	}
};