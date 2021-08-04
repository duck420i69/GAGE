#pragma once

#include "Scene.h"
#include "Logger.h"

class Globals
{
public:
	static unsigned int gScreenWidth, gScreenHeight;
	static std::string gScreenTitle;
	static std::unique_ptr<Scene> gCurrentScene;

	template<typename T, typename...Args>
	static void ChangeScene(Args&&... args) noexcept
	{
		if (gCurrentScene)
		{
			if (typeid(*gCurrentScene).name() == typeid(T).name())
			{
				return;
			}
			gCurrentScene.reset();
		}

		Logger::info("Switching current scene to: {}", typeid(T).name());
		gCurrentScene = std::make_unique<T>(std::forward<Args>(args)...);
	}
};