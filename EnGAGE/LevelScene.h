#pragma once

#include "Scene.h"

class LevelScene : public IScene
{
public:
	void Update(double delta) noexcept override;
	void Render() noexcept override;
};