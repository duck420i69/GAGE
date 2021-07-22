#pragma once


class GameObject;
class Component
{
public:
	GameObject* mGameObject;
public:
	Component() :
		mGameObject(nullptr) {}

	virtual void Update(double dt) noexcept = 0;
};