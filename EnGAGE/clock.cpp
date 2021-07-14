#include "pch.h"
#include "clock.h"

using namespace std::chrono_literals;
using namespace std::chrono;
using namespace std;

Clock::Clock() noexcept :
	mStartTime(high_resolution_clock::now()),
	mPrevTime(mStartTime),
	mDelta(0.0f)
{
}

void Clock::tick() noexcept
{
	mStartTime = high_resolution_clock::now();
	duration<float, milli> ms = mStartTime - mPrevTime;
	mPrevTime = mStartTime;
	mDelta = ms.count();

	if (mDelta > 20) mDelta = 20;
}
