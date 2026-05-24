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
	unsigned int X;
	float Y;
	unsigned int Z;

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
	unsigned int Columns, Rows, CellSize, HeightStep;
	GridMap(unsigned int columns, unsigned int rows, unsigned int cellSize, float heightStep);
	void generateMesh();
	void generateTexture();

private:
	std::unique_ptr<Shader> _mapGenerationShader;
	std::unique_ptr<Mesh> _generatedMesh;
	std::vector<GridMapCell> _cells;
};

