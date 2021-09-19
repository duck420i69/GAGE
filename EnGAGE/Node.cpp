#include "pch.h"
#include "Node.h"

#include "Mesh.h"
#include "ModelProbe.h"

#include <imgui.h>

Node::Node(const std::string& name, const int id, std::vector<Mesh*> meshes, glm::mat4& transform) noexcept :
	name(name), id(id), meshes(std::move(meshes)), base_transform(transform), applied_transform(1.0f) {}

/*void Node::Draw(glm::mat4 accumulated_transform) const noexcept
{
	const auto built = accumulated_transform * base_transform * applied_transform;
	for (const auto& mesh : meshes) {
		mesh->Draw(built);
	}
	for (const auto& child : childs) {
		child->Draw(built);
	}
}*/

void Node::RenderTree(int& node_index, std::optional<int>& selected_index, Node*& p_selected_node) noexcept
{

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

void Node::Accept(ModelProbe& probe) noexcept
{
	if (probe.PushNode(*this)) {
		for (const auto& child : childs) {
			child->Accept(probe);
		}
		probe.PopNode(*this);
	}
}

void Node::Accept(TechniqueProbe& probe) noexcept
{
	for (auto& mesh : meshes) {
		mesh->Accept(probe);
	}
}

void Node::Submit(RenderQueue& queue, glm::mat4 accumulated_transform) const noexcept
{
	auto built = accumulated_transform * base_transform * applied_transform ;
	for (const auto& mesh : meshes) {
		mesh->SetTransform(built);
		mesh->Submit(queue);
	}
	for (const auto& child : childs) {
		child->Submit(queue, built);
	}
}

void Node::AddChild(std::unique_ptr<Node> child) noexcept
{
	assert(child);
	childs.push_back(std::move(child));
}