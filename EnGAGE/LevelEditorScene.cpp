#include "pch.h"
#include "LevelEditorScene.h"

#include <glad/glad.h>

#include "thirdparty/imgui/imgui.h"

#include "Events.h"
#include "Asset.h"

static std::mt19937 eng;

static std::uniform_real_distribution<float> randPos(-1, 1);
static std::uniform_real_distribution<float> randScale(0.1f, 1);
static std::uniform_real_distribution<float> randColor(0, 1);
static std::uniform_int_distribution<uint32_t> randTexture(1, 2);

LevelEditorScene::LevelEditorScene()
{
	eng.seed(10);

}

LevelEditorScene::~LevelEditorScene()
{

}

void LevelEditorScene::Update(double delta) noexcept
{
	mCamera.UpdateProjection();

	if (Events::IsKeyDown(Events::KEY_F))
	{
		auto ptr = std::make_shared<GameObject>("");
		ptr->mTransform.mScale.x = randScale(eng);
		ptr->mTransform.mScale.y = randScale(eng);
		ptr->mTransform.mScale.z = 1;
		ptr->mTransform.mPos.x = randPos(eng);
		ptr->mTransform.mPos.y = randPos(eng);
		if (randTexture(eng) == 1)
		{
			ptr->AddComponent<SpriteRenderer>(Asset::GetTexture("Assets/Textures/testImage.png"));
		}
		else if (randTexture(eng) == 2)
		{
			ptr->AddComponent<SpriteRenderer>(Asset::GetTexture("Assets/Textures/testImage2.png"));
		}
		this->AddGameObject(ptr);
	}
}

void LevelEditorScene::Render() noexcept
{
	this->mRenderer.Render();
}
