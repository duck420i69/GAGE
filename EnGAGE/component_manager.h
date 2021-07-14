#pragma once

#include "common.h"
#include "logger.h"


#include "components.h"
#include "component_array.h"


#include <cassert>

class ComponentManager
{
	UnorderedMap<const char*, ComponentType>			mComponentTypes;
	UnorderedMap<const char*, Shared<IComponentArray>>	mComponentArrays;
	ComponentType										mNextComponentType;
public:
	ComponentManager() :
		mComponentTypes(),
		mComponentArrays(),
		mNextComponentType(0)
	{}

	template<typename T>
	void registerComponent()
	{
		const char* typeName = typeid(T).name();
		assert(mComponentTypes.find(typeName) == mComponentTypes.end() && "Registering component type more than once.");
		Logger::info("Registering component: {}, id: {}", typeName, mNextComponentType);

		//Thêm vào map
		mComponentTypes.insert({ typeName, mNextComponentType });
		mComponentArrays.insert({ typeName, makeShared<ComponentArray<T>>() });

		mNextComponentType++;
	}

	template<typename T>
	ComponentType getComponentType()
	{
		const char* typeName = typeid(T).name();
		assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before use.");
		// Return this component's type - used for creating signatures
		return mComponentTypes[typeName];
	}
	template<typename T>
	void addComponent(Entity e, T component)
	{
		Logger::info("Adding component.(Entity: {}, type: {})", e, typeid(T).name());
		getComponentArray<T>()->insertData(e, component);
	}
	template<typename T>
	void removeComponent(Entity e)
	{
		Logger::info("Removing component.(Entity: {}, type: {})", e, typeid(T).name());
		getComponentArray<T>()->removeData(e);
	}

	template<typename T>
	T& getComponent(Entity e)
	{
		return getComponentArray<T>()->getData(e);
	}

	void entityDestroyed(Entity e)
	{
		for (const auto& pair : mComponentArrays)
		{
			const auto& component = pair.second;
			component->entityDestroyed(e);
		}
	}
private:
	template<typename T>
	Shared<ComponentArray<T>> getComponentArray()
	{
		const char* typeName = typeid(T).name();
		assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before use.");
		return sharedStaticCast<ComponentArray<T>>(mComponentArrays[typeName]);
	}
};