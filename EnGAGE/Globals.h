#pragma once

#include "Scene.h"
#include "Logger.h"

class Globals
{
public:
	static uint32_t gScreenWidth, gScreenHeight;
	static std::string gScreenTitle;


	template<typename T>
	static void ChangeScene(std::unique_ptr<Scene>& current_scene) noexcept
	{
		if (current_scene)
		{
			if (typeid(*current_scene).name() == typeid(T).name())
			{
				return;
			}
			current_scene.reset();
		}

		Logger::info("Switching current scene to: {}", typeid(T).name());
		current_scene = std::make_unique<T>();
	}
};