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
static std::uniform_int_distribution<uint32_t> randTexture(0, 25);

LevelEditorScene::LevelEditorScene()
{
	eng.seed(10);

	auto sheet_texture = Asset::GetTexture("Assets/Textures/spritesheet.png");
	Asset::AddSpriteSheets("Blocks", std::make_shared<SpriteSheet>(sheet_texture, 16, 16, 26, 0));
}

LevelEditorScene::~LevelEditorScene()
{

}

void LevelEditorScene::Update(double delta) noexcept
{
	mCamera.UpdateProjection();
	auto sprite_sheet = *Asset::GetSpriteSheets("Blocks");
	if (Events::IsKeyDown(Events::KEY_F))
	{
		auto ptr = std::make_shared<GameObject>("");
		ptr->mTransform.mScale.x = randScale(eng);
		ptr->mTransform.mScale.y = randScale(eng);
		ptr->mTransform.mScale.z = 1;
		ptr->mTransform.mPos.x = randPos(eng);
		ptr->mTransform.mPos.y = randPos(eng);
		ptr->AddComponent<SpriteRenderer>(sprite_sheet[randTexture(eng)]);
		this->AddGameObject(ptr);
	}
}

void LevelEditorScene::Render() noexcept
{
	this->mRenderer.Render();
}
