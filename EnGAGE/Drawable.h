#pragma once

#include <glm/mat4x4.hpp>

#include <vector>
#include <memory>

#include "Bindable.h"
#include "IndexBufferObject.h"
#include "Opengl.h"


class Drawable {
	std::vector<std::shared_ptr<Bindable>> mBinds;
protected:
	const IndexBufferObject* mIndexBuffer = nullptr;
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	virtual ~Drawable() = default;

	void Draw() const noexcept {
		for (auto& b : mBinds) {
			b->Bind();
		}
		Opengl::DrawIndexed(mIndexBuffer->GetCount());
	}

	void AddBind(std::shared_ptr<Bindable> bind) noexcept {

		if (typeid(*bind) == typeid(IndexBufferObject)) {
			assert(mIndexBuffer == nullptr && "IndexBufferObject already added !");
			mIndexBuffer = &static_cast<IndexBufferObject&>(*bind);
		}

		mBinds.push_back(std::move(bind));
	}

	virtual glm::mat4x4 GetTransform() const noexcept = 0;
};