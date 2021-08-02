#pragma once


class Scene
{
public:
	virtual ~Scene() = default;

	virtual void Update(float delta) noexcept {};
	virtual void Render() noexcept {};
	virtual void ImGui() noexcept {};
};