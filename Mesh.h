#pragma once
#include <glm/glm.hpp>

class Mesh
{
public:
	Mesh(glm::vec3* vertices, size_t vertexCount, 
		 unsigned int* indices, size_t indexCount, 
		 glm::vec2* uvs);
	void draw();
	~Mesh();

private:
	unsigned int _VAO;
	unsigned int _EBO;

	glm::vec3* _vertices;
	unsigned int* _indices;
	glm::vec2* _uvs;
	size_t _indexCount;
};

