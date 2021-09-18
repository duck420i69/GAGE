#pragma once



#include "Scene.h"

#include "Camera.h"
#include "PointLight.h"
#include "Model.h"
#include "window.h"
#include "TestPlane.h"
#include "RenderQueue.h"

#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>


class MenuScene final : public Scene{
	//Model mMuro,  mSponza;
	TestPlane mPlane, mBox2;
	Camera mCam;
	PointLight mLight;
	RenderQueue mRenQueue;
public:
	MenuScene() noexcept :
		mCam()
		//mMuro("Assets/Models/muro/muro.obj"),
		//mSponza("Assets/Models/Sponza/sponza.obj")
		//mBrickWall("Assets/Models/brick_wall/brick_wall.obj")
	{		
		Opengl::SetProjection(glm::perspective(glm::radians(75.0f), 16.0f / 9.0f, 0.05f, 1000.0f));
	}
	void Update(float delta) noexcept override {
		if (Events::IsKeyDownOnce(Events::KEY_TAB)) {
			Window::ToggleCursor();
		}

		if (!Window::IsCursorEnabled()) {
			if (Events::IsKeyDown(Events::KEY_W)) {
				mCam.MoveForward(delta);
			}
			if (Events::IsKeyDown(Events::KEY_S)) {
				mCam.MoveForward(-delta);
			}
			if (Events::IsKeyDown(Events::KEY_D)) {
				mCam.MoveRight(delta);
			}
			if (Events::IsKeyDown(Events::KEY_A)) {
				mCam.MoveRight(-delta);
			}
			mCam.Rotate(Events::GetDY(), Events::GetDX());
		}
		Opengl::SetCamera(mCam.GetMatrix());
	};
	void Render() noexcept override {
		Opengl::Clear();
		mLight.Bind(mCam.GetMatrix());	
		mPlane.Submit(mRenQueue);
		mBox2.Submit(mRenQueue);

		mRenQueue.Execute();
		//mSponza.Draw();
		//mBrickWall.Draw();
		//mMuro.Draw();

		mRenQueue.Reset();
	};
	void ImGui() noexcept override {
		mCam.SpawnControlWindow();
		mLight.SpawnControlWindow();
		mPlane.SpawmControlWindow("Box1");
		mBox2.SpawmControlWindow("Box2");
		//mSponza.RenderTree("Sponza");
	};

	const char* GetName() const noexcept { return "Menu Scene"; }
};
