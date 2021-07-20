#pragma once

class IScene
{
public:
	virtual ~IScene() = default;

	virtual void Update(double delta) noexcept = 0;
	virtual void Render() noexcept = 0;
};