#pragma once

#include <glm/mat4x4.hpp>

#include <vector>
#include <memory>

#include "Bindable.h"
#include "Opengl.h"


class Drawable {
	std::vector<std::shared_ptr<Bindable>> mBinds;
protected:
	const int mVertexCount = 0;
public:
	Drawable(const int vertex_count) noexcept : mVertexCount(vertex_count) {};
	Drawable(const Drawable&) = delete;
	virtual ~Drawable() = default;

	void Draw() const noexcept {
		for (auto& b : mBinds) {
			b->Bind();
		}
		Opengl::DrawIndexed(mVertexCount);
	}

	void AddBind(std::shared_ptr<Bindable> bind) noexcept {
		mBinds.push_back(std::move(bind));
	}

	virtual glm::mat4x4 GetTransform() const noexcept = 0;
};