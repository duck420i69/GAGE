#pragma once

#include "Scene.h"

#include "Camera.h"
#include "PointLight.h"
#include "Model.h"
#include "window.h"
#include "TestPlane.h"
#include "RenderQueue.h"
#include "ModelProbe.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <imgui.h>


class MenuScene final : public Scene {
	Model mMuro, mSponza;
	//TestPlane mPlane, mBox2;
	Camera mCam;
	PointLight mLight;
	RenderQueue mRenQueue;
public:
	MenuScene() noexcept;
	void Update(float delta) noexcept override;
	void Render() noexcept override;
	void ImGui() noexcept override;

	const char* GetName() const noexcept { return "Menu Scene"; }
};
