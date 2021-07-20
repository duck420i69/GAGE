#include "pch.h"
#include "LevelEditorScene.h"

#include <glad/glad.h>

#include "imgui/imgui.h"

static std::string g_vertex_source =
	"#version 460 core\n\
	layout(location = 0) in vec3 inPos;\n\
	layout(location = 1) in vec3 inColor;\n\
	out vec3 FS_Color;\n\
	void main()\n\
	{\
		FS_Color = inColor;\n\
		gl_Position = vec4(inPos, 1);\n\
	}";

static std::string g_fragment_source = 
	"#version 460 core\n\
	in vec3 FS_Color;\n\
	out vec4 out_Color;\n\
	void main()\n\
	{\n\
		out_Color = vec4(FS_Color, 1);\n\
	}";

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

static uint32_t g_vertex, g_fragment, g_program;
static uint32_t g_vao, g_vbo, g_ebo;

LevelEditorScene::LevelEditorScene()
{
	g_vertex = glCreateShader(GL_VERTEX_SHADER);
	g_fragment = glCreateShader(GL_FRAGMENT_SHADER);
	g_program = glCreateProgram();

	const char* vertex_source_c_str = g_vertex_source.c_str();
	const char* fragment_source_c_str = g_fragment_source.c_str();
	glShaderSource(g_vertex, 1, &vertex_source_c_str, nullptr);
	glShaderSource(g_fragment, 1, &fragment_source_c_str, nullptr);

	glCompileShader(g_vertex);
	glCompileShader(g_fragment);

	glAttachShader(g_program, g_vertex);
	glAttachShader(g_program, g_fragment);
	glLinkProgram(g_program);
	glValidateProgram(g_program);

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
	glDeleteProgram(g_program);
	glDeleteShader(g_vertex);
	glDeleteShader(g_fragment);

	glDeleteVertexArrays(1, &g_vao);
	glDeleteBuffers(1, &g_vbo);
	glDeleteBuffers(1, &g_ebo);
}

void LevelEditorScene::Update(double delta) noexcept
{
	ImGui::Begin("Level editor");
	ImGui::Text("Program id: %i", g_program);
	ImGui::Text("vertex id: %i", g_vertex);
	ImGui::Text("fragment id: %i", g_fragment);
	ImGui::Text("vao id: %i", g_vao);
	ImGui::Text("vbo id: %i", g_vbo);
	ImGui::Text("ebo id: %i", g_ebo);
	ImGui::End();
}

void LevelEditorScene::Render() noexcept
{
	glUseProgram(g_program);
	glBindVertexArray(g_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ebo);
	glDrawElements(GL_TRIANGLES, sizeof(g_element_data) / sizeof(uint32_t), GL_UNSIGNED_INT, nullptr);

}
