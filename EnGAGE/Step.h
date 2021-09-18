#pragma once

#include "Bindable.h"

class Step {
	size_t mTarget;
	std::vector<std::shared_ptr<Bindable>> mBindables;
public:
	Step(const size_t target) noexcept : mTarget(target) {}

	void AddBindable(std::shared_ptr<Bindable> bindable) noexcept;
	void Bind() const noexcept;
	void Submit(class RenderQueue& queue, const class Drawable& drawable) const noexcept;
	void InitParent(const class Drawable& drawable) const noexcept;

	void Accept(class TechniqueProbe& probe) noexcept;
};