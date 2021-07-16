#include "pch.h"
#include "render_system.h"

#include "ecs.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

RenderSystem::RenderSystem() :
	mShader("Default Shader")
{
	mShader.loadVertexShader("res/shaders/vertex.glsl");
	mShader.loadFragmentShader("res/shaders/fragment.glsl");
	mShader.compileAllShaders();

	mModelLoc = mShader.loadUniformLocation("uModel");
	mProjLoc = mShader.loadUniformLocation("uProj");
	mViewLoc = mShader.loadUniformLocation("uView");
}

void RenderSystem::render(const Camera& camera) const noexcept
{
	auto& ecs = ECS::getInstance();
	glm::mat4x4 model(1.0f);
	
	glUseProgram(mShader.getProgram());
	for (const auto& e : mEntities)
	{
		TransformComponent& transform = ecs.getComponent<TransformComponent>(e);
		MeshComponent& mesh = ecs.getComponent<MeshComponent>(e);

		model = glm::scale(glm::mat4x4(1.0f), glm::vec3(transform.scale));
		model = model * glm::toMat4(transform.rotation);
		model = glm::translate(model, transform.position);

		glUniformMatrix4fv(mProjLoc, 1, GL_FALSE, glm::value_ptr(camera.getProj()));
		glUniformMatrix4fv(mViewLoc, 1, GL_FALSE, glm::value_ptr(camera.getView()));
		glUniformMatrix4fv(mModelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(mesh.vao);
		glDrawArrays(GL_TRIANGLES, 0, mesh.vertex_count);
	}
}