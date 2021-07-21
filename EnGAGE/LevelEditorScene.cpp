#include "pch.h"
#include "LevelEditorScene.h"

#include <glad/glad.h>

#include "imgui/imgui.h"


static float g_vertex_data[] = 
{ 
	/* Position                           Color*/
	0.5f, -0.5f, 0.0f,					1, 0, 0,
	-0.5f, 0.5f, 0.0f,					0, 1, 0,
	0.5f, 0.5f, 0.0f,					0, 0, 1,
	-0.5f, -0.5f, 0.0f,					1, 0, 1,
};

static uint32_t g_element_data[] =
{
	2, 1, 0,
	0, 1, 3
};

static uint32_t g_vao, g_vbo, g_ebo;

LevelEditorScene::LevelEditorScene()
{
	mShader.LoadVertex("assets/shaders/vertex.glsl");
	mShader.LoadFragment("assets/shaders/fragment.glsl");
	mShader.Create();

	//TODO: check for shader errors


	/*Create vao, vbo, ebo*/
	glGenVertexArrays(1, &g_vao);
	glGenBuffers(1, &g_vbo);
	glGenBuffers(1, &g_ebo);

	glBindVertexArray(g_vao);
	glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (const void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (const void*)(sizeof(float) * 3));
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_data), g_vertex_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_element_data), g_element_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

LevelEditorScene::~LevelEditorScene()
{

	glDeleteVertexArrays(1, &g_vao);
	glDeleteBuffers(1, &g_vbo);
	glDeleteBuffers(1, &g_ebo);
}

void LevelEditorScene::Update(double delta) noexcept
{
	ImGui::Begin("Level editor");
	ImGui::Text("vao id: %i", g_vao);
	ImGui::Text("vbo id: %i", g_vbo);
	ImGui::Text("ebo id: %i", g_ebo);
	ImGui::End();
}

void LevelEditorScene::Render() noexcept
{
	mShader.Bind();
	glBindVertexArray(g_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ebo);
	glDrawElements(GL_TRIANGLES, sizeof(g_element_data) / sizeof(uint32_t), GL_UNSIGNED_INT, nullptr);

}
