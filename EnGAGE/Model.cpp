#include "pch.h"
#include "Model.h"

#include "TextureObject.h"
#include "VertexBufferObject.h"
#include "TransformUBuf.h"
#include "ShaderObject.h"
#include "DynamicVertex.h"
#include "Logger.h"
#include "Rasterizer.h"

#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <imgui.h>

Mesh::Mesh(std::vector<std::shared_ptr<Bindable>> bindptrs, const int vertex_count) noexcept
{

	for (auto& pb : bindptrs) {
		AddBind(std::move(pb));
	}
	AddBind(std::make_shared<TransformUBuf>(*this));
	this->SetVertexCount(vertex_count);
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
		ImGui::DragFloat("Scale", &transform.scale, 0.01f);
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
	model = glm::scale(model, { transform.scale, transform.scale, transform.scale });
	return model;
}

Model::Model(const std::string& file_path) noexcept :
	model_window(std::make_unique<ModelWindow>())
{
	Logger::info("Loading Assimp: {}", file_path);
	Assimp::Importer importer;
	const auto pScene = importer.ReadFile(file_path,
		aiProcess_Triangulate | aiProcess_JoinIdenticalVertices |
		aiProcess_GenNormals | aiProcess_CalcTangentSpace);

	for (size_t i = 0; i < pScene->mNumMeshes; i++) {
		meshes.push_back(ParseMesh(*pScene->mMeshes[i], pScene->mMaterials, file_path));
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

std::unique_ptr<Mesh> Model::ParseMesh(const aiMesh& mesh, const aiMaterial* const* ppMaterial,
	const std::filesystem::path& full_path) noexcept
{
	namespace dv = DynamicVertex;
	struct MaterialStruct {
		alignas(16) glm::vec3 mat_color = {1, 1, 1};
		alignas(16) glm::vec3 mat_specular_color = { 1, 1, 1 };
		int specular_power = 128;
		int has_diffuse = false;
		int has_specular = false;
		int has_normal = false;
		float padding[3];
	};
	std::vector<std::shared_ptr<Bindable>> bindables;
	const aiMaterial& mat = *ppMaterial[mesh.mMaterialIndex];
	dv::VertexLayout layout;

	layout.Append(dv::VertexLayout::Type::Position3D).
		Append(dv::VertexLayout::Type::Normal).
		Append(dv::VertexLayout::Type::Texture2D).
		Append(dv::VertexLayout::Type::Tangent).
		Append(dv::VertexLayout::Type::BiTangent);

	dv::VertexBuffer vbuf(layout);
	for (size_t i = 0; i < mesh.mNumVertices; i++) {
		aiVector3D& v = mesh.mVertices[i];
		aiVector3D& n = mesh.mNormals[i];
		aiVector3D& t = mesh.mTextureCoords[0][i];
		aiVector3D& ta = mesh.mBitangents[i];
		aiVector3D& bta = mesh.mBitangents[i];
		vbuf.EmplaceBack(glm::vec3{ v.x, v.y, v.z }, glm::vec3{ n.x, n.y, n.z }, glm::vec2{ t.x, t.y }, glm::vec3{ ta.x, ta.y, ta.z }, glm::vec3{bta.x, bta.y, bta.z});

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
	MaterialStruct mat_struct;
	bool has_alpha = false;
	const std::string path = full_path.parent_path().string() + "/";

	const auto mesh_tag = path + "#" + mesh.mName.C_Str();

	if (mesh.mMaterialIndex >= 0) {

		auto min_filter = Opengl::TextureFilter::LINEAR_MIPMAP_NEAREST;
		auto mag_filter = Opengl::TextureFilter::NEAREST;
		auto wrap = Opengl::TextureWrap::REPEAT;

		const aiMaterial& mat = *ppMaterial[mesh.mMaterialIndex];
		aiString texture_file_name;

		if (mat.GetTexture(aiTextureType_DIFFUSE, 0, &texture_file_name) == aiReturn_SUCCESS) {
			auto texture = BindableCodex::Resolve<TextureObject>(path + std::string(texture_file_name.C_Str()), min_filter, mag_filter, wrap, 0);
			has_alpha = texture->HasAlpha();
			bindables.push_back(texture);
			mat_struct.has_diffuse = true;
		}
		else {
			aiColor3D c;
			mat.Get(AI_MATKEY_COLOR_DIFFUSE, c);
			mat_struct.has_diffuse = false;
			mat_struct.mat_color.x = c.r;
			mat_struct.mat_color.y = c.g;
			mat_struct.mat_color.z = c.b;
		}

		if (mat.GetTexture(aiTextureType_SPECULAR, 0, &texture_file_name) == aiReturn_SUCCESS) {
			bindables.push_back(BindableCodex::Resolve<TextureObject>(path + std::string(texture_file_name.C_Str()), min_filter, mag_filter, wrap, 1));
			mat_struct.has_specular = true;
		}
		else {
			mat_struct.has_specular = false;
			mat.Get(AI_MATKEY_SHININESS, mat_struct.specular_power);
			//mat.Get(AI_MATKEY_SHININESS_STRENGTH, mat_struct.specular_intensity);
			aiColor3D c;
			mat.Get(AI_MATKEY_COLOR_SPECULAR, c);
			mat_struct.mat_specular_color.x = c.r;
			mat_struct.mat_specular_color.y = c.g;
			mat_struct.mat_specular_color.z = c.b;
		}

		if (mat.GetTexture(aiTextureType_NORMALS, 0, &texture_file_name) == aiReturn_SUCCESS) {
			bindables.push_back(BindableCodex::Resolve<TextureObject>(path + std::string(texture_file_name.C_Str()), min_filter, mag_filter, wrap, 2));
			mat_struct.has_normal = true;
		}
	}
	auto shader = BindableCodex::Resolve<ShaderObject>("Assets/Shaders/phong_VS.glsl", "Assets/Shaders/phong_FS.glsl");
	shader->LoadTextureSlot("uDiffuse", 0);
	shader->LoadTextureSlot("uSpecular", 1);
	shader->LoadTextureSlot("uNormal", 2);
	bindables.push_back(std::move(shader));
	bindables.push_back(BindableCodex::Resolve<Rasterizer>(has_alpha));
	bindables.push_back(BindableCodex::Resolve<VertexBufferObject>(mesh_tag, layout, Opengl::CreateIndexBuffer(indices.size(), indices.data()), vbuf));
	auto ubuf = std::make_shared<UniformBufferObject<MaterialStruct>>(UniformBufferObject<MaterialStruct>(2));
	ubuf->Update(mat_struct);
	bindables.push_back(ubuf);


	return std::make_unique<Mesh>(std::move(bindables), (unsigned int)indices.size());
}