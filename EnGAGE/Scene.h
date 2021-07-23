#pragma once

#include "Camera.h"
#include "GameObject.h"
#include "Renderer.h"


class Scene
{
protected:
	Renderer mRenderer;
	Camera mCamera;
	std::vector<std::shared_ptr<GameObject>> mGameObjects;
public:
	virtual ~Scene() = default;

	virtual void Update(double delta) noexcept {};
	virtual void Render() noexcept {};

	inline const Camera& GetCamera() const noexcept { return mCamera; }
protected:
	void AddGameObject(const std::shared_ptr<GameObject>& go) noexcept
	{
		mGameObjects.push_back(go);
		mRenderer.Add(*go);
	}
};