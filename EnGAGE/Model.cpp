#include "pch.h"
#include "Model.h"

#include "TextureObject.h"
#include "VertexBufferObject.h"
#include "TransformUBuf.h"
#include "ShaderObject.h"
#include "DynamicVertex.h"
#include "Logger.h"
#include "Rasterizer.h"
#include "DynamicUniform.h"
#include "StencilObject.h"
#include "Material.h"
#include "RenderQueue.h"

#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <imgui.h>



Model::Model(const std::string& file_path, float scale) noexcept
{
	Logger::info("Loading Assimp: {}", file_path);
	Assimp::Importer importer;
	const auto pScene = importer.ReadFile(file_path,
		aiProcess_Triangulate | aiProcess_JoinIdenticalVertices |
		aiProcess_GenNormals | aiProcess_CalcTangentSpace);

	std::vector<Material> materials;
	materials.reserve(pScene->mNumMaterials);
	for (size_t i = 0; i < pScene->mNumMaterials; i++) {
		materials.emplace_back(*pScene->mMaterials[i], file_path);
	}

	for (size_t i = 0; i < pScene->mNumMeshes; i++) {
		const auto& mesh = *pScene->mMeshes[i];
		meshes.push_back(std::make_unique<Mesh>(materials[mesh.mMaterialIndex], mesh, file_path, scale));
	}

	int next_id = 0;
	root = ParseNode(next_id , *pScene->mRootNode);
}



std::unique_ptr<Node> Model::ParseNode(int& next_id, const aiNode& node) noexcept
{
	auto transform = glm::make_mat4<float>(node.mTransformation[0]);

	std::vector<Mesh*> mesh_ptrs;
	mesh_ptrs.reserve(node.mNumMeshes);

	for (size_t i = 0; i < node.mNumMeshes; i++) {
		const auto mesh_idx = node.mMeshes[i];
		mesh_ptrs.push_back(meshes[mesh_idx].get());
	}

	std::unique_ptr<Node> pNode = std::make_unique<Node>(node.mName.C_Str(), next_id++, std::move(mesh_ptrs), transform);
	for (size_t i = 0; i < node.mNumChildren; i++) {
		pNode->AddChild(ParseNode(next_id , *node.mChildren[i]));
	}
	return pNode;
}

void Model::Accept(ModelProbe& probe)
{
	root->Accept(probe);
}

void Model::Submit(RenderQueue& queue) const noexcept
{
	root->Submit(queue, glm::mat4(1.0f));
}

