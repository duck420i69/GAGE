#include "pch.h"
#include "render_system.h"

#include "ecs.h"

#include <glad/glad.h>

void RenderSystem::render()
{
	auto& ecs = ECS::getInstance();

	for (const auto& e : mEntities)
	{
		TransformComponent& transform = ecs.getComponent<TransformComponent>(e);
		MeshComponent& mesh = ecs.getComponent<MeshComponent>(e);

		glBindVertexArray(mesh.vao);
		glDrawArrays(GL_TRIANGLES, 0, mesh.vertex_count);
	}
}