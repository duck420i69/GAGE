#include "pch.h"
#include "Mesh.h"

#include "BindableCodex.h"
#include "VertexBufferObject.h"
#include "Material.h"

Mesh::Mesh(const Material& material, const aiMesh& mesh,
	const std::filesystem::path& full_path, float scale) noexcept
{
	DynamicVertex::VertexLayout layout;

	layout.Append(DynamicVertex::VertexLayout::Type::Position3D).
		Append(DynamicVertex::VertexLayout::Type::Normal).
		Append(DynamicVertex::VertexLayout::Type::Texture2D).
		Append(DynamicVertex::VertexLayout::Type::Tangent).
		Append(DynamicVertex::VertexLayout::Type::BiTangent);

	const auto mesh_tag = full_path.parent_path().string() + "/" + mesh.mName.C_Str();

	DynamicVertex::VertexBuffer vbuf(layout);
	for (size_t i = 0; i < mesh.mNumVertices; i++) {
		aiVector3D& v = mesh.mVertices[i];
		aiVector3D& n = mesh.mNormals[i];
		aiVector3D& t = mesh.mTextureCoords[0][i];
		aiVector3D& ta = mesh.mBitangents[i];
		aiVector3D& bta = mesh.mBitangents[i];
		vbuf.EmplaceBack(glm::vec3{ v.x, v.y, v.z } * scale, glm::vec3{ n.x, n.y, n.z }, glm::vec2{ t.x, t.y }, glm::vec3{ ta.x, ta.y, ta.z }, glm::vec3{ bta.x, bta.y, bta.z });

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
	this->SetVertexCount(indices.size());
	mVertexData = BindableCodex::Resolve<VertexBufferObject>(mesh_tag, layout, Opengl::CreateIndexBuffer(indices.size(), indices.data()), vbuf);

	for (auto& tech : material.GetTechniques()) {
		AddTechnique(tech);
	}


}
