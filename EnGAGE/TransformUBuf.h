#pragma once

#include "Bindable.h"
#include "UniformBufferObject.h"
#include "Drawable.h"

#include <glm/mat4x4.hpp>


//Default slot 0 to transform cbuf
class TransformUBuf final : public Bindable {

	struct TransformBuf {
		glm::mat4x4 model;
		glm::mat4x4 modelViewProj;
	};

	static std::unique_ptr<UniformBufferObject<TransformBuf>> s_buff;
	const Drawable& mParent;

public:
	TransformUBuf(const Drawable& parent) noexcept :
		mParent(parent)
	{
		if (!s_buff) {
			s_buff = std::make_unique< UniformBufferObject < TransformBuf>>(0, TransformBuf{ glm::mat4(1.0), glm::mat4(1.0f) });
		}
	}

	void Bind() const noexcept override {
		glm::mat4 model_view = Opengl::GetCamera() * mParent.GetTransform();
		s_buff->Update({ model_view, Opengl::GetProjection() * model_view });
		s_buff->Bind();
	}
};

std::unique_ptr< UniformBufferObject<TransformUBuf::TransformBuf>> TransformUBuf::s_buff;