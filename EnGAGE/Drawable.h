#pragma once

#include <vector>
#include <memory>

#include "Bindable.h"
#include "IndexBufferObject.h"
#include "Opengl.h"

class Drawable {
	std::vector<std::unique_ptr<Bindable>> mBinds;
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

	void AddIndexBuffer(std::unique_ptr<IndexBufferObject> ibuf) noexcept {
		mIndexBuffer = ibuf.get();
		mBinds.push_back(std::move(ibuf));
	}

	void AddBind(std::unique_ptr<Bindable> bind) noexcept {
		mBinds.push_back(std::move(bind));
	}

	virtual void Update(float dt) noexcept = 0;
};