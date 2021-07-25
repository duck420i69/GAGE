#include "pch.h"
#include "LevelEditorScene.h"

#include <glad/glad.h>

#include "thirdparty/imgui/imgui.h"

#include "Events.h"
#include "Asset.h"
#include "Logger.h"

static std::mt19937 eng;

static std::uniform_real_distribution<float> randPos(-1, 1);
static std::uniform_real_distribution<float> randScale(0.1f, 1);
static std::uniform_real_distribution<float> randColor(0, 1);
static std::uniform_int_distribution<uint32_t> randTexture(0, 25);

static std::shared_ptr<GameObject> g_first_object = nullptr;
static bool g_first_time = true;
static int g_sprite_index = 0;
static float g_sprite_time = 0.2f;
static float g_sprite_time_left = 0.0f;

static std::weak_ptr<SpriteSheet> g_sprite_sheet = std::weak_ptr<SpriteSheet>();
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
	g_sprite_sheet = Asset::GetSpriteSheets("Blocks");
	if (Events::IsKeyDownOnce(Events::KEY_F))
	{
		auto ptr = std::make_shared<GameObject>("");

		if (g_first_time)
		{
			g_first_object = ptr;
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

	if (g_first_object)
	{
		g_sprite_time_left -= delta;
		if (g_sprite_time_left <= 0.0f) {
			g_sprite_time_left = g_sprite_time;
			g_sprite_index++;
			if (g_sprite_index > 4)
			{
				g_sprite_index = 0;
			}
			g_first_object->GetComponent<SpriteRenderer>().lock()->SetSprite((*g_sprite_sheet.lock())[g_sprite_index]);
		}
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
