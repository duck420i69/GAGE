#include "pch.h"
#include "sandbox_game.h"

#include "imgui/imgui.h"

#include "ecs.h"

#include <glad/glad.h>

Entity test;
uint32_t vao, vbo;

void SandboxGame::init()
{
	std::vector<float> vertex_data =
	{
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
	};

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertex_data.size() * sizeof(float), vertex_data.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	auto& ecs = ECS::getInstance();
	test = ecs.createEntity();

	ecs.addComponent(test, TransformComponent{
		{0, 0, 0}, {0, 0, 0, 1}, 1
	});
	ecs.addComponent(test, MeshComponent
	{
		vao, 3
	});
}

void SandboxGame::update(float delta)
{
	
}


void SandboxGame::shutdown()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}
