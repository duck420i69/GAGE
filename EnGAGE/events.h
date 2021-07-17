#pragma once


class TEvents
{
public:
	~TEvents() noexcept = default;
	TEvents(const TEvents& other) noexcept = delete;
	TEvents& operator=(const TEvents& other) noexcept = delete;

	void init() noexcept;
	void pollEvents() noexcept;

private:
	TEvents() noexcept = default;
public:
	static TEvents& get() noexcept;
};

static TEvents& Events = TEvents::get();