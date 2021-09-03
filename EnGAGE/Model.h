#pragma once

#include "VertexBufferObject.h"
#include "VertexLayout.h"
#include "TransformUBuf.h"
#include "ShaderObject.h"

#include "OrbitDrawable.h"

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

class Model final : public OrbitDrawable<Model> {
public:
	Model(std::mt19937& rng,
		std::uniform_real_distribution<float>& a,
		std::uniform_real_distribution<float>& d,
		std::uniform_real_distribution<float>& o,
		std::uniform_real_distribution<float>& r,
		const glm::vec3& mat_color) noexcept :
		OrbitDrawable(rng, a, d, o, r)
	{
		if (!IsStaticInited()) {
			struct Vertex {
				glm::vec3 pos;
				glm::vec3 normal;
			};

			Assimp::Importer importer;

			const auto pModel = importer.ReadFile("Assets/Models/suzanne.obj", aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
			const auto pMesh = pModel->mMeshes[0];

			std::vector<Vertex> vertices;
			vertices.reserve(pMesh->mNumVertices);

			for (unsigned int i = 0; i < pMesh->mNumVertices; i++) {
				vertices.push_back({ {pMesh->mVertices[i].x, pMesh->mVertices[i].y, pMesh->mVertices[i].z}, 
					{pMesh->mNormals[i].x, pMesh->mNormals[i].y, pMesh->mNormals[i].z} });
			}

			std::vector<unsigned int> indices;
			indices.reserve((size_t)pMesh->mNumFaces * 3);
			for (unsigned int i = 0; i < pMesh->mNumFaces; i++) {
				const auto& face = pMesh->mFaces[i];
				indices.push_back(face.mIndices[0]);
				indices.push_back(face.mIndices[1]);
				indices.push_back(face.mIndices[2]);
			}

			std::vector<VertexLayout::Layout> layout = {
				{0, 3, sizeof(Vertex), 0},
				{1, 3, sizeof(Vertex), sizeof(float) * 3},
			};

			this->AddStaticBind(std::make_unique<VertexBufferObject>(vertices));
			this->AddStaticBind(std::make_unique<VertexLayout>(layout));
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