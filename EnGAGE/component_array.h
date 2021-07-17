#pragma once

#include <vector>
#include <cassert>
#include <algorithm>

#include "types.h"

namespace ECS
{
	class IComponentArray
	{
	public:
		IComponentArray() = default;
		virtual ~IComponentArray() = default;
		virtual void erase(const EntityID id) = 0;
	};

	template<typename T>
	class ComponentArray
	{
	public:
		std::vector<T> mData;
	public:
		ComponentArray() = default;
		~ComponentArray() = default;

		void insert(const T& component)
		{
			auto compare = std::find_if(mData.begin(), mData.end(), [&](const T& c)
				{
					return c.getID() == component.getID();
				});
			if (compare != mData.end())
			{
				mData.push_back(component);
			}
		}

		T& get(const EntityID id)
		{
			auto compare = std::find_if(mData.begin(), mData.end(), [&](const T& c)
				{
					return c.getID() == id;
				});
			assert(compare != mData.end() && "Component khong ton tai !");
			return *compare;
		}
		void erase(const EntityID id) override final
		{
			auto compare = std::find_if(mData.begin(), mData.end(), [&](const T& c)
				{
					return c.getID() == id;
				});
			if (compare != mData.end())
			{
				mData.erase(compare);
			}
		}
	};
}