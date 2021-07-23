#include "pch.h"
#include "LevelEditorScene.h"

#include <glad/glad.h>

#include "thirdparty/imgui/imgui.h"

#include "Events.h"

static std::mt19937 eng;

static std::uniform_real_distribution<float> randPos(-1, 1);
static std::uniform_real_distribution<float> randScale(0.1f, 1);
static std::uniform_real_distribution<float> randColor(0, 1);

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

	if (Events::IsKeyDownOnce(Events::KEY_F))
	{
		auto ptr = std::make_shared<GameObject>("");
		ptr->mTransform.mScale.x = randScale(eng);
		ptr->mTransform.mScale.y = randScale(eng);
		ptr->mTransform.mScale.z = 1;
		ptr->mTransform.mPos.x = randPos(eng);
		ptr->mTransform.mPos.y = randPos(eng);
		ptr->AddComponent<SpriteRenderer>(glm::vec4{ randColor(eng), randColor(eng), randColor(eng), 1 });
		this->AddGameObject(ptr);
	}
}

void LevelEditorScene::Render() noexcept
{
	this->mRenderer.Render();
}
