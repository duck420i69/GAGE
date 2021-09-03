#pragma once


#include "VertexBufferObject.h"
#include "VertexLayoutObject.h"
#include "TransformUBuf.h"
#include "ShaderObject.h"

#include "OrbitDrawable.h"

class Box final : public OrbitDrawable<Box> {
	

public:
	Box(std::mt19937& rng,
		std::uniform_real_distribution<float>& a,
		std::uniform_real_distribution<float>& d, 
		std::uniform_real_distribution<float>& o, 
		std::uniform_real_distribution<float>& r ,
		const glm::vec3& mat_color) noexcept :
		OrbitDrawable(rng, a, d, o, r)
	{
		if (!IsStaticInited()) {
			struct Vertex {			
				struct {
					float x, y, z;
				} pos;
				struct {
					float x, y, z;
				} normal;
			};

			std::vector<Vertex> vertices = {
				{-0.5, -0.5, 0.5, 0, 0, 1},
				{0.5, -0.5, 0.5, 0, 0, 1},
				{-0.5, 0.5, 0.5, 0, 0, 1},
				{0.5, 0.5, 0.5, 0, 0, 1},
				{-0.5, 0.5, 0.5, 0, 1, 0},
				{0.5, 0.5, 0.5, 0, 1, 0},
				{-0.5, 0.5, -0.5, 0, 1, 0},
				{0.5, 0.5, -0.5, 0, 1, 0},
				{-0.5, 0.5, -0.5, 0, 0, -1},
				{0.5, 0.5, -0.5,  0, 0, -1},
				{-0.5, -0.5, -0.5, 0, 0, -1},
				{0.5, -0.5, -0.5,  0, 0, -1},
				{-0.5, -0.5, -0.5, 0, -1, 0,},
				{0.5, -0.5, -0.5, 0, -1, 0},
				{-0.5, -0.5, 0.5, 0, -1, 0},
				{0.5, -0.5, 0.5, 0, -1, 0},
				{0.5, -0.5, 0.5, 1, 0, 0},
				{0.5, -0.5, -0.5, 1, 0, 0},
				{0.5, 0.5, 0.5, 1, 0, 0},
				{0.5, 0.5, -0.5, 1, 0, 0},
				{-0.5, -0.5, -0.5, -1, 0, 0},
				{-0.5, -0.5, 0.5, -1, 0, 0},
				{-0.5, 0.5, -0.5, -1, 0, 0},
				{-0.5, 0.5, 0.5,-1, 0, 0},

			};
			std::vector<unsigned int> indices = {
				0, 1, 2, 2, 1, 3, 4, 5, 6, 6, 5, 7, 8, 9, 10, 10, 9, 11, 12, 13, 14, 14, 13, 15, 16, 17, 18, 18, 17, 19, 20, 21, 22, 22, 21, 23
			};

			std::vector<VertexLayoutObject::Layout> layout = {
				{0, 3, sizeof(Vertex), 0},
				{1, 3, sizeof(Vertex), sizeof(float) * 3},
			};

			this->AddStaticBind(std::make_unique<VertexBufferObject>(vertices));
			this->AddStaticBind(std::make_unique<VertexLayoutObject>(layout));
			this->AddStaticBind(std::make_unique<ShaderObject>("Assets/Shaders/phong"));
			this->AddStaticIndexBuffer(std::make_unique<IndexBufferObject>(indices));
		}
		else {
			this->SetIndexFromStatic();
			this->AddBind(std::make_unique<TransformUBuf>(*this));

			//Material per instance
			struct MaterialUBuf {
				alignas(16) glm::vec3 color;
				float specular_intensity;
				int specular_power;
				float padding[2];
			} material;
			material.color = mat_color;
			material.specular_intensity = 0.6f;
			material.specular_power = 30;
			this->AddBind(std::make_unique<UniformBufferObject<MaterialUBuf>>(2, material));
		}
	}

	
};