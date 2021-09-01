#pragma once

#include "Bindable.h"
#include "UniformBufferObject.h"
#include "Drawable.h"

#include <glm/mat4x4.hpp>


//Default slot 0 to transform cbuf
class TransformUBuf final : public Bindable {
	static std::unique_ptr<UniformBufferObject<glm::mat4x4>> s_buff;
	const Drawable& mParent;

public:
	TransformUBuf(const Drawable& parent) noexcept :
		mParent(parent)
	{
		if (!s_buff) {
			s_buff = std::make_unique< UniformBufferObject < glm::mat4x4>>(0, glm::mat4(1.0));
		}
	}

	void Bind() const noexcept override {
		s_buff->Update(Opengl::GetProjection() * Opengl::GetCamera() * mParent.GetTransform());
		s_buff->Bind();
	}
};

std::unique_ptr< UniformBufferObject<glm::mat4x4>> TransformUBuf::s_buff;