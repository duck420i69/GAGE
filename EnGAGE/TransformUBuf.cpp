#include "pch.h"
#include "TransformUBuf.h"

std::unique_ptr< UniformBufferObject<TransformUBuf::TransformBuf>> TransformUBuf::s_buff;

void TransformUBuf::Bind() const noexcept
{
	glm::mat4 model_view = Opengl::GetCamera() * mParent.GetTransform();
	s_buff->Update({ model_view, Opengl::GetProjection() * model_view });
	s_buff->Bind();
}
