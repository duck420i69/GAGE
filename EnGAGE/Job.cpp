#include "pch.h"
#include "Job.h"

#include "Drawable.h"
#include "Opengl.h"

void Job::Execute() const noexcept
{
	mDrawable.Bind();
	mStep.Bind();
	Opengl::DrawIndexed(mDrawable.GetVertexCount());
}
