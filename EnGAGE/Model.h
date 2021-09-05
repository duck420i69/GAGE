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
	Mesh(std::vector<std::unique_ptr<Bindable>> bindptrs) noexcept;
	void Draw(glm::mat4x4 accumulated_transform) const noexcept;
	void Update(float dt) noexcept override {}
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
	Node(const std::string& name, std::vector<Mesh*> meshes, glm::mat4& transform) noexcept;

	void Draw(glm::mat4 accumulated_transform) const noexcept;
	void RenderTree(int& node_index, std::optional<int>& selected_index, Node*& p_selected_node) noexcept;
	inline void SetAppliedTransform(const glm::mat4x4& trans) noexcept { applied_transform = trans; }
private:
	void AddChild(std::unique_ptr<Node> child) noexcept;
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
	void RenderTree(Node& root, const char* window_name) noexcept;
	glm::mat4x4 GetTransform() const noexcept;
	inline Node* GetSelectedNode() noexcept { return p_selected_node; }
};

class Model {
	std::unique_ptr<Node> root;
	std::vector<std::unique_ptr<Mesh>> meshes;
	std::unique_ptr<ModelWindow> model_window;

public:
	Model(const std::string& file_name) noexcept;
	void RenderTree(const char* window_name = "Model") noexcept;
	void Draw() const noexcept;
	std::unique_ptr<Node> ParseNode(const aiNode& node) noexcept;

	static std::unique_ptr<Mesh> ParseMesh(const aiMesh& mesh) noexcept;
};