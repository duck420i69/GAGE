#pragma once

#include "system.h"
#include "camera.h"
#include "shader.h"

class RenderSystem : public System
{
	Shader	mShader;
	int32_t mModelLoc, mProjLoc, mViewLoc;
public:
	RenderSystem();

	void render(const Camera& camera) const noexcept;
};