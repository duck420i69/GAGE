#include "pch.h"
#include "Technique.h"

#include "TechniqueProbe.h"

Technique::Technique(const std::string& name) noexcept :
	mName(name)
{
}

void Technique::Submit(RenderQueue& queue, const Drawable& drawable) const noexcept
{
	if (mActivated) {
		for (const auto& step : mSteps) {
			step.Submit(queue, drawable);
		}
	}
}

void Technique::AddStep(Step step) noexcept
{
	mSteps.push_back(std::move(step));
}

void Technique::InitParent(const Drawable& drawable) noexcept
{
	for (const auto& step : mSteps) {
		step.InitParent(drawable);
	}
}

void Technique::Accept(TechniqueProbe& probe) noexcept
{
	probe.SetTechnique(this);
	for (auto& step : mSteps) {
		step.Accept(probe);
	}
}
