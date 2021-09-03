#pragma once

#include <glm/mat4x4.hpp>

#include <vector>
#include <memory>

#include "Bindable.h"
#include "IndexBufferObject.h"
#include "Opengl.h"


class Drawable {
	template<class T>
	friend class DrawableBase;

	std::vector<std::unique_ptr<Bindable>> mBinds;
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
		auto& static_binds = GetStaticBind();
		for (auto& b : static_binds) {
			b->Bind();
		}
		Opengl::DrawIndexed(mIndexBuffer->GetCount());
	}
	virtual void Update(float dt) noexcept = 0;
	virtual glm::mat4x4 GetTransform() const noexcept = 0;

protected:
	void AddIndexBuffer(std::unique_ptr<IndexBufferObject> ibuf) noexcept {
		mIndexBuffer = ibuf.get();
		mBinds.push_back(std::move(ibuf));
	}

	void AddBind(std::unique_ptr<Bindable> bind) noexcept {
		mBinds.push_back(std::move(bind));
	}

private:
	virtual const std::vector<std::unique_ptr<Bindable>>& GetStaticBind() const noexcept = 0;
};