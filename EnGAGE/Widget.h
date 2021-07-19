#pragma once

class Widget
{
public:
	static void Init() noexcept;
	static void Destroy() noexcept;

	static void Prepare() noexcept;
	static void Render() noexcept;
};