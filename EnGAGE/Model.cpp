#include "pch.h"
#include "Model.h"

#include "TextureObject.h"
#include "VertexBufferObject.h"
#include "TransformUBuf.h"
#include "ShaderObject.h"
#include "DynamicVertex.h"
#include "Logger.h"

#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <imgui.h>

Mesh::Mesh(std::vector<std::shared_ptr<Bindable>> bindptrs, const int vertex_count) noexcept :
	Drawable(vertex_count)
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

	std::vector<std::shared_ptr<Bindable>> bindables;

	const aiMaterial& mat = *ppMaterial[mesh.mMaterialIndex];

	//Material
	bool has_diffuse = false;
	bool has_specular_map = false;
	bool has_normal_map = false;
	float shininess = 35.0f;
	glm::vec3 mat_color = { 1, 1, 1 };
	const std::string path = full_path.parent_path().string() + "\\";
	if (mesh.mMaterialIndex >= 0) {

		auto min_filter = Opengl::TextureFilter::LINEAR_MIPMAP_NEAREST;
		auto mag_filter = Opengl::TextureFilter::NEAREST;
		auto wrap = Opengl::TextureWrap::REPEAT;

		const aiMaterial& mat = *ppMaterial[mesh.mMaterialIndex];
		aiString texture_file_name;

		if (mat.GetTexture(aiTextureType_DIFFUSE, 0, &texture_file_name) == aiReturn_SUCCESS) {
			bindables.push_back(BindableCodex::Resolve<TextureObject>(path + std::string(texture_file_name.C_Str()), min_filter, mag_filter, wrap, 0));
			has_diffuse = true;
		}
		else {
			aiColor3D c;
			mat.Get(AI_MATKEY_COLOR_DIFFUSE, c);
			mat_color.r = c.r;
			mat_color.g = c.g;
			mat_color.b = c.b;
		}

		if (mat.GetTexture(aiTextureType_SPECULAR, 0, &texture_file_name) == aiReturn_SUCCESS) {
			bindables.push_back(BindableCodex::Resolve<TextureObject>(path + std::string(texture_file_name.C_Str()), min_filter, mag_filter, wrap, 1));
			has_specular_map = true;
		}
		else {
			mat.Get(AI_MATKEY_SHININESS, shininess);
		}

		if (mat.GetTexture(aiTextureType_NORMALS, 0, &texture_file_name) == aiReturn_SUCCESS) {
			bindables.push_back(BindableCodex::Resolve<TextureObject>(path + std::string(texture_file_name.C_Str()), min_filter, mag_filter, wrap, 2));
			has_normal_map = true;
		}
	}


	const auto mesh_tag = path + "#" + mesh.mName.C_Str();
	//Indices
	std::vector<unsigned int> indices;
	indices.reserve((size_t)mesh.mNumFaces * 3);
	for (unsigned int i = 0; i < mesh.mNumFaces; i++) {
		const auto& face = mesh.mFaces[i];
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}


	if (has_diffuse && has_specular_map && has_normal_map) {
		dv::VertexLayout layout;
		layout.
			Append(dv::VertexLayout::Type::Position3D).
			Append(dv::VertexLayout::Type::Normal).
			Append(dv::VertexLayout::Type::Texture2D).
			Append(dv::VertexLayout::Type::Tangent).
			Append(dv::VertexLayout::Type::BiTangent);
		dv::VertexBuffer buf(layout);

		//Vertices
		for (unsigned int i = 0; i < mesh.mNumVertices; i++) {
			buf.EmplaceBack(
				glm::vec3{ mesh.mVertices[i].x, mesh.mVertices[i].y, mesh.mVertices[i].z },
				glm::vec3{ mesh.mNormals[i].x, mesh.mNormals[i].y, mesh.mNormals[i].z },
				glm::vec2{ mesh.mTextureCoords[0][i].x, mesh.mTextureCoords[0][i].y },
				glm::vec3{ mesh.mTangents[i].x, mesh.mTangents[i].y, mesh.mTangents[i].z },
				glm::vec3{ mesh.mBitangents[i].x, mesh.mBitangents[i].y, mesh.mBitangents[i].z });

		}

		//Load shader

		auto shader = BindableCodex::Resolve < ShaderObject>("Assets/Shaders/phong_normalmap_VS.glsl", "Assets/Shaders/phong_normalspecmap_FS.glsl");
		shader->LoadTextureSlot("uDiffuse", 0);
		shader->LoadTextureSlot("uSpecular", 1);
		shader->LoadTextureSlot("uNormal", 2);


		//Load bindables
		bindables.push_back(std::move(shader));
		VertexBuffer ib = Opengl::CreateIndexBuffer(indices.size(), indices.data());
		bindables.push_back(BindableCodex::Resolve<VertexBufferObject>(mesh_tag, layout, ib, buf));


	}
	else if (has_diffuse && has_normal_map) {
		dv::VertexLayout layout;
		layout.
			Append(dv::VertexLayout::Type::Position3D).
			Append(dv::VertexLayout::Type::Normal).
			Append(dv::VertexLayout::Type::Texture2D).
			Append(dv::VertexLayout::Type::Tangent).
			Append(dv::VertexLayout::Type::BiTangent);
		dv::VertexBuffer buf(layout);

		//Vertices
		for (unsigned int i = 0; i < mesh.mNumVertices; i++) {
			buf.EmplaceBack(
				glm::vec3{ mesh.mVertices[i].x, mesh.mVertices[i].y, mesh.mVertices[i].z },
				glm::vec3{ mesh.mNormals[i].x, mesh.mNormals[i].y, mesh.mNormals[i].z },
				glm::vec2{ mesh.mTextureCoords[0][i].x, mesh.mTextureCoords[0][i].y },
				glm::vec3{ mesh.mTangents[i].x, mesh.mTangents[i].y, mesh.mTangents[i].z },
				glm::vec3{ mesh.mBitangents[i].x, mesh.mBitangents[i].y, mesh.mBitangents[i].z });

		}
		//Load shader

		auto shader = BindableCodex::Resolve < ShaderObject>("Assets/Shaders/phong_normalmap_VS.glsl", "Assets/Shaders/phong_normalmap_FS.glsl");
		shader->LoadTextureSlot("uDiffuse", 0);
		shader->LoadTextureSlot("uNormal", 2);


		//Uniform buffer
		struct MaterialUBuf {
			float specular_intensity;
			float specular_power;
			int enable_normal;
			float padding;
		} material;
		material.specular_intensity = 15.8f;
		material.specular_power = shininess;
		material.enable_normal = true;

		auto mat_ubuf = std::make_shared<UniformBufferObject<MaterialUBuf>>(UniformBufferObject<MaterialUBuf>(2));
		mat_ubuf->Update(material);

		//Load bindables
		bindables.push_back(mat_ubuf);
		bindables.push_back(std::move(shader));
		VertexBuffer ib = Opengl::CreateIndexBuffer(indices.size(), indices.data());
		bindables.push_back(BindableCodex::Resolve<VertexBufferObject>(mesh_tag, layout, ib, buf));
	}
	else if (has_diffuse && has_specular_map) {
		dv::VertexLayout layout;
		layout.
			Append(dv::VertexLayout::Type::Position3D).
			Append(dv::VertexLayout::Type::Normal).
			Append(dv::VertexLayout::Type::Texture2D);
		dv::VertexBuffer buf(layout);

		//Vertices
		for (unsigned int i = 0; i < mesh.mNumVertices; i++) {
			buf.EmplaceBack(
				glm::vec3{ mesh.mVertices[i].x, mesh.mVertices[i].y, mesh.mVertices[i].z },
				glm::vec3{ mesh.mNormals[i].x, mesh.mNormals[i].y, mesh.mNormals[i].z },
				glm::vec2{ mesh.mTextureCoords[0][i].x, mesh.mTextureCoords[0][i].y });

		}
		//Load shader

		auto shader = BindableCodex::Resolve < ShaderObject>("Assets/Shaders/phong_VS.glsl", "Assets/Shaders/phong_specmap_FS.glsl");
		shader->LoadTextureSlot("uDiffuse", 0);
		shader->LoadTextureSlot("uSpecular", 1);


		//Uniform buffer
		struct MaterialUBuf {
			float specular_intensity;
			float specular_power;
			float padding[2];
		} material;
		material.specular_intensity = 15.8f;
		material.specular_power = shininess;

		auto mat_ubuf = std::make_shared<UniformBufferObject<MaterialUBuf>>(UniformBufferObject<MaterialUBuf>(2));
		mat_ubuf->Update(material);

		//Load bindables
		bindables.push_back(mat_ubuf);
		bindables.push_back(std::move(shader));
		VertexBuffer ib = Opengl::CreateIndexBuffer(indices.size(), indices.data());
		bindables.push_back(BindableCodex::Resolve<VertexBufferObject>(mesh_tag, layout, ib, buf));
	}
	else if (has_diffuse && !has_normal_map && !has_specular_map) {
		dv::VertexLayout layout;
		layout.
			Append(dv::VertexLayout::Type::Position3D).
			Append(dv::VertexLayout::Type::Normal).Append(dv::VertexLayout::Type::Texture2D);
		dv::VertexBuffer buf(layout);

		//Vertices
		for (unsigned int i = 0; i < mesh.mNumVertices; i++) {
			buf.EmplaceBack(
				glm::vec3{ mesh.mVertices[i].x, mesh.mVertices[i].y, mesh.mVertices[i].z },
				glm::vec3{ mesh.mNormals[i].x, mesh.mNormals[i].y, mesh.mNormals[i].z },
				glm::vec2{ mesh.mTextureCoords[0][i].x, mesh.mTextureCoords[0][i].y });

		}

		//Load shader

		auto shader = BindableCodex::Resolve < ShaderObject>("Assets/Shaders/phong_VS.glsl", "Assets/Shaders/phong_FS.glsl");


		//Uniform buffer
		struct MaterialUBuf {
			glm::vec3 mat_color;
			float specular_intensity;
			float specular_power;
			int has_texture;

			float padding[2];
		} material;
		material.mat_color = mat_color;
		material.specular_intensity = 15.8f;
		material.specular_power = shininess;
		material.has_texture = true;

		auto mat_ubuf = std::make_shared<UniformBufferObject<MaterialUBuf>>(UniformBufferObject<MaterialUBuf>(2));
		mat_ubuf->Update(material);

		//Load bindables
		bindables.push_back(mat_ubuf);
		bindables.push_back(std::move(shader));
		VertexBuffer ib = Opengl::CreateIndexBuffer(indices.size(), indices.data());
		bindables.push_back(BindableCodex::Resolve<VertexBufferObject>(mesh_tag, layout, ib, buf));
	}
	else if (!has_diffuse && !has_normal_map && !has_specular_map) {
		dv::VertexLayout layout;
		layout.
			Append(dv::VertexLayout::Type::Position3D).
			Append(dv::VertexLayout::Type::Normal).Append(dv::VertexLayout::Type::Texture2D);
		dv::VertexBuffer buf(layout);

		//Vertices
		for (unsigned int i = 0; i < mesh.mNumVertices; i++) {
			buf.EmplaceBack(
				glm::vec3{ mesh.mVertices[i].x, mesh.mVertices[i].y, mesh.mVertices[i].z },
				glm::vec3{ mesh.mNormals[i].x, mesh.mNormals[i].y, mesh.mNormals[i].z },
				glm::vec2{ mesh.mTextureCoords[0][i].x, mesh.mTextureCoords[0][i].y });

		}
		//Load shader

		auto shader = BindableCodex::Resolve < ShaderObject>("Assets/Shaders/phong_VS.glsl", "Assets/Shaders/phong_FS.glsl");


		//Uniform buffer
		struct MaterialUBuf {
			glm::vec3 mat_color;
			float specular_intensity;
			float specular_power;
			int has_texture;

			float padding[2];
		} material;
		material.mat_color = mat_color;
		material.specular_intensity = 15.8f;
		material.specular_power = shininess;
		material.has_texture = false;

		auto mat_ubuf = std::make_shared<UniformBufferObject<MaterialUBuf>>(UniformBufferObject<MaterialUBuf>(2));
		mat_ubuf->Update(material);

		//Load bindables
		bindables.push_back(mat_ubuf);
		bindables.push_back(std::move(shader));
		VertexBuffer ib = Opengl::CreateIndexBuffer(indices.size(), indices.data());
		bindables.push_back(BindableCodex::Resolve<VertexBufferObject>(mesh_tag, layout, ib, buf));

	}
	return std::make_unique<Mesh>(std::move(bindables), indices.size());
}