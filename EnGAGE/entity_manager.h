#pragma once

#include "logger.h"

#include "entity.h"
#include "signature.h"

#include <cassert>

class EntityManager
{
	uint32_t								mLivingEnttiyCount;
	std::queue<Entity>						mAvailableEntities;
	std::array<Signature, MAX_ENTITIES>		mSignatures;
public:
	EntityManager() noexcept :
		mLivingEnttiyCount(0),
		mAvailableEntities(),
		mSignatures() 
	{
		for (Entity e = 0; e < MAX_ENTITIES; e++)
		{
			mAvailableEntities.push(e);
		}
	}

	Entity createEntity() noexcept
	{
		assert(mLivingEnttiyCount < MAX_ENTITIES && "Too many entities in existence.");

		// Lấy ID từ phía trước của queue
		Entity e = mAvailableEntities.front();
		Logger::info("Entity created(id: {}).", e);
		mAvailableEntities.pop();
		mLivingEnttiyCount++;

		return e;
	}

	void destroyEntity(Entity e) noexcept
	{
		assert(e < MAX_ENTITIES && "Entity out of range.");
		Logger::info("Entity destroyed(id: {}).", e);
		// Phá chữ kí của entity
		mSignatures[e].reset();

		//Đẩy entity ngược lại vào queue
		mAvailableEntities.push(e);
		mLivingEnttiyCount--;
	}

	void setSignature(Signature s, Entity e) noexcept
	{
		assert(e < MAX_ENTITIES && "Entity out of range.");
		mSignatures[e] = s;
	}

	const Signature& getSignature(Entity e) const noexcept
	{
		assert(e < MAX_ENTITIES && "Entity out of range.");
		return mSignatures[e];
	}
};