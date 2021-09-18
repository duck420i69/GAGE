#include "pch.h"
#include "TestPlane.h"

#include "DynamicVertex.h"
#include "TransformUBuf.h"
#include "ShaderObject.h"
#include "TextureObject.h"
#include "VertexBufferObject.h"
#include "StencilObject.h"
#include "TechniqueProbe.h"

#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>

TestPlane::TestPlane()
{
	namespace dv = DynamicVertex;

	dv::VertexLayout layout;
	layout.Append(dv::VertexLayout::Type::Position3D);
	layout.Append(dv::VertexLayout::Type::Texture2D);

	dv::VertexBuffer buffer(layout);


	buffer.EmplaceBack(glm::vec3{ -1.0f, 1.0f, -1.0f }, glm::vec2{ 0,0 });
	buffer.EmplaceBack(glm::vec3{ -1.0f, -1.0f, -1.0f }, glm::vec2{ 0, 1 });
	buffer.EmplaceBack(glm::vec3{ 1.0f, -1.0f, -1.0f }, glm::vec2{ 1,1 });
	buffer.EmplaceBack(glm::vec3{ 1.0f, 1.0f, -1.0f }, glm::vec2{ 1,0 });
	buffer.EmplaceBack(glm::vec3{ -1.0f, 1.0f, 1.0f }, glm::vec2{ 0,0 });
	buffer.EmplaceBack(glm::vec3{ -1.0f, -1.0f, 1.0f }, glm::vec2{ 0,1 });
	buffer.EmplaceBack(glm::vec3{ 1.0f, -1.0f, 1.0f }, glm::vec2{ 1,1 });
	buffer.EmplaceBack(glm::vec3{ 1.0f, 1.0f, 1.0f }, glm::vec2{ 1,0 });
	buffer.EmplaceBack(glm::vec3{ 1.0f, 1.0f, -1.0f }, glm::vec2{ 0,0 });
	buffer.EmplaceBack(glm::vec3{ 1.0f, -1.0f, -1.0f }, glm::vec2{ 0,1 });
	buffer.EmplaceBack(glm::vec3{ 1.0f, -1.0f, 1.0f }, glm::vec2{ 1,1 });
	buffer.EmplaceBack(glm::vec3{ 1.0f, 1.0f, 1.0f }, glm::vec2{ 1,0 });
	buffer.EmplaceBack(glm::vec3{ -1.0f, 1.0f, -1.0f }, glm::vec2{ 0,0 });
	buffer.EmplaceBack(glm::vec3{ -1.0f, -1.0f, -1.0f }, glm::vec2{ 0, 1 });
	buffer.EmplaceBack(glm::vec3{ -1.0f, -1.0f, 1.0f }, glm::vec2{ 1,1 });
	buffer.EmplaceBack(glm::vec3{ -1.0f, 1.0f, 1.0f }, glm::vec2{ 1,0 });
	buffer.EmplaceBack(glm::vec3{ -1.0f, 1.0f, 1.0f }, glm::vec2{ 0,0 });
	buffer.EmplaceBack(glm::vec3{ -1.0f, 1.0f, -1.0f }, glm::vec2{ 0,1 });
	buffer.EmplaceBack(glm::vec3{ 1.0f, 1.0f, -1.0f }, glm::vec2{ 1,1 });
	buffer.EmplaceBack(glm::vec3{ 1.0f, 1.0f, 1.0f }, glm::vec2{ 1,0 });
	buffer.EmplaceBack(glm::vec3{ -1.0f, -1.0f, 1.0f }, glm::vec2{ 0,0 });
	buffer.EmplaceBack(glm::vec3{ -1.0f, -1.0f, -1.0f }, glm::vec2{ 0, 1 });
	buffer.EmplaceBack(glm::vec3{ 1.0f, -1.0f, -1.0f }, glm::vec2{ 1,1 });
	buffer.EmplaceBack(glm::vec3{ 1.0f, -1.0f, 1.0f }, glm::vec2{ 1,0 });



	std::vector<unsigned int> indices = { 0,1,3,
				3,1,2,
				4,5,7,
				7,5,6,
				8,9,11,
				11,9,10,
				12,13,15,
				15,13,14,
				16,17,19,
				19,17,18,
				20,21,23,
				23,21,22
	};
	this->mVertexData = BindableCodex::Resolve<VertexBufferObject>("Plane", layout, Opengl::CreateIndexBuffer(indices.size(), indices.data()), buffer);
	this->SetVertexCount(indices.size());

	{
		Technique standard("Standard");
		{
			Step only(0);
			DynamicUniform::Layout layout;
			layout["mat_color"] = DynamicUniform::Type::Vec3;
			layout["mat_specular_color"] = DynamicUniform::Type::Vec3;
			layout["specular_power"] = DynamicUniform::Type::Int;
			layout["has_diffuse"] = DynamicUniform::Type::Int;
			layout["has_specular"] = DynamicUniform::Type::Int;
			layout["has_normal"] = DynamicUniform::Type::Int;
			DynamicUniform::Buffer buffer(layout);
			buffer["mat_color"] = glm::vec3{1, 1, 1};
			buffer["mat_specular_color"] = glm::vec3{1, 1, 1};
			buffer["specular_power"] = 16;
			buffer["has_diffuse"] = true;
			buffer["has_specular"] = false;
			buffer["has_normal"] = false;

			only.AddBindable(std::make_shared<UniformBufferObjectDynamic>(2, buffer));

			only.AddBindable(std::make_shared<TransformUBuf>());
			only.AddBindable(BindableCodex::Resolve<ShaderObject>("Assets/Shaders/phong_VS.glsl", "Assets/Shaders/phong_FS.glsl"));
			only.AddBindable(BindableCodex::Resolve<TextureObject>("Assets/Models/brick_wall/brick_wall_diffuse.jpg" ,Opengl::TextureFilter::NEAREST_MIPMAP_LINEAR,
				 Opengl::TextureFilter::NEAREST,
				Opengl::TextureWrap::CLAMP_TO_EDGE, 0u));
			standard.AddStep(only);
		}
		AddTechnique(standard);

	}
	{
		Technique outline("outline");
		{
			{
				Step mask(1);
				mask.AddBindable(std::make_shared<TransformUBuf>());
				mask.AddBindable(BindableCodex::Resolve<ShaderObject>("Assets/Shaders/texture_VS.glsl", "Assets/Shaders/null_FS.glsl"));
				outline.AddStep(mask);
			}
			{
				Step draw(2);
				class TransformUBufScaled : public TransformUBuf {
					mutable DynamicUniform::Buffer buf;
				public:
					TransformUBufScaled(float scale = 1.04f) noexcept :
						buf(MakeLayout()) 
					{
						buf["outline_scale"] = scale;
					}
					void Bind() const noexcept override {
						assert(mParent != nullptr && "Parent is null!");

						const auto scale = *reinterpret_cast<float*>(buf["outline_scale"].location);

						glm::mat4 parent_scaled = glm::scale(mParent->GetTransform(), { scale,scale,scale });

						glm::mat4 model_view = Opengl::GetCamera() * parent_scaled;
						s_buff->Update({ model_view, Opengl::GetProjection() * model_view });
						s_buff->Bind();
					}

					void Accept(TechniqueProbe& probe) noexcept {
						probe.VisitBuffer(buf);
					}
				private:
					static DynamicUniform::Layout MakeLayout() noexcept {
						DynamicUniform::Layout layout;
						layout["outline_scale"] = DynamicUniform::Type::Float;
						return layout;
					}
				};
				DynamicUniform::Layout layout;
				layout["outline_color"] = DynamicUniform::Type::Vec3;
				DynamicUniform::Buffer buffer(layout);
				buffer["outline_color"] = glm::vec3{1, 0, 0};

				draw.AddBindable(std::make_shared<UniformBufferObjectDynamic>(2, buffer));
				draw.AddBindable(std::make_shared<TransformUBufScaled>());
				draw.AddBindable(BindableCodex::Resolve<ShaderObject>("Assets/Shaders/solid_VS.glsl", "Assets/Shaders/solid_FS.glsl"));
				outline.AddStep(draw);
			}

		}
		AddTechnique(outline);
	}
}

