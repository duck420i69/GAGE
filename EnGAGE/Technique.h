#pragma once

#include "Step.h"

class Technique {
	std::vector<Step> mSteps;
	std::string mName;
	bool mActivated;
public:
	Technique(const std::string& name, bool start_state = true) noexcept;
	void Submit(class RenderQueue& queue, const class Drawable& drawable) const noexcept;
	void AddStep(Step step) noexcept;
	void InitParent(const class Drawable& drawable) noexcept;
	void Accept(class TechniqueProbe& probe) noexcept;

	inline const std::string& GetName() const noexcept { return mName; }
	inline bool& GetActiveRef() noexcept { return mActivated; }

};