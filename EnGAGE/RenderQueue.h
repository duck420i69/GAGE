#pragma once

#include "Job.h"
#include "Pass.h"

class RenderQueue {
	std::array<Pass, 3> mPasses;
public:
	void Accept(Job job, size_t target) noexcept;
	void Execute() const noexcept;
	void Reset() noexcept;
};