#pragma once

#include <chrono>

class Clock
{
	std::chrono::high_resolution_clock::time_point mStartTime, mPrevTime;
	float mDelta;
public:
	Clock()		noexcept;

	void tick() noexcept;

	inline const float& getDelta() const noexcept { return mDelta; }
};