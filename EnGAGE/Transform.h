#pragma once

#include <glm/vec3.hpp>

class Transform
{
	using Vec3 = glm::vec3;

public:
	Vec3 mPos, mRot, mScale;
public:
	Transform(const Vec3& pos, const Vec3& rot, const Vec3& scale) :
		mPos(pos), mRot(rot), mScale(scale) {}


	Transform() :
		Transform({ 0, 0, 0 }, { 0, 0, 0 }, {1, 1, 1})
	{}
};