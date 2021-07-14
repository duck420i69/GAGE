#include "pch.h"
#include "ecs.h"

Shared<ECS> ECS::sInstance;

Shared<ECS> ECS::getInstance()
{
	if (!sInstance)
	{
		sInstance = makeShared<ECS>();
		sInstance->init();
	}
	return sInstance;
}
