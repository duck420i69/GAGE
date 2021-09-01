#pragma once

#include "DrawableBase.h"

#include "VertexBufferObject.h"
#include "VertexLayout.h"
#include "TransformUBuf.h"
#include "ShaderObject.h"

#include <glm/gtc/matrix_transform.hpp>

class Box final : public DrawableBase<Box> {
	float pitch = 0, yaw = 0, roll = 0;
	float pitch2 = 0, yaw2 = 0, roll2 = 0;
	float r;
	float dpitch, dyaw, droll;
	float dpitch2, dyaw2, droll2;

public:
	Box(std::mt19937& rng,
		std::uniform_real_distribution<float>& a,
		std::uniform_real_distribution<float>& d, 
		std::uniform_real_distribution<float>& o, 
		std::uniform_real_distribution<float>& r ) noexcept :
		dpitch(a(rng)), dyaw(a(rng)), droll(a(rng)),
		dpitch2(d(rng)), dyaw2(d(rng)), droll2(d(rng)),
		r(r(rng))
	{
		if (!IsStaticInited()) {
			struct Vertex {
				float x, y, z;
				float r, g, b;
			};

			std::vector<Vertex> vertices = {
				{ -1.0f,-1.0f,-1.0f, 1, 0, 0 },
				{ 1.0f,-1.0f,-1.0f, 0, 1, 0},
				{ -1.0f,1.0f,-1.0f ,0, 0, 1},
				{ 1.0f,1.0f,-1.0f ,1, 1, 0},
				{ -1.0f,-1.0f,1.0f,0, 1, 1 },
				{ 1.0f,-1.0f,1.0f,1, 0, 1 },
				{ -1.0f,1.0f,1.0f, 1, 0, 0 },
				{ 1.0f,1.0f,1.0f,0, 0, 0 },
			};
			std::vector<unsigned int> indices = {
				0,2,1, 2,3,1,
				1,3,5, 3,7,5,
				2,6,3, 3,6,7,
				4,5,7, 4,7,6,
				0,4,2, 2,4,6,
				0,1,4, 1,5,4
			};

			std::vector<VertexLayout::Layout> layout = {
				{0, 3, sizeof(Vertex), 0},
				{1, 3, sizeof(Vertex), sizeof(float) * 3},
			};

			this->AddStaticBind(std::make_unique<VertexBufferObject>(vertices));
			this->AddStaticBind(std::make_unique<VertexLayout>(layout));
			this->AddStaticBind(std::make_unique<ShaderObject>("Assets/Shaders/default"));
			this->AddStaticIndexBuffer(std::make_unique<IndexBufferObject>(indices));
		}
		else {
			this->SetIndexFromStatic();
			this->AddBind(std::make_unique<TransformUBuf>(*this));
		}
	}

	inline void Update(float dt) noexcept override {
		pitch += dpitch * dt;
		pitch2 += dpitch2 * dt;
		yaw += dyaw * dt;
		yaw2 += dyaw2 * dt;
		roll += droll * dt;
		roll2 += droll2 * dt;
	};

	inline glm::mat4x4 GetTransform() const noexcept override {
		glm::mat4x4 result (1.0f);

		result = glm::rotate(result, roll2, { 0, 0, 1 });
		result = glm::rotate(result, yaw2, { 0, 1, 0 });
		result = glm::rotate(result, pitch2, { 1, 0, 0 });

		result = glm::translate(result, { r, 0, 0 });

		result = glm::rotate(result, roll, { 0, 0, 1 });
		result = glm::rotate(result, yaw, { 0, 1, 0 });
		result = glm::rotate(result, pitch, { 1, 0, 0 });


		return result;
	};
};