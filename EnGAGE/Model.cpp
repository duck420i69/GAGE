#include "pch.h"
#include "Model.h"

#include "TextureObject.h"
#include "VertexBufferObject.h"
#include "VertexLayoutObject.h"
#include "TransformUBuf.h"
#include "ShaderObject.h"
#include "DynamicVertex.h"
#include "Sampler.h"

#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <imgui.h>

Mesh::Mesh(std::vector<std::shared_ptr<Bindable>> bindptrs) noexcept
{

	for (auto& pb : bindptrs) {
		AddBind(std::move(pb));
	}
	AddBind(std::make_shared<TransformUBuf>(*this));
}

void Mesh::Draw(glm::mat4x4 accumulated_transform) const noexcept
{
	transform = accumulated_transform;
	Drawable::Draw();
}

Node::Node(const std::string& name, std::vector<Mesh*> meshes, glm::mat4& transform) noexcept :
	name(name), meshes(std::move(meshes)), base_transform(transform), applied_transform(1.0f) {}

void Node::Draw(glm::mat4 accumulated_transform) const noexcept
{
	const auto built = accumulated_transform * base_transform * applied_transform;
	for (const auto& mesh : meshes) {
		mesh->Draw(built);
	}
	for (const auto& child : childs) {
		child->Draw(built);
	}
}

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

void Node::AddChild(std::unique_ptr<Node> child) noexcept
{
	assert(child);
	childs.push_back(std::move(child));
}

void ModelWindow::RenderTree(Node& root, const char* window_name) noexcept
{
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

glm::mat4x4 ModelWindow::GetTransform() const noexcept
{
	glm::mat4 model(1.0f);
	const TransformParams& transform = transforms.at(*selected_index);
	model = glm::translate(glm::mat4(1.0f), { transform.x, transform.y, transform.z });
	model = glm::rotate(model, glm::radians(transform.pitch), { 1, 0, 0 });
	model = glm::rotate(model, glm::radians(transform.yaw), { 0, 1, 0 });
	model = glm::rotate(model, glm::radians(transform.roll), { 0, 0, 1 });
	return model;
}

Model::Model(const std::string& file_name) noexcept :
	model_window(std::make_unique<ModelWindow>())
{
	Assimp::Importer importer;
	const auto pScene = importer.ReadFile(file_name, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

	for (size_t i = 0; i < pScene->mNumMeshes; i++) {
		meshes.push_back(ParseMesh(*pScene->mMeshes[i], pScene->mMaterials));
	}
	root = ParseNode(*pScene->mRootNode);
}

void Model::RenderTree(const char* window_name) noexcept
{
	model_window->RenderTree(*root, window_name);
}

void Model::Draw() const noexcept
{

	if (auto node = model_window->GetSelectedNode())
	{
		node->SetAppliedTransform(model_window->GetTransform());
	}
	root->Draw(glm::mat4(1.0f));
}

std::unique_ptr<Node> Model::ParseNode(const aiNode& node) noexcept
{
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

std::unique_ptr<Mesh> Model::ParseMesh(const aiMesh& mesh, const aiMaterial* const* ppMaterial) noexcept
{
	namespace dv = DynamicVertex;

	std::vector<std::shared_ptr<Bindable>> bindables;
	dv::VertexLayout layout;
	layout.Append(dv::VertexLayout::Type::Position3D).Append(dv::VertexLayout::Type::Normal).Append(dv::VertexLayout::Type::Texture2D);
	dv::VertexBuffer buf(layout);

	const aiMaterial& mat = *ppMaterial[mesh.mMaterialIndex];


	//Vertices
	for (unsigned int i = 0; i < mesh.mNumVertices; i++) {
		buf.EmplaceBack(
			glm::vec3{ mesh.mVertices[i].x, mesh.mVertices[i].y, mesh.mVertices[i].z },
			glm::vec3{ mesh.mNormals[i].x, mesh.mNormals[i].y, mesh.mNormals[i].z }, 
			glm::vec2{ mesh.mTextureCoords[0][i].x, mesh.mTextureCoords[0][i].y });
	}

	//Indices
	std::vector<unsigned int> indices;
	indices.reserve((size_t)mesh.mNumFaces * 3);
	for (unsigned int i = 0; i < mesh.mNumFaces; i++) {
		const auto& face = mesh.mFaces[i];
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}

	//Material
	bool has_specular_map = false;
	float shininess = 35.0f;
	if (mesh.mMaterialIndex >= 0) {
		const aiMaterial& mat = *ppMaterial[mesh.mMaterialIndex];
		aiString texture_file_name;
		mat.GetTexture(aiTextureType_DIFFUSE, 0, &texture_file_name);
		bindables.push_back(BindableCodex::Resolve<TextureObject>( "Assets/Models/nano_textured/" + std::string(texture_file_name.C_Str()), 0));
		bindables.push_back(BindableCodex::Resolve<Sampler>(Opengl::TextureFilter::NEAREST, Opengl::TextureFilter::NEAREST, Opengl::TextureWrap::REPEAT));


		if (mat.GetTexture(aiTextureType_SPECULAR, 0, &texture_file_name) == aiReturn_SUCCESS) {
			bindables.push_back(BindableCodex::Resolve<TextureObject>("Assets/Models/nano_textured/" + std::string(texture_file_name.C_Str()), 1));
			bindables.push_back(BindableCodex::Resolve<Sampler>(Opengl::TextureFilter::NEAREST, Opengl::TextureFilter::NEAREST, Opengl::TextureWrap::REPEAT));
			has_specular_map = true;
		}
		else {
			mat.Get(AI_MATKEY_SHININESS, shininess);
		}
	}

	
	bindables.push_back(BindableCodex::Resolve<VertexBufferObject>(mesh.mName.C_Str(), buf));
	bindables.push_back(BindableCodex::Resolve<VertexLayoutObject>(layout));
	std::shared_ptr<ShaderObject> shader;
	
	if (has_specular_map) {
		shader = BindableCodex::Resolve < ShaderObject>("Assets/Shaders/phong_VS.glsl", "Assets/Shaders/phong_specmap_FS.glsl");
	}
	else {
		shader = BindableCodex::Resolve < ShaderObject>("Assets/Shaders/phong_VS.glsl", "Assets/Shaders/phong_FS.glsl");
	}
	shader->LoadTextureSlot("uDiffuse", 0);
	shader->LoadTextureSlot("uSpecular", 1);

	bindables.push_back(std::move(shader));
	bindables.push_back(BindableCodex::Resolve <IndexBufferObject>(mesh.mName.C_Str(), indices));

	//Material per instance
	struct MaterialUBuf {
		float specular_intensity;
		int specular_power;
		float padding[2];
	} material;
	material.specular_intensity = 0.8f;
	material.specular_power = shininess;
	auto mat_ubuf = std::make_shared<UniformBufferObject<MaterialUBuf>>(UniformBufferObject<MaterialUBuf>(2));
	mat_ubuf->Update(material);
	bindables.push_back(mat_ubuf);

	return std::make_unique<Mesh>(std::move(bindables));
}