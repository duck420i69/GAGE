#include "pch.h"
#include "camera.h"

#include "window.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()  noexcept :
	mPos(0, 0, 0),
	mPitch(0), mYaw(0), mRoll(0),
	mFov(60.0f),
	mNear(0.1f), mFar(1000.0f)
{
}

void Camera::buildMatrices()
{
	mProjection = glm::perspective(glm::radians(mFov), (float)Window::getWidth() / (float)Window::getHeight(), mNear, mFar);
	mView = glm::translate(glm::mat4(1.0f), -mPos);
}
