#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.h"

class Camera
{
	glm::mat4x4 mProjectionMatrix, mViewMatrix;
	glm::vec3	mPosition, mFront, mUp;
	//float		mFov;
	//float		mNear, mFar;
public:
	Camera(const glm::vec3& position) noexcept :
		mProjectionMatrix(1.0f), mViewMatrix(1.0f),
		mPosition(position), mFront(0, 0, -1), mUp(0, 1, 0)
		//mFov(60.0f), mNear(1.0f), mFar(100.0f) 
	{}

	inline void UpdateProjection() noexcept
	{
		mProjectionMatrix = glm::ortho<float>(-1, 1, -1, 1);
		mViewMatrix = glm::lookAt(mPosition, mPosition + mFront, mUp);
	}

	inline const glm::mat4& GetViewMatrix() const noexcept
	{
		return mViewMatrix;
	}

	inline const glm::mat4& GetProjection() const noexcept
	{
		return mProjectionMatrix;
	}
	inline void MovePosition(float x, float y, float z) noexcept
	{
		mPosition.x += x;
		mPosition.y += y;
		mPosition.z += z;
	}
	inline void SetPosition(float x, float y, float z) noexcept
	{
		mPosition.x = x;
		mPosition.y = y;
		mPosition.z = z;
	}
};