#pragma once

#include "Drawable.h"

#include "DynamicVertex.h"
#include "BindableCodex.h"
#include "VertexBufferObject.h"
#include "VertexLayoutObject.h"
#include "ShaderObject.h"
#include "UniformBufferObject.h"
#include "TextureObject.h"
#include "TransformUBuf.h"

class Plane final : public Drawable {
	struct MaterialUBuf {
		float specular_intensity;
		int specular_power;
		int enable_normal;
		float padding;
	} material;
	std::shared_ptr<UniformBufferObject<MaterialUBuf>> mMatBuf;
public:
	Plane() noexcept {
		material.specular_intensity = 0.1f;
		material.specular_power = 45;
		material.enable_normal = true;

		namespace dv = DynamicVertex;
		dv::VertexLayout layout;
		layout.Append(dv::VertexLayout::Type::Position3D)
			.Append(dv::VertexLayout::Type::Normal)
			.Append(dv::VertexLayout::Type::Texture2D);

		dv::VertexBuffer vbuf(layout);
		vbuf.EmplaceBack(glm::vec3{ 0, 0, 0 }, glm::vec3{ 0, 0, 1 }, glm::vec2{ 0, 0 });
		vbuf.EmplaceBack(glm::vec3{ 0, 1, 0 }, glm::vec3{ 0, 0, 1 }, glm::vec2{ 0, 1 });
		vbuf.EmplaceBack(glm::vec3{ 1, 1, 0 }, glm::vec3{ 0, 0, 1 }, glm::vec2{ 1, 1 });
		vbuf.EmplaceBack(glm::vec3{ 1, 0, 0 }, glm::vec3{ 0, 0, 1 }, glm::vec2{ 1, 0 });


		std::vector<unsigned int> indices{0, 1, 2, 0 ,2, 3};	
		mMatBuf = std::make_shared<UniformBufferObject<MaterialUBuf>>(UniformBufferObject<MaterialUBuf>(2));
		mMatBuf->Update(material);
		AddBind(mMatBuf);

		AddBind(BindableCodex::Resolve<TextureObject>("Assets/Models/brick_wall/brick_wall_diffuse.jpg", Opengl::TextureFilter::LINEAR, Opengl::TextureFilter::LINEAR, Opengl::TextureWrap::REPEAT, 0));

		AddBind(BindableCodex::Resolve<TextureObject>("Assets/Models/brick_wall/brick_wall_normal.jpg", Opengl::TextureFilter::LINEAR, Opengl::TextureFilter::LINEAR, Opengl::TextureWrap::REPEAT, 1));

		auto shader = BindableCodex::Resolve<ShaderObject>("Assets/Shaders/phong_VS.glsl", "Assets/Shaders/phong_normalmap_FS.glsl");
		shader->LoadTextureSlot("uDiffuse", 0);
		shader->LoadTextureSlot("uNormal", 1);
		AddBind(std::move(shader));
		AddBind(BindableCodex::Resolve<VertexBufferObject>("plane_mesh", vbuf));
		AddBind(BindableCodex::Resolve<IndexBufferObject>("plane_mesh", indices));
		AddBind(BindableCodex::Resolve<VertexLayoutObject>(layout));

		AddBind(std::make_unique<TransformUBuf>(*this));
	}
	glm::mat4x4 GetTransform() const noexcept {
		return glm::scale(glm::mat4x4(1.0f), glm::vec3(30, 30, 30));
	}

	void SpawnControlWindow() noexcept {
		ImGui::Begin("Plane");
		bool check = (bool)material.enable_normal;
		bool dirty = ImGui::Checkbox("Enable normal mapping", &check);
		material.enable_normal = check;

		if (dirty) {
			mMatBuf->Update(material);
		}
		ImGui::End();
	}
};