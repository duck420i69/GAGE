#pragma once

#include "Camera.h"
#include "GameObject.h"
#include "Renderer.h"
#include "Thirdparty/imgui/imgui.h"

class Scene
{
protected:
	Renderer mRenderer;
	Camera mCamera;
	std::vector<std::shared_ptr<GameObject>> mGameObjects;
	std::weak_ptr<GameObject> mActiveGameObject;
public:
	virtual ~Scene() = default;

	virtual void Update(double delta) noexcept {};
	virtual void Render() noexcept {};
	virtual void ImGui() noexcept {};

	void SceneImGui() noexcept
	{
		if (auto ptr = mActiveGameObject.lock())
		{
			ImGui::Begin("Inspector");
			ptr->ImGui();
			ImGui::End();
		}
		ImGui();
	}

	inline const Camera& GetCamera() const noexcept { return mCamera; }
protected:
	void AddGameObject(const std::shared_ptr<GameObject>& go) noexcept
	{
		mGameObjects.push_back(go);
		mRenderer.Add(*go);
	}
};