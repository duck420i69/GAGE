#pragma once

#include "Scene.h"
#include "Logger.h"

class Globals
{
public:
	static uint32_t screen_width, screen_height;
	static std::string screen_title;

	static std::unique_ptr<Scene> current_scene;

	template<class T>
	static void ChangeScene() noexcept
	{
		auto new_scene = std::make_unique<T>();
		
		const char* new_scene_name = new_scene->GetName();
		if (Globals::current_scene)
		{
			const char* current_scene_name = current_scene->GetName();
			if (memcmp(new_scene_name, current_scene_name, strlen(current_scene_name)) == 0 )
			{
				return;
			}
			current_scene.reset();
		}

		Logger::info("Switching current scene to: {}", new_scene_name);
		current_scene = std::move(new_scene);
	}
};