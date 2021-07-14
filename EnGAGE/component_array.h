#pragma once

#include "common.h"

#include "entity.h"
#include "logger.h"


class IComponentArray
{
public:
	virtual void entityDestroyed(Entity e) = 0;
	virtual ~IComponentArray() = default;
};

template<typename T>
class ComponentArray : public IComponentArray
{
	Arr<T, MAX_ENTITIES>		mArray;
	UnorderedMap<Entity, U64>	mEntityToIndex;
	UnorderedMap<U64, Entity>   mIndexToEntity;
	U64							mSize;
public:
	ComponentArray() :
		mArray(),
		mEntityToIndex(),
		mIndexToEntity(),
		mSize(0)
	{}

	void insertData(Entity e, T component)
	{
		assert(mEntityToIndex.find(e) == mEntityToIndex.end() && "Component added to same entity more than once.");
		

		//Thêm component vào cuối map và cập nhập 2 map
		U64 newIndex = mSize;
		mEntityToIndex[e] = newIndex;
		mIndexToEntity[newIndex] = e;
		mArray[newIndex] = component;
		mSize++;
	}

	void removeData(Entity e)
	{
		assert(mEntityToIndex.find(e) != mEntityToIndex.end() && "Removing non-existent component.");
		
		//Copy đầu cuối của mảng vào chỗ cần xóa
		U64 indexOfRemovedEntity = mEntityToIndex[e];
		U64 indexOfLastElement = mSize - 1;
		mArray[indexOfRemovedEntity] = mArray[indexOfLastElement];

		//Cập nhập lại hai máp
		Entity entityOfLastElement = mIndexToEntity[indexOfLastElement];
		mEntityToIndex[entityOfLastElement] = indexOfRemovedEntity;
		mIndexToEntity[indexOfRemovedEntity] = entityOfLastElement;

		mEntityToIndex.erase(e);
		mIndexToEntity.erase(indexOfLastElement);
		mSize--;
	}

	T& getData(Entity e)
	{
		assert(mEntityToIndex.find(e) != mEntityToIndex.end() && "Retrieving non-existent component.");
		
		//Trở về con trỏ
		return mArray[mEntityToIndex[e]];
	}

	void entityDestroyed(Entity e) override
	{
		if (mEntityToIndex.find(e) != mEntityToIndex.end())
		{
			removeData(e);
		}
	}
};