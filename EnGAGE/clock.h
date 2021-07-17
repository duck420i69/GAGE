#pragma once


class TClock
{
	float mPrevTime, mDelta;
public:
	~TClock() noexcept = default;
	TClock(const TClock& other) noexcept = delete;
	TClock& operator=(const TClock& other) noexcept = delete;

	void init() noexcept;
	void tick() noexcept;

	inline float getDelta() const noexcept { return mDelta; }
private:
	TClock() noexcept;
public:
	static TClock& get() noexcept;
};

static TClock& Clock = TClock::get();