void TestPlane::SpawmControlWindow(const char* name) noexcept
{
	class Probe final : public TechniqueProbe {
	public:
		void OnSetTechnique(class  Technique*) noexcept override {
			ImGui::TextColored({ 0.4f, 1.0f, 0.6f, 1.0f }, mTech->GetName().c_str());
			ImGui::Checkbox(("Tech_active##" + std::to_string(mTechID)).c_str(), &mTech->GetActiveRef());
		};
		bool OnVisitBuffer(class DynamicUniform::Buffer& buf) noexcept override {
			float dirty = false;
			auto dcheck = [&dirty](bool changed) {dirty = dirty || changed; };
			auto tag = [tag_scratch = std::string(), tag_string = "##" + std::to_string(mBufferID)](const char* label) mutable {
				tag_scratch = label + tag_string;
				return tag_scratch.c_str();
			};


			if (DynamicUniform::Buffer::ElementRef v = buf["specular_power"]; v.Exist()) {
				dcheck(ImGui::SliderInt(tag("specular_power"), reinterpret_cast<int*>(v.location), 16, 256));
			}

			if (DynamicUniform::Buffer::ElementRef v = buf["outline_color"]; v.Exist()) {
				dcheck(ImGui::ColorEdit3(tag("outline_color"), reinterpret_cast<float*>(v.location)));
			}


			if (DynamicUniform::Buffer::ElementRef v = buf["outline_scale"]; v.Exist()) {
				dcheck(ImGui::DragFloat(tag("outline_scale"), reinterpret_cast<float*>(v.location), 0.001f, 1.0f, 2.0f));
			}


			return dirty;
		}
	} probe;
	ImGui::Begin(name);

	ImGui::DragFloat3("Position", &mPos.x, 0.1f);
	Accept(probe);

	ImGui::End();
}

glm::mat4x4 TestPlane::GetTransform() const noexcept
{
	return glm::translate(glm::mat4(1.0f), mPos);
}
