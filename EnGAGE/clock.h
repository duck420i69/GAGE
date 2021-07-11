#pragma once

#include "common.h"
#include <chrono>

class Clock
{
	std::chrono::high_resolution_clock::time_point mStartTime, mPrevTime;
	Real mDelta;
public:
	Clock()		noexcept;

	void tick() noexcept;

	inline Real getDelta() const noexcept { return mDelta; }
};