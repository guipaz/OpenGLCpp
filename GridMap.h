#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include "Mesh.h"
#include "GameObject.h"
#include <memory>

struct GridMapCell
{
	unsigned int x;
	float y;
	unsigned int z;

	unsigned int textureID;
	
	glm::vec2 textureUVA;
	glm::vec2 textureUVB;
	glm::vec2 textureUVC;
	glm::vec2 textureUVD;

	// y-mod of each tile corner
	float nwModY; //  nw ._____. ne
	float neModY; //     |     |
	float swModY; //     |     |
	float seModY; //  sw ._____. se

	bool smoothEdge = true;
};

class GridMap : public GameObject
{
public:
	unsigned int columns, rows;
	float cellSize, heightStep;
	GridMap(unsigned int columns, unsigned int rows, float cellSize, float heightStep);
	
private:
	std::unique_ptr<Mesh> _generatedMesh;
	std::vector<GridMapCell> _cells;

	void generateMesh();
	void generateCells();
};

