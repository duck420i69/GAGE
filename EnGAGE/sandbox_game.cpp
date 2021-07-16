#include "pch.h"
#include "sandbox_game.h"

#include "imgui/imgui.h"

#include "ecs.h"
#include "input.h"
#include "loader.h"

#include <glad/glad.h>

static Entity test;
static MeshComponent mesh;

void SandboxGame::init() noexcept
{
	std::vector<float> vertex_data =
	{
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
	};

	mesh = Loader::loadMesh(vertex_data);
	

	auto& ecs = ECS::getInstance();
	test = ecs.constructEntity({"Test entity"}, {
		glm::vec3{0, 0, 0}, glm::quat{1, 0, 0, 0}, 1
	});

	ecs.addComponent(test, mesh);
}

void SandboxGame::update(float delta) noexcept
{

}

void SandboxGame::processCamera(Camera& camera, float delta) noexcept
{
	if (Input::isKeyDown(Input::KEY_S))
	{
		camera.getPosition().z += delta * 0.1f;
	}
	if (Input::isKeyDown(Input::KEY_W))
	{
		camera.getPosition().z -= delta * 0.1f;
	}
}


void SandboxGame::shutdown() noexcept
{
	
}
