#pragma once

#include "Scene.h"

class LevelScene : public Scene
{
public:
	LevelScene() noexcept;
	void Update(double delta) noexcept override;
	void Render() noexcept override;
};