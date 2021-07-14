#include "pch.h"
#include "ecs.h"

std::shared_ptr<ECS> ECS::sInstance;

ECS& ECS::getInstance()
{
	if (!sInstance)
	{
		sInstance = std::make_shared<ECS>();
	}
	return *sInstance;
}
