#include "pch.h"
#include "LevelEditorScene.h"

#include <glad/glad.h>

#include "thirdparty/imgui/imgui.h"


static float g_vertex_data[] = 
{ 
	/* Position                           Color             UV*/
	0.5f, -0.5f,  0.0f,					1, 0, 0,			1, 0,
	-0.5f, 0.5f,  0.0f,					0, 1, 0,			0, 1,
	0.5f, 0.5f,   0.0f,					0, 0, 1,			1, 1,
	-0.5f, -0.5f, 0.0f,					1, 0, 1,			0, 0
};

static uint32_t g_element_data[] =
{
	2, 1, 0,
	0, 1, 3
};

static uint32_t g_vao, g_vbo, g_ebo;


LevelEditorScene::LevelEditorScene() :
	Scene(std::make_shared<Camera>(glm::vec3(0, 0, 0))),
	mTestTexture("Assets/Textures/ignore_this.png")
{
	mCamera->SetPosition(0, 0, 1.0f);
	mShader.LoadVertex("Assets/Shaders/vertex.glsl");
	mShader.LoadFragment("Assets/Shaders/fragment.glsl");
	mShader.Create();



	/*Create vao, vbo, ebo*/
	glGenVertexArrays(1, &g_vao);
	glGenBuffers(1, &g_vbo);
	glGenBuffers(1, &g_ebo);

	glBindVertexArray(g_vao);
	glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (const void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (const void*)(sizeof(float) * 3));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (const void*)(sizeof(float) * 6));
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
	mCamera->UpdateProjection();
}

void LevelEditorScene::Render() noexcept
{
	mShader.Bind();
	mShader.UploadMat4x4("uProjection", glm::value_ptr(mCamera->GetProjection()));
	mShader.UploadMat4x4("uView", glm::value_ptr(mCamera->GetViewMatrix()));
	mShader.UploadTexture("uTexSampler", 0);
	mTestTexture.Bind(0);
	glBindVertexArray(g_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ebo);
	glDrawElements(GL_TRIANGLES, sizeof(g_element_data) / sizeof(uint32_t), GL_UNSIGNED_INT, nullptr);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}
