#pragma once

#include "Scene.h"
#include "Shader.h"

class LevelEditorScene : public IScene
{
	Shader mShader;
public:
	LevelEditorScene();
	~LevelEditorScene();

	void Update(double delta) noexcept override;
	void Render() noexcept override;
};