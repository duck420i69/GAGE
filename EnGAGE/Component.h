#pragma once


class GameObject;
class Component
{
public:
	GameObject* mGameObject;
public:
	Component() noexcept :
		mGameObject(nullptr)
	{}
	virtual ~Component() = default;

	virtual void Start() noexcept = 0;
	virtual void Update(double dt) noexcept = 0;
};