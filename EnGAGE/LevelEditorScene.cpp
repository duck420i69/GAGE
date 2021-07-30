#include "pch.h"
#include "LevelEditorScene.h"

#include <glad/glad.h>

#include "thirdparty/imgui/imgui.h"

#include "Events.h"
#include "Asset.h"
#include "Logger.h"
#include "Globals.h"

static std::mt19937 eng;

static std::uniform_real_distribution<float> randPos(-100, 100);
static std::uniform_real_distribution<float> randScale(1.0f, 50);
static std::uniform_real_distribution<float> randColor(0, 1);
static std::uniform_int_distribution<uint32_t> randTexture(0, 25);

static bool g_first_time = true;

static std::weak_ptr<SpriteSheet> g_sprite_sheet = std::weak_ptr<SpriteSheet>();
LevelEditorScene::LevelEditorScene()
{
	eng.seed(10);

	auto sheet_texture = Asset::GetTexture("Assets/Textures/spritesheet.png");
	Asset::AddSpriteSheets("Blocks", std::make_shared<SpriteSheet>(sheet_texture, 16, 16, 26, 0));
	mGameObjects = Globals::LoadAllGameObjects();
}

LevelEditorScene::~LevelEditorScene()
{
	Globals::SaveAllGameObjects(mGameObjects);
}

void LevelEditorScene::Update(double delta) noexcept
{
	mCamera.UpdateProjection();
	g_sprite_sheet = Asset::GetSpriteSheets("Blocks");
	if (Events::IsKeyDownOnce(Events::KEY_F))
	{
		auto ptr = std::make_shared<GameObject>("Dumb ass", 0);

		if (g_first_time)
		{
			this->mActiveGameObject = ptr;
			g_first_time = false;
		}

		ptr->mTransform.mScale.x = randScale(eng);
		ptr->mTransform.mScale.y = randScale(eng);
		ptr->mTransform.mScale.z = 1;
		ptr->mTransform.mPos.x = randPos(eng);
		ptr->mTransform.mPos.y = randPos(eng);
		ptr->AddComponent<SpriteRenderer>((*g_sprite_sheet.lock())[randTexture(eng)]);
		this->AddGameObject(ptr);
	}

	
	for (auto& go : mGameObjects)
	{
		go->Update(delta);
	}
}

void LevelEditorScene::Render() noexcept
{
	this->mRenderer.Render();
}

void LevelEditorScene::ImGui() noexcept
{
	ImGui::Begin("Level editor");
	ImGui::Text("Hello world");
	ImGui::End();
}
