#pragma once

#include "Camera.h"

class Scene
{
protected:
	std::shared_ptr<Camera> mCamera;
public:
	Scene(const std::shared_ptr<Camera>& camera)
		: mCamera(camera) {}
	virtual ~Scene() = default;

	virtual void Update(double delta) noexcept {};
	virtual void Render() noexcept {};
};