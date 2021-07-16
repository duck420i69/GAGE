#pragma once

#include "entity.h"
#include "logger.h"


class IComponentArray
{
public:
	virtual void entityDestroyed(Entity e) noexcept = 0;
	virtual ~IComponentArray() noexcept = default;
};

template<typename T>
class ComponentArray : public IComponentArray
{
	std::array<T, MAX_ENTITIES>				mArray;
	std::unordered_map<Entity, uint64_t>	mEntityToIndex;
	std::unordered_map<uint64_t, Entity>	mIndexToEntity;
	uint64_t								mSize;
public:
	ComponentArray() noexcept :
		mArray(),
		mEntityToIndex(),
		mIndexToEntity(),
		mSize(0u)
	{}

	void insertData(Entity e, T component) noexcept
	{
		assert(mEntityToIndex.find(e) == mEntityToIndex.end() && "Component added to same entity more than once.");
		

		//Thêm component vào cuối map và cập nhập 2 map
		uint64_t newIndex = mSize;
		mEntityToIndex[e] = newIndex;
		mIndexToEntity[newIndex] = e;
		mArray[newIndex] = component;
		mSize++;
	}

	void removeData(Entity e) noexcept
	{
		assert(mEntityToIndex.find(e) != mEntityToIndex.end() && "Removing non-existent component.");
		
		//Copy đầu cuối của mảng vào chỗ cần xóa
		uint64_t indexOfRemovedEntity = mEntityToIndex[e];
		uint64_t indexOfLastElement = mSize - 1;
		mArray[indexOfRemovedEntity] = mArray[indexOfLastElement];

		//Cập nhập lại hai máp
		Entity entityOfLastElement = mIndexToEntity[indexOfLastElement];
		mEntityToIndex[entityOfLastElement] = indexOfRemovedEntity;
		mIndexToEntity[indexOfRemovedEntity] = entityOfLastElement;

		mEntityToIndex.erase(e);
		mIndexToEntity.erase(indexOfLastElement);
		mSize--;
	}

	T& getData(Entity e) noexcept
	{
		assert(mEntityToIndex.find(e) != mEntityToIndex.end() && "Retrieving non-existent component.");
		
		//Trở về con trỏ
		return mArray[mEntityToIndex[e]];
	}

	void entityDestroyed(Entity e) noexcept override
	{
		if (mEntityToIndex.find(e) != mEntityToIndex.end())
		{
			removeData(e);
		}
	}
};