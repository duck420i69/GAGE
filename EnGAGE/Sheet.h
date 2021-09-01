#pragma once

#include "DrawableBase.h"

#include "VertexBufferObject.h"
#include "VertexLayout.h"
#include "TransformUBuf.h"
#include "ShaderObject.h"
#include "Sampler.h"
#include "TextureObject.h"

#include <glm/gtc/matrix_transform.hpp>

class Sheet final : public DrawableBase<Sheet> {
	float pitch = 0, yaw = 0, roll = 0;
	float pitch2 = 0, yaw2 = 0, roll2 = 0;
	float r;
	float dpitch, dyaw, droll;
	float dpitch2, dyaw2, droll2;

public:
	Sheet(std::mt19937& rng,
		std::uniform_real_distribution<float>& a,
		std::uniform_real_distribution<float>& d,
		std::uniform_real_distribution<float>& o,
		std::uniform_real_distribution<float>& r) noexcept :
		dpitch(a(rng)), dyaw(a(rng)), droll(a(rng)),
		dpitch2(d(rng)), dyaw2(d(rng)), droll2(d(rng)),
		r(r(rng))
	{
		if (!IsStaticInited()) {
			struct Vertex {
				float x, y, z;
				float u, v;
			};

			std::vector<Vertex> vertices = {
				{ -1.0f,-1.0f,0,	0, 1},
				{ -1.0f,1.0f, 0,	0, 0 },
				{ 1.0f,1.0f, 0,		1, 0},
				{ 1.0f,-1.0f,  0,	1, 1},
			};
			std::vector<unsigned int> indices = {
				0, 1, 2,
				0, 2, 3
			};

			std::vector<VertexLayout::Layout> layout = {
				{0, 3, sizeof(Vertex), 0},
				{1, 2, sizeof(Vertex), sizeof(float) * 2},
			};

			this->AddStaticBind(std::make_unique<VertexBufferObject>(vertices));
			this->AddStaticBind(std::make_unique<VertexLayout>(layout));
			this->AddStaticBind(std::make_unique<ShaderObject>("Assets/Shaders/uv"));
			this->AddStaticBind(std::make_unique<TextureObject>("Assets/Textures/ignore_this.png"));
			this->AddStaticBind(std::make_unique<Sampler>(Opengl::TextureFilter::NEAREST, Opengl::TextureFilter::NEAREST, Opengl::TextureWrap::REPEAT));
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
		glm::mat4x4 result(1.0f);

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