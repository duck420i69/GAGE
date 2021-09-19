#pragma once
#include "Drawable.h"

#include "Node.h"
#include "Mesh.h"


class Model {
	std::unique_ptr<Node> root;
	std::vector<std::unique_ptr<Mesh>> meshes;
	std::vector<std::shared_ptr<Bindable>> outline_bindables;
public:
	Model(const std::string& file_path, float scale = 1.0f) noexcept;
	//void RenderTree(const char* window_name = "Model") noexcept;
	//void Draw() const noexcept;

	void Accept(class ModelProbe& probe);
	void Submit(class RenderQueue& queue) const noexcept;
	std::unique_ptr<Node> ParseNode(int& next_id, const struct aiNode& node) noexcept;
};