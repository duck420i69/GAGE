#pragma once

#include "VertexBufferObject.h"
#include "VertexLayoutObject.h"
#include "TransformUBuf.h"
#include "ShaderObject.h"
#include "DynamicVertex.h"

#include "OrbitDrawable.h"

#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

class Mesh : public DrawableBase<Mesh> {
	mutable glm::mat4x4 transform;
public:
	Mesh(std::vector<std::unique_ptr<Bindable>> bindptrs) noexcept {

		for (auto& pb : bindptrs) {
			if (auto pi = dynamic_cast<IndexBufferObject*>(pb.get())) {
				AddIndexBuffer(std::unique_ptr<IndexBufferObject>{pi});
				pb.release();
			}
			else {
				AddBind(std::move(pb));
			}
		}
		AddBind(std::make_unique<TransformUBuf>(*this));
	}
	inline void Draw(glm::mat4x4 accumulated_transform) const noexcept {
		transform = accumulated_transform;
		Drawable::Draw();
	}
	inline void Update(float dt) noexcept override {}
	inline glm::mat4x4 GetTransform() const noexcept override {
		return transform;
	}
};

class Node {
	friend class Model;
	std::vector<std::unique_ptr<Node>> childs;
	std::vector<Mesh*> meshes;
	glm::mat4x4 transform;
public:
	Node(std::vector<Mesh*> meshes, glm::mat4& transform) noexcept :
		meshes(std::move(meshes)), transform(transform) {}

	void Draw(glm::mat4 accumulated_transform) const noexcept {
		const auto built = accumulated_transform * transform;
		for (const auto& mesh : meshes) {
			mesh->Draw(built);
		}
		for (const auto& child : childs) {
			child->Draw(built);
		}
	}
private:
	void AddChild(std::unique_ptr<Node> child) noexcept {
		assert(child);
		childs.push_back(std::move(child));
	}
};

class Model {
	std::unique_ptr<Node> root;
	std::vector<std::unique_ptr<Mesh>> meshes;
public:
	Model(const std::string& file_name) noexcept
	{
		Assimp::Importer importer;
		const auto pScene = importer.ReadFile(file_name, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

		for (size_t i = 0; i < pScene->mNumMeshes; i++) {
			meshes.push_back(ParseMesh(*pScene->mMeshes[i]));
		}
		root = ParseNode(*pScene->mRootNode);
	}

	void Draw(const glm::mat4& transform) const noexcept {
		root->Draw(transform);
	}

	std::unique_ptr<Node> ParseNode(const aiNode& node) noexcept {
		auto transform = glm::make_mat4<float>(node.mTransformation[0]);

		std::vector<Mesh*> mesh_ptrs;
		mesh_ptrs.reserve(node.mNumMeshes);

		for (size_t i = 0; i < node.mNumMeshes; i++) {
			const auto mesh_idx = node.mMeshes[i];
			mesh_ptrs.push_back(meshes[mesh_idx].get());
		}

		std::unique_ptr<Node> pNode = std::make_unique<Node>(std::move(mesh_ptrs), transform);
		for (size_t i = 0; i < node.mNumChildren; i++) {
			pNode->AddChild(ParseNode(*node.mChildren[i]));
		}
		return pNode;
	}

	static std::unique_ptr<Mesh> ParseMesh(const aiMesh& mesh) noexcept {
		namespace dv = DynamicVertex;

		dv::VertexLayout layout;
		layout.Append(dv::VertexLayout::Type::Position3D).Append(dv::VertexLayout::Type::Normal);
		dv::VertexBuffer buf(layout);

		for (unsigned int i = 0; i < mesh.mNumVertices; i++) {
			buf.EmplaceBack(glm::vec3{ mesh.mVertices[i].x, mesh.mVertices[i].y, mesh.mVertices[i].z },
				glm::vec3{ mesh.mNormals[i].x, mesh.mNormals[i].y, mesh.mNormals[i].z });
		}

		std::vector<unsigned int> indices;
		indices.reserve((size_t)mesh.mNumFaces * 3);
		for (unsigned int i = 0; i < mesh.mNumFaces; i++) {
			const auto& face = mesh.mFaces[i];
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		std::vector<std::unique_ptr<Bindable>> bindables;
		bindables.push_back(std::make_unique<VertexBufferObject>(buf));
		bindables.push_back(std::make_unique<VertexLayoutObject>(layout));
		bindables.push_back(std::make_unique<ShaderObject>("Assets/Shaders/phong"));
		bindables.push_back(std::make_unique<IndexBufferObject>(indices));

		//Material per instance
		struct MaterialUBuf {
			alignas(16) glm::vec3 color;
			float specular_intensity;
			int specular_power;
			float padding[2];
		} material;
		material.color = {0.6f, 0.6f, 0.8f};
		material.specular_intensity = 0.6f;
		material.specular_power = 30;
		bindables.push_back(std::make_unique<UniformBufferObject<MaterialUBuf>>(2, material));

		return std::make_unique<Mesh>(std::move(bindables));
	}
};