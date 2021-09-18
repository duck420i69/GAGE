#pragma once

class Job {
	const class Step& mStep;
	const class Drawable& mDrawable;
public:
	explicit Job(const class Step& step, const class Drawable& drawable) noexcept :
		mStep(step), mDrawable(drawable) {}
	void Execute() const noexcept;
};