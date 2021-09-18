#include "pch.h"
#include "Pass.h"

void Pass::Accept(Job job) noexcept
{
	mJobs.push_back(job);
}

void Pass::Reset() noexcept
{
	mJobs.clear();
}

void Pass::Execute() const noexcept
{
	for (const auto& job : mJobs) {
		job.Execute();
	}
}
