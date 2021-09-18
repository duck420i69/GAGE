#include "pch.h"
#include "RenderQueue.h"

#include "StencilObject.h"

void RenderQueue::Accept(Job job, size_t target) noexcept
{
	mPasses[target].Accept(job);
}

void RenderQueue::Execute() const noexcept
{

	StencilObject(StencilObject::Off).Bind();
	mPasses[0].Execute();
	StencilObject(StencilObject::Write).Bind();
	mPasses[1].Execute();
	StencilObject(StencilObject::Mask).Bind();
	mPasses[2].Execute();
	
}

void RenderQueue::Reset() noexcept
{
	std::for_each(mPasses.begin(), mPasses.end(), [](Pass& pass) {
		pass.Reset();
	});
}
