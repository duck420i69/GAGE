#pragma once

#include "VertexBufferObject.h"
#include "VertexLayoutObject.h"
#include "TransformUBuf.h"
#include "ShaderObject.h"
#include "DynamicVertex.h"
#include "DrawableBase.h"

#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <imgui.h>

#include <optional>

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
	friend class ModelWindow;
	std::string name;
	std::vector<std::unique_ptr<Node>> childs;
	std::vector<Mesh*> meshes;
	glm::mat4x4 base_transform, applied_transform;
public:
	Node(const std::string& name, std::vector<Mesh*> meshes, glm::mat4& transform) noexcept :
		name(name), meshes(std::move(meshes)), base_transform(transform), applied_transform(1.0f) {}

	void Draw(glm::mat4 accumulated_transform) const noexcept {
		const auto built = accumulated_transform * base_transform * applied_transform;
		for (const auto& mesh : meshes) {
			mesh->Draw(built);
		}
		for (const auto& child : childs) {
			child->Draw(built);
		}
	}

	void RenderTree(int& node_index, std::optional<int>& selected_index, Node*& p_selected_node) noexcept {

		const int current_node_index = node_index;
		node_index++;

		int flags = ImGuiTreeNodeFlags_OpenOnArrow;
		if (current_node_index == selected_index.value_or(-1)) {
			flags |= ImGuiTreeNodeFlags_Selected;
		}
		if (childs.empty()) {
			flags |= ImGuiTreeNodeFlags_Leaf;
		}
		if (ImGui::TreeNodeEx((void*)(intptr_t)current_node_index, flags, name.c_str())) {
			if (ImGui::IsItemClicked()) {
				selected_index = current_node_index;
				p_selected_node = const_cast<Node*>(this);
			}
			for (const auto& pChild : childs) {
				pChild->RenderTree(node_index, selected_index, p_selected_node);
			}
			ImGui::TreePop();
		}
	}

	inline void SetAppliedTransform(const glm::mat4x4& trans) noexcept { applied_transform = trans; }
private:
	void AddChild(std::unique_ptr<Node> child) noexcept {
		assert(child);
		childs.push_back(std::move(child));
	}
};

class ModelWindow {
	struct TransformParams {
		float roll = 0.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	};
	Node* p_selected_node = nullptr;
	std::optional<int> selected_index;
	std::unordered_map<int, TransformParams> transforms;
public:
	void RenderTree(Node& root, const char* window_name) noexcept {
		int index_tracker = 0;

		ImGui::Begin(window_name);
		ImGui::Columns(2, nullptr, true);
		root.RenderTree(index_tracker, selected_index, p_selected_node);

		ImGui::NextColumn();
		if (p_selected_node != nullptr) {
			TransformParams& transform = transforms[*selected_index];
			ImGui::DragFloat3("Rotation", &transform.roll);
			ImGui::DragFloat3("Position", &transform.x);
		}
		ImGui::End();
	}
	glm::mat4x4 GetTransform() const noexcept {
		glm::mat4 model(1.0f);
		const TransformParams& transform = transforms.at(*selected_index);
		model = glm::translate(glm::mat4(1.0f), { transform.x, transform.y, transform.z });
		model = glm::rotate(model, glm::radians(transform.pitch), { 1, 0, 0 });
		model = glm::rotate(model, glm::radians(transform.yaw), { 0, 1, 0 });
		model = glm::rotate(model, glm::radians(transform.roll), { 0, 0, 1 });
		return model;
	}
	inline Node* GetSelectedNode() noexcept { return p_selected_node; }
};

class Model {
	std::unique_ptr<Node> root;
	std::vector<std::unique_ptr<Mesh>> meshes;
	std::unique_ptr<ModelWindow> model_window;

public:
	Model(const std::string& file_name) noexcept :
		model_window(std::make_unique<ModelWindow>())
	{
		Assimp::Importer importer;
		const auto pScene = importer.ReadFile(file_name, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

		for (size_t i = 0; i < pScene->mNumMeshes; i++) {
			meshes.push_back(ParseMesh(*pScene->mMeshes[i]));
		}
		root = ParseNode(*pScene->mRootNode);
	}

	void RenderTree(const char* window_name = "Model") noexcept {
		model_window->RenderTree(*root, window_name);
	}

	void Draw() const noexcept {

		if (auto node = model_window->GetSelectedNode())
		{
			node->SetAppliedTransform(model_window->GetTransform());
		}
		root->Draw(glm::mat4(1.0f));
	}

	std::unique_ptr<Node> ParseNode(const aiNode& node) noexcept {
		auto transform = glm::make_mat4<float>(node.mTransformation[0]);

		std::vector<Mesh*> mesh_ptrs;
		mesh_ptrs.reserve(node.mNumMeshes);

		for (size_t i = 0; i < node.mNumMeshes; i++) {
			const auto mesh_idx = node.mMeshes[i];
			mesh_ptrs.push_back(meshes[mesh_idx].get());
		}

		std::unique_ptr<Node> pNode = std::make_unique<Node>(node.mName.C_Str(), std::move(mesh_ptrs), transform);
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
		material.color = { 0.6f, 0.6f, 0.8f };
		material.specular_intensity = 0.6f;
		material.specular_power = 30;
		bindables.push_back(std::make_unique<UniformBufferObject<MaterialUBuf>>(2, material));

		return std::make_unique<Mesh>(std::move(bindables));
	}
};