#pragma once

#include <glm/mat4x4.hpp>

class Node {
	friend class Model;
	friend class ModelWindow;
	std::string name;
	int id;
	std::vector<std::unique_ptr<Node>> childs;
	std::vector<class Mesh*> meshes;
	glm::mat4x4 base_transform, applied_transform;
public:
	Node(const std::string& name, const int id, std::vector<Mesh*> meshes, glm::mat4& transform) noexcept;

	//void Draw(glm::mat4 accumulated_transform) const noexcept;
	void Submit(class RenderQueue& queue, glm::mat4 accumulated_transform) const noexcept;
	void RenderTree(int& node_index, std::optional<int>& selected_index, Node*& p_selected_node) noexcept;
	void Accept(class ModelProbe& probe) noexcept;
	void Accept(class TechniqueProbe& probe) noexcept;

	inline void SetAppliedTransform(const glm::mat4x4& trans) noexcept { applied_transform = trans; }
	inline const glm::mat4x4& GetAppliedTransform() const noexcept { return applied_transform; }

	inline const std::string& GetName() const noexcept { return name; }
	inline const int& GetID() const noexcept { return id; }
	inline bool HasChildren() const noexcept {
		return childs.size() > 0;
	}
private:
	void AddChild(std::unique_ptr<Node> child) noexcept;
};
