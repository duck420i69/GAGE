#pragma once

#include "Scene.h"

class MenuScene : public Scene {
public:
	void Update(float delta) noexcept override;
	void Render() noexcept override;
	void ImGui() noexcept override;
};