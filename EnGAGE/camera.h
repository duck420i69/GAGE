#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Camera
{
	glm::mat4x4 mProjection, mView;
	glm::vec3	mPos;
	float		mPitch, mYaw, mRoll;
	float		mFov;
	float		mNear, mFar;
public:
	Camera() noexcept;

	void buildMatrices();

	inline const glm::mat4x4& getProj() const noexcept { return mProjection; }
	inline const glm::mat4x4& getView() const noexcept { return mView; }
	inline glm::vec3& getPosition() noexcept { return mPos; }
};