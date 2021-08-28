#pragma once


class Scene
{
public:
	virtual ~Scene() = default;

	virtual void Update(float delta) noexcept {};
	virtual void Render() noexcept {};
	virtual void ImGui() noexcept {};

	virtual const char* GetName() const noexcept = 0;
};