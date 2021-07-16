#include "pch.h"
#include "ecs.h"

std::shared_ptr<ECS> ECS::sInstance;

ECS& ECS::getInstance() noexcept
{
	if (!sInstance)
	{
		try
		{
			sInstance = std::make_shared<ECS>();
		}
		catch (std::bad_alloc& e)
		{
			Logger::error("Exception thrown: {}", e.what());
		}
	}
	return *sInstance;
}
