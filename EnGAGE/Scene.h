#pragma once

#include "Camera.h"
#include "GameObject.h"

class Scene
{
protected:
	std::shared_ptr<Camera> mCamera;
	std::vector<std::shared_ptr<GameObject>> mGameObject;
public:
	Scene(const std::shared_ptr<Camera>& camera)
		: mCamera(camera) {}
	virtual ~Scene() = default;

	virtual void Update(double delta) noexcept {};
	virtual void Render() noexcept {};
protected:
	void AddGameObject(const std::shared_ptr<GameObject>& go) noexcept
	{
		mGameObject.push_back(go);
	}
};