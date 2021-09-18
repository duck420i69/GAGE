#include "pch.h"
#include "Step.h"

#include "Job.h"
#include "RenderQueue.h"
#include "TechniqueProbe.h"

void Step::AddBindable(std::shared_ptr<Bindable> bindable) noexcept
{
	mBindables.push_back(std::move(bindable));
}

void Step::Bind() const noexcept
{
	for (const auto& bind : mBindables) {
		bind->Bind();
	}
}

void Step::Submit(RenderQueue& queue, const Drawable& drawable) const noexcept
{
	queue.Accept(Job( *this, drawable ), mTarget);
}

void Step::InitParent(const Drawable& drawable) const noexcept
{
	for (const auto& bind : mBindables) {
		bind->InitParent(drawable);
	}
}

void Step::Accept(TechniqueProbe& probe) noexcept
{
	probe.SetStep(this);

	for (auto& bind : mBindables) {
		bind->Accept(probe);
	}
}
