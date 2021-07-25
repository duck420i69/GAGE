#pragma once

#include <glm/vec3.hpp>

class Transform
{
public:
	glm::vec3 mPos, mRot, mScale;
public:
	Transform() :
		Transform({ 0, 0, 0 }, { 0, 0, 0 }, { 1, 1, 1 })
	{}
	Transform(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale) :
		mPos(pos), mRot(rot), mScale(scale) {}	
	

	inline const bool operator==(const Transform& other) const noexcept
	{
		return
			mPos == other.mPos &&
			mRot == other.mRot &&
			mScale == other.mScale;
	}
	inline const bool operator!=(const Transform& other) const noexcept
	{
		return  !(*this == other);
	}
};