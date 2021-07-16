#include "pch.h"
#include "loader.h"

#include <glad/glad.h>

std::vector<uint32_t> Loader::sVAO, Loader::sVBO;

MeshComponent Loader::loadMesh(const std::vector<float>& data) noexcept
{
	MeshComponent m;
	m.vertex_count = data.size() / 3;

	glGenVertexArrays(1, &m.vao);
	glGenBuffers(1, &m.vbo);
	glBindVertexArray(m.vao);
	glBindBuffer(GL_ARRAY_BUFFER, m.vbo);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	sVAO.push_back(m.vao);
	sVBO.push_back(m.vbo);
	return m;
}

void Loader::shutdown()
{
	for (const auto& vao : sVAO)
	{
		glDeleteVertexArrays(1, &vao);
	}
	for (const auto& vbo : sVBO)
	{
		glDeleteBuffers(1, &vbo);
	}
}



