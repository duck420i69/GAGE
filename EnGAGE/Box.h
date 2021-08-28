#pragma once

#include "Drawable.h"

#include "VertexBufferObject.h"
#include "VertexLayout.h"
#include "Shader.h"

class Box final : public Drawable {
public:
	Box() {

		struct Vertex {
			float x, y, z;
		};

		std::vector<Vertex> vertices = { 
			{-0.5f, 0.5f, 0.0f},
			{-0.5f, -0.5f, 0.0f},
			{0.5f, -0.5f, 0.0f}
		};
		std::vector<unsigned int> indices = {
			0, 1, 2
		};

		std::vector<VertexLayout::Layout> layout = {
			{0, 3, sizeof(Vertex), 0}
		};

		this->AddBind(std::make_unique<VertexBufferObject>(vertices));
		this->AddBind(std::make_unique<VertexLayout>(layout));
		this->AddBind(std::make_unique<Shader>("Assets/Shaders/default"));
		this->AddIndexBuffer(std::make_unique<IndexBufferObject>(indices));
	}

	void Update(float dt) noexcept override {};
};