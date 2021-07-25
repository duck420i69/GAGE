#pragma once

#include "Scene.h"

class LevelEditorScene : public Scene 
{
public:
	LevelEditorScene();
	~LevelEditorScene();

	void Update(double delta) noexcept override;
	void Render() noexcept override;
};