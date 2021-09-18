#include "pch.h"
#include "StencilObject.h"

#include <glad/glad.h>

StencilObject::StencilObject(Mode mode) noexcept :
	mode(mode)
{
}

void StencilObject::Bind() const noexcept
{
	switch (mode) {
	case Off:
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);
		break;
	case Write:
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 1, 0xff);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		break;
	case Mask:
		glEnable(GL_STENCIL_TEST);
		glDisable(GL_DEPTH_TEST);
		glStencilFunc(GL_NOTEQUAL, 1, 0xff);
		break;
	}
}
