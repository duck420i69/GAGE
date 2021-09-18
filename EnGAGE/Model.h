#pragma once
#include "Drawable.h"

class Mesh : public Drawable {
	mutable glm::mat4x4 transform;
public:
	Mesh(std::vector<std::shared_ptr<Bindable>> bindptrs, const int vertex_count) noexcept;
	void Draw(glm::mat4x4 accumulated_transform) const noexcept;
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
		float scale = 1.0f;
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
	std::vector<std::shared_ptr<Bindable>> outline_bindables;
public:
	Model(const std::string& file_path) noexcept;
	void RenderTree(const char* window_name = "Model") noexcept;
	void Draw() const noexcept;
	std::unique_ptr<Node> ParseNode(const struct aiNode& node) noexcept;

	static std::unique_ptr<Mesh> ParseMesh(const struct aiMesh& mesh, const struct aiMaterial* const* ppMaterial,
		const std::filesystem::path& full_path) noexcept;
};