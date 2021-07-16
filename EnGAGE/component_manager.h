#pragma once

#include "logger.h"


#include "components.h"
#include "component_array.h"


#include <cassert>

class ComponentManager
{
	std::unordered_map<const char*, ComponentType>						mComponentTypes;
	std::unordered_map<const char*, std::shared_ptr<IComponentArray>>	mComponentArrays;
	ComponentType														mNextComponentType;
public:
	ComponentManager()  noexcept :
		mComponentTypes(),
		mComponentArrays(),
		mNextComponentType(0)
	{}

	template<typename T>
	void registerComponent() noexcept
	{
		const char* typeName = typeid(T).name();
		assert(mComponentTypes.find(typeName) == mComponentTypes.end() && "Registering component type more than once.");
		Logger::info("Registering component: {}, id: {}", typeName, mNextComponentType);

		//Thêm vào map
		mComponentTypes.insert({ typeName, mNextComponentType });
		try
		{
			mComponentArrays.insert({ typeName, std::make_shared<ComponentArray<T>>() });
		}
		catch (std::bad_alloc& e)
		{
			Logger::error("Exception thrown: {}", e.what());
		}

		mNextComponentType++;
	}

	template<typename T>
	ComponentType getComponentType() noexcept
	{
		const char* typeName = typeid(T).name();
		assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before use.");
		// Return this component's type - used for creating signatures
		return mComponentTypes[typeName];
	}
	template<typename T>
	void addComponent(Entity e, T component) noexcept
	{
		Logger::info("Adding component.(Entity: {}, type: {})", e, typeid(T).name());
		getComponentArray<T>()->insertData(e, component);
	}
	template<typename T>
	void removeComponent(Entity e) noexcept
	{
		Logger::info("Removing component.(Entity: {}, type: {})", e, typeid(T).name());
		getComponentArray<T>()->removeData(e);
	}

	template<typename T>
	T& getComponent(Entity e) noexcept
	{
		return getComponentArray<T>()->getData(e);
	}

	void entityDestroyed(Entity e) noexcept
	{
		for (const auto& pair : mComponentArrays)
		{
			const auto& component = pair.second;
			component->entityDestroyed(e);
		}
	}
private:
	template<typename T>
	std::shared_ptr<ComponentArray<T>> getComponentArray() noexcept
	{
		const char* typeName = typeid(T).name();
		assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before use.");
		return std::static_pointer_cast<ComponentArray<T>>(mComponentArrays[typeName]);
	}
};