#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Camera
{
	using Vec3 = glm::vec3;
	using Mat4 = glm::mat4;

	Mat4	mProjectionMatrix, mViewMatrix;
	Vec3	mPosition, mFront, mUp;
	//float		mFov;
	//float		mNear, mFar;
public:
	Camera() noexcept :
		Camera({ 0, 0, 0 }, { 0, 0 , -1 }, {0, 1, 0})
	{}

	Camera(const Vec3& pos) noexcept :
		Camera(pos, { 0, 0 , -1 }, { 0, 1, 0 })
	{}

	Camera(const Vec3& pos, const Vec3& front, const Vec3& up) noexcept :
		mProjectionMatrix(1.0f), mViewMatrix(1.0f),
		mPosition(pos), mFront(front), mUp(up) {}

	inline void UpdateProjection() noexcept
	{
		mProjectionMatrix = glm::ortho<float>(-100, 100, -100, 100);
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