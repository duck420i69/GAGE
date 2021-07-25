#pragma once

#include "Component.h"
#include "Transform.h"

class GameObject
{
	std::string mName;
	std::vector<std::shared_ptr<Component>> mComponents;
	int			mZIndex;
public:
	Transform mTransform;
public:
	GameObject(const std::string& name, const Transform& transform, int z_index) noexcept :
		mName(name), mComponents(), mZIndex(z_index), mTransform(transform)
	{}

	GameObject(const std::string& name, int z_index = 0) noexcept :
		GameObject(name, Transform(), z_index)
	{}

	template<class T>
	const std::weak_ptr<T> GetComponent() const  noexcept
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
		comp->Start();
		mComponents.push_back(comp);

	}

	void Update(double dt) noexcept
	{
		for (auto& comp : mComponents)
		{
			comp->Update(dt);
		}
	}

	inline const int& ZIndex() const noexcept { return mZIndex; }
};