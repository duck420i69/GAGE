#pragma once

#include "Component.h"

class GameObject
{
	std::string mName;
	std::vector<std::shared_ptr<Component>> mComponents;
public:
	GameObject(const std::string& name) noexcept :
		mName(name)
	{}

	template<class T>
	std::weak_ptr<T> GetComponent() noexcept
	{
		for (auto& component : mComponents)
		{
			if (typeid(T).name() == typeid(*component).name())
			{
				return std::dynamic_pointer_cast<T>(component);
			}
		}
		return std::weak_ptr<T>{};
	}
	template<class T>
	void RemoveComponent() noexcept
	{
		for (auto it = mComponents.begin(); it != mComponents.end(); it++)
		{
			if (typeid(T).name() == typeid(**it).name())
			{
				mComponents.erase(it);
				return;
			}
		}
	}

	template<class T, typename... Args>
	void AddComponent(Args&&... args) noexcept
	{
		auto comp = std::make_shared<T>(std::forward<Args>(args)...);
		comp->mGameObject = this;
		mComponents.push_back(comp);

	}

	void Update(double dt) noexcept
	{
		for (auto& comp : mComponents)
		{
			comp->Update(dt);
		}
	}

};