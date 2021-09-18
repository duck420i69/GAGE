#pragma once

#include "Job.h"

class Pass {
	std::vector<Job> mJobs;
public:
	void Accept(Job job) noexcept;
	void Reset() noexcept;
	void Execute() const noexcept;
};