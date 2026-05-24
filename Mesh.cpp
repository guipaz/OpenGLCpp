#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Mesh.h"

Mesh::Mesh(glm::vec3* vertices, size_t vertexCount, unsigned int* indices, size_t indexCount, glm::vec2* uvs)
{
	static_assert(sizeof(glm::vec3) == 3 * sizeof(float));
	static_assert(sizeof(glm::vec2) == 2 * sizeof(float));

	_vertices = vertices;
	_indices = indices;
	_uvs = uvs;
	_indexCount = indexCount;

	/// VAO
	// generates and binds the VAO to store attributes
	glGenVertexArrays(1, &_VAO);
	glBindVertexArray(_VAO);

	// generates and binds the VBO to store the vertices
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(glm::vec3), _vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);

	// generates and binds the VBO to store the uvs
	unsigned int uv_VBO;
	glGenBuffers(1, &uv_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, uv_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(glm::vec2), _uvs, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
	glEnableVertexAttribArray(1);

	/// EBO
	// generates and binds the EBO to store indices
	glGenBuffers(1, &_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), _indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::draw()
{
	glBindVertexArray(_VAO);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, 0);
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &_EBO);
	glDeleteBuffers(1, &_VAO);
}
