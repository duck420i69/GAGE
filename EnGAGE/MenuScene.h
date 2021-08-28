#pragma once

#include "Scene.h"

#include "Box.h"

class MenuScene final : public Scene{
	Box mBox;
public:

	void Update(float delta) noexcept override {};
	void Render() noexcept override {
		mBox.Draw();
	};
	void ImGui() noexcept override {};

	const char* GetName() const noexcept { return "Menu Scene"; }
};
