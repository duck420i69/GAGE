#pragma once


class GameObject;
class Component
{
	friend class Globals;
public:
	GameObject* mGameObject;
public:
	Component() noexcept :
		mGameObject(nullptr)
	{}
	virtual ~Component() = default;

	virtual void Start() noexcept {};
	virtual void Update(double dt) noexcept {};
	virtual void ImGui() noexcept {};
private:
	virtual void ParseComponent(std::ofstream& s) noexcept = 0;
};