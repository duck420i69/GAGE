#pragma once

#include <glm/mat4x4.hpp>

#include <vector>
#include <memory>

#include "VertexBufferObject.h"
#include "Technique.h"

class Drawable {
protected:
	int mVertexCount = 0;
	std::vector<Technique> mTechniques;
	std::shared_ptr<VertexBufferObject> mVertexData;
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	virtual ~Drawable();

	void Submit(class RenderQueue& queue) const noexcept;
	void Bind() const noexcept;
	void Accept(class TechniqueProbe& probe) noexcept;

	virtual glm::mat4x4 GetTransform() const noexcept = 0;
	inline const int& GetVertexCount() const noexcept { return mVertexCount; }
protected:
	inline void SetVertexCount(int vertex_count) noexcept { mVertexCount = vertex_count; }

	void AddTechnique(Technique technique) noexcept;
};