#pragma once

#include <set>

namespace ECS
{
	struct BaseComponent;
	class  BaseSystem;

	const size_t MAX_ENTITY_COUNT = 5000;
	const size_t MAX_COMPONENT_COUNT = 32;

	using EntityID = size_t;
	using SystemID = size_t;
	using ComponentID = size_t;
	using EntitySignature = std::set<ComponentID>;

	inline static const ComponentID getRuntimeComponentID() noexcept
	{
		static ComponentID id = 0u;
		return id++;
	}

	inline static const ComponentID getRuntimeSystemID() noexcept
	{
		static SystemID id = 0u;
		return id++;
	}

	template<typename T>
	inline static ComponentID getComponentType() noexcept
	{
		static_assert((std::is_base_of<BaseComponent, T>::value && !std::is_same<BaseComponent, T>), "invalid template");
		static const ComponentID id = getRuntimeComponentID();
		return id;
	}

	template<typename T>
	inline static SystemID getSystemID() noexcept
	{
		static_assert((std::is_base_of<BaseSystem, T>::value && !std::is_same<BaseSystem, T>), "invalid template");
		static const SystemID id = getRuntimeSystemID();
		return id;
	}
}