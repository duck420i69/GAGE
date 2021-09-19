#include "pch.h"
#include "Material.h"

#include "DynamicUniform.h"
#include "BindableCodex.h"
#include "TextureObject.h"
#include "Rasterizer.h"
#include "ShaderObject.h"
#include "TransformUBuf.h"

#include <glm/gtx/transform.hpp>

Material::Material(const aiMaterial& material, const std::filesystem::path& model_path) noexcept :
	mModelPath(model_path.string())
{
	const auto root_path = model_path.parent_path().string() + "/";
	{
		aiString mat_name;
		material.Get(AI_MATKEY_NAME, mat_name);
		mName = mat_name.C_Str();
	}

	{
		Technique phong("phong");
		Step step(0);
		aiString texture_file;
		auto texture_min_filter = Opengl::TextureFilter::LINEAR_MIPMAP_LINEAR;
		auto texture_mag_filter = Opengl::TextureFilter::LINEAR;
		auto texture_wrap = Opengl::TextureWrap::REPEAT;
		DynamicUniform::Layout uniform_layout;
		uniform_layout.PushBack("mat_color", DynamicUniform::Type::Vec3);
		uniform_layout.PushBack("mat_specular_color", DynamicUniform::Type::Vec3);
		uniform_layout.PushBack("specular_power", DynamicUniform::Type::Int);
		uniform_layout.PushBack("has_diffuse", DynamicUniform::Type::Int);
		uniform_layout.PushBack("has_specular", DynamicUniform::Type::Int);
		uniform_layout.PushBack("has_normal", DynamicUniform::Type::Int);
		DynamicUniform::Buffer uniform_buffer(uniform_layout);


		//Diffuse
		{
			bool has_alpha = false;
			if (material.GetTexture(aiTextureType_DIFFUSE, 0, &texture_file) == aiReturn_SUCCESS) {
				uniform_buffer["has_diffuse"] = 1;
				auto texture = BindableCodex::Resolve<TextureObject>(root_path + texture_file.C_Str(),
					texture_min_filter,
					texture_mag_filter,
					texture_wrap, 0u);

				has_alpha = texture->HasAlpha();

				step.AddBindable(std::move(texture));
			}
			else {
				uniform_buffer["has_diffuse"] = 0;


			}
			step.AddBindable(BindableCodex::Resolve<Rasterizer>(has_alpha));
		}

		//Specular 
		{
			if (material.GetTexture(aiTextureType_SPECULAR, 0, &texture_file) == aiReturn_SUCCESS) {
				uniform_buffer["has_specular"] = 1;
				auto texture = BindableCodex::Resolve<TextureObject>(root_path + texture_file.C_Str(),
					texture_min_filter,
					texture_mag_filter,
					texture_wrap, 1u);


				step.AddBindable(std::move(texture));
			}
			else {
				uniform_buffer["has_specular"] = 0;
			}


		}

		//Normal
		{
			if (material.GetTexture(aiTextureType_NORMALS, 0, &texture_file) == aiReturn_SUCCESS) {
				uniform_buffer["has_normal"] = 1;
				auto texture = BindableCodex::Resolve<TextureObject>(root_path + texture_file.C_Str(),
					texture_min_filter,
					texture_mag_filter,
					texture_wrap, 2u);


				step.AddBindable(std::move(texture));
			}
			else {
				uniform_buffer["has_normal"] = 0;
			}
		}

		//Common
		{
			step.AddBindable(std::make_shared<TransformUBuf>());

			auto shader = BindableCodex::Resolve<ShaderObject>("Assets/Shaders/phong_VS.glsl", "Assets/Shaders/phong_FS.glsl");
			shader->LoadTextureSlot("uDiffuse", 0);
			shader->LoadTextureSlot("uSpecular", 1);
			shader->LoadTextureSlot("uNormal", 2);
			step.AddBindable(shader);

			aiColor3D color;
			material.Get(AI_MATKEY_COLOR_SPECULAR, color);
			uniform_buffer["mat_specular_color"] = glm::vec3{ color.r, color.g, color.b };

			material.Get(AI_MATKEY_COLOR_DIFFUSE, color);
			uniform_buffer["mat_color"] = glm::vec3{ color.r, color.g, color.b };

			float specular_power = 0.0f;
			material.Get(AI_MATKEY_SHININESS, specular_power);
			uniform_buffer["specular_power"] = int(specular_power);

			step.AddBindable(std::make_shared<UniformBufferObjectDynamic>(2, uniform_buffer));
		}
		phong.AddStep(step);
		mTechniques.push_back(std::move(phong));

	}

	{
		Technique outline("outline", false);
		{
			{
				Step mask(1);
				mask.AddBindable(std::make_shared<TransformUBuf>());
				mask.AddBindable(BindableCodex::Resolve<ShaderObject>("Assets/Shaders/texture_VS.glsl", "Assets/Shaders/null_FS.glsl"));
				outline.AddStep(mask);
			}
			{
				Step draw(2);
				/*class TransformUBufScaled : public TransformUBuf {
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
				};*/
				DynamicUniform::Layout layout;
				layout["outline_color"] = DynamicUniform::Type::Vec3;
				DynamicUniform::Buffer buffer(layout);
				buffer["outline_color"] = glm::vec3{ 1, 0, 0 };

				layout.Reset();
				layout.PushBack("offset", DynamicUniform::Type::Float);
				DynamicUniform::Buffer reserved_buffer(layout);
				reserved_buffer["offset"] = 0.1f;

				draw.AddBindable(std::make_shared<UniformBufferObjectDynamic>(2, buffer));
				draw.AddBindable(std::make_shared<UniformBufferObjectDynamic>(3, reserved_buffer));
				draw.AddBindable(std::make_shared<TransformUBuf>());
				draw.AddBindable(BindableCodex::Resolve<ShaderObject>("Assets/Shaders/offset_VS.glsl", "Assets/Shaders/solid_FS.glsl"));
				outline.AddStep(draw);
			}

		}
		mTechniques.push_back(outline);
	}
}
