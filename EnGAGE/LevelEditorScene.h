#pragma once

#include "Scene.h"
#include "Shader.h"
#include "Texture.h"

class LevelEditorScene : public Scene
{
	Shader mShader;
	Texture mTestTexture;
public:
	LevelEditorScene();
	~LevelEditorScene();

	void Update(double delta) noexcept override;
	void Render() noexcept override;
